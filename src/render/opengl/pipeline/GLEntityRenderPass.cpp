#include "render/opengl/pipeline/GLEntityRenderPass.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLProgram.hpp"
#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLUniformManager.hpp"

namespace zero::render
{

const char* GLEntityRenderPass::kTitle = "GLEntityRenderPass";

GLEntityRenderPass::GLEntityRenderPass(GLCompiler* gl_compiler,
                                       GLModelManager* gl_model_manager,
                                       GLPrimitiveMeshManager* gl_primitive_mesh_manager,
                                       GLTextureManager* gl_texture_manager,
                                       GLUniformManager* gl_uniform_manager,
                                       Logger& logger)
: gl_compiler_(gl_compiler)
, gl_model_manager_(gl_model_manager)
, gl_primitive_mesh_manager_(gl_primitive_mesh_manager)
, gl_texture_manager_(gl_texture_manager)
, gl_uniform_manager_(gl_uniform_manager)
, logger_(logger)
, shadow_map_sampler_(std::make_shared<GLSampler>())
, diffuse_map_sampler_(std::make_shared<GLSampler>())
{
    shadow_map_sampler_->Initialize();
    shadow_map_sampler_->SetMinificationFilter(ISampler::Filter::LINEAR);
    shadow_map_sampler_->SetMagnificationFilter(ISampler::Filter::LINEAR);
    shadow_map_sampler_->SetWrappingS(ISampler::Wrapping::CLAMP_TO_BORDER);
    shadow_map_sampler_->SetWrappingT(ISampler::Wrapping::CLAMP_TO_BORDER);
    shadow_map_sampler_->SetBorderColour(math::Vec4f::One());
    glSamplerParameteri(shadow_map_sampler_->GetNativeIdentifier(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glSamplerParameteri(shadow_map_sampler_->GetNativeIdentifier(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    diffuse_map_sampler_->Initialize();
    diffuse_map_sampler_->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    diffuse_map_sampler_->SetMagnificationFilter(ISampler::Filter::LINEAR);
    diffuse_map_sampler_->SetWrappingS(ISampler::Wrapping::REPEAT);
    diffuse_map_sampler_->SetWrappingT(ISampler::Wrapping::REPEAT);
}

void GLEntityRenderPass::Execute(const Camera& camera,
                                 entt::registry& registry,
                                 const std::vector<Entity>& viewable_entities)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    UpdateGLSettings(camera);

    math::Matrix4x4 projection_matrix = camera.GetProjectionMatrix();
    math::Matrix4x4 view_matrix = camera.GetViewMatrix();
    gl_uniform_manager_->UpdateCameraUniforms(projection_matrix, view_matrix, camera.position_);

    auto renderable_view = registry.view<const Transform, const Material, const Volume>();
    auto model_view      = registry.view<const Transform, const Material, const Volume, const ModelInstance>();
    auto primitive_view  = registry.view<const Transform, const Material, const Volume, const PrimitiveInstance>();

    for (Entity viewable_entity : viewable_entities)
    {
        assert(renderable_view.contains(viewable_entity));
        assert(model_view.contains(viewable_entity) || primitive_view.contains(viewable_entity));

        const Transform& transform = renderable_view.get<const Transform>(viewable_entity);
        const Material& material = renderable_view.get<const Material>(viewable_entity);
        const Volume& volume = renderable_view.get<const Volume>(viewable_entity);
        ToggleWireframeMode(material.wireframe_enabled_);
        ToggleBackfaceCulling(!material.two_sided_);

        auto graphics_program = gl_compiler_->CreateProgram(material);
        if (!graphics_program)
        {
            continue;
        }

        // Bind uniform blocks
        gl_uniform_manager_->BindCameraUniforms(graphics_program);
        gl_uniform_manager_->BindModelUniforms(graphics_program);
        gl_uniform_manager_->BindLightUniforms(graphics_program);
        gl_uniform_manager_->BindMaterialUniforms(graphics_program);
        gl_uniform_manager_->BindShadowMapUniforms(graphics_program);
        graphics_program->Use();

        // Update Model and Material uniform blocks
        math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
        gl_uniform_manager_->UpdateModelUniforms(model_matrix, (view_matrix * model_matrix).Inverse());
        gl_uniform_manager_->UpdateMaterialUniforms(material);

        // Textures
        std::vector<std::shared_ptr<GLTexture>> shadow_map_textures = gl_texture_manager_->GetShadowMapTextures();
        for (int32 texture_unit_index = 0; texture_unit_index < GLUniformManager::kShadowCascadeCount; ++texture_unit_index)
        {
            // Map sampler2D uniform name with the texture unit
            graphics_program->SetUniform(GLUniformManager::GetShadowSamplerUniformName(texture_unit_index), texture_unit_index);

            // Bind texture to the texture unit
            std::shared_ptr<GLTexture> shadow_map_texture = shadow_map_textures[texture_unit_index];
            glActiveTexture(GL_TEXTURE0 + texture_unit_index);
            glBindTexture(shadow_map_texture->GetTarget(), shadow_map_texture->GetNativeIdentifier());

            // Set texture sampler
            gl_texture_manager_->SetSampler(shadow_map_sampler_, texture_unit_index);
        }

        int32 diffuse_texture_unit_index = GLUniformManager::kShadowCascadeCount;
        graphics_program->SetUniform(GLUniformManager::GetDiffuseSamplerName(), diffuse_texture_unit_index);
        std::shared_ptr<GLTexture> gl_diffuse_texture = gl_texture_manager_->GetGLTexture(material.texture_map_.diffuse_map_);
        glActiveTexture(GL_TEXTURE0 + diffuse_texture_unit_index);
        glBindTexture(gl_diffuse_texture->GetTarget(), gl_diffuse_texture->GetNativeIdentifier());
        gl_texture_manager_->SetSampler(diffuse_map_sampler_, diffuse_texture_unit_index);

        graphics_program->FlushUniforms();

        // Mesh
        if (model_view.contains(viewable_entity))
        {
            const ModelInstance& model_instance = model_view.get<const ModelInstance>(viewable_entity);
            std::shared_ptr<IModel> model = gl_model_manager_->GetModel(model_instance);
            model->Draw();
        }
        else
        {
            const PrimitiveInstance& primitive_instance = primitive_view.get<const PrimitiveInstance>(viewable_entity);
            std::shared_ptr<GLMesh> primitive = gl_primitive_mesh_manager_->GetPrimitiveMesh(primitive_instance);
            primitive->Draw();
        }

        // Unbind textures
        for (int32 texture_unit_index = 0; texture_unit_index < GLUniformManager::kShadowCascadeCount + 1; ++texture_unit_index)
        {
            glActiveTexture(GL_TEXTURE0 + texture_unit_index);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        graphics_program->Finish();
    }
}

void GLEntityRenderPass::UpdateGLSettings(const Camera& camera) const
{
    glViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void GLEntityRenderPass::RenderVolume(const Camera& camera,
                                      const math::Matrix4x4& projection_matrix,
                                      const math::Matrix4x4& view_matrix,
                                      const Volume& volume)
{
    ToggleWireframeMode(true);
    ToggleBackfaceCulling(false);

    math::Matrix4x4 model_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(volume.bounding_volume_.radius_))
            .Translate(volume.bounding_volume_.center_);
    // use default shaders
    std::shared_ptr<IProgram> gl_primitive_program = gl_compiler_->CreateProgram(Material{});
    gl_primitive_program->Use();
    math::Matrix4x4 model_view_matrix = view_matrix * model_matrix;
    gl_primitive_program->SetUniform("projection_matrix", projection_matrix);
    gl_primitive_program->SetUniform("model_view_matrix", model_view_matrix);
    gl_primitive_program->SetUniform("normal_matrix", model_view_matrix.Inverse().Transpose().GetMatrix3x3());
    gl_primitive_program->SetUniform("camera_position", camera.position_);
    gl_primitive_program->SetUniform("material.specular_exponent", 32.0F);
    gl_primitive_program->SetUniform("material.diffuse_color", math::Vec3f(1.0F, 0.0F, 0.0F));
    gl_primitive_program->FlushUniforms();

    PrimitiveInstance primitive_instance{};
    primitive_instance.Set(Sphere());
    std::shared_ptr<GLMesh> gl_primitive = gl_primitive_mesh_manager_->GetPrimitiveMesh(primitive_instance);
    gl_primitive->Draw();
    gl_primitive_program->Finish();
}

void GLEntityRenderPass::ToggleWireframeMode(bool enable_wireframe_mode)
{
    if (enable_wireframe_mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void GLEntityRenderPass::ToggleBackfaceCulling(bool enable_backface_culling)
{
    if (enable_backface_culling)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

} // namespace zero::render

