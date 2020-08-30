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
    shadow_map_sampler_->SetWrappingS(ISampler::Wrapping::CLAMP_TO_EDGE);
    shadow_map_sampler_->SetWrappingT(ISampler::Wrapping::CLAMP_TO_EDGE);

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
    constexpr uint8 shadow_map_texture_unit_index = 0;
    constexpr uint8 diffuse_map_texture_unit_index = 1;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);

    math::Matrix4x4 projection_matrix = camera.GetProjectionMatrix();
    math::Matrix4x4 view_matrix = camera.GetViewMatrix();
    gl_uniform_manager_->UpdateCameraUniforms(projection_matrix, view_matrix, camera.position_);

    auto renderable_view = registry.view<const Transform, const Material, const Volume>();
    auto model_view = registry.view<const ModelInstance>();
    auto primitive_view = registry.view<const PrimitiveInstance>();

    for (Entity viewable_entity : viewable_entities)
    {
        const auto& transform = renderable_view.get<const Transform>(viewable_entity);
        const auto& material = renderable_view.get<const Material>(viewable_entity);
        const auto& volume = renderable_view.get<const Volume>(viewable_entity);
        if (camera.render_bounding_volumes_)
        {
            RenderVolume(camera, projection_matrix, view_matrix, volume);
        }
        ToggleWireframeMode(material.wireframe_enabled_);
        ToggleBackfaceCulling(!material.two_sided_);

        auto graphics_program = gl_compiler_->CreateProgram(material);
        if (!graphics_program)
        {
            continue;
        }
        GLUniformManager::BindCameraUniforms(graphics_program);
        GLUniformManager::BindModelUniforms(graphics_program);
        GLUniformManager::BindLightUniforms(graphics_program);
        GLUniformManager::BindMaterialUniforms(graphics_program);
        GLUniformManager::BindShadowMapUniforms(graphics_program);
        graphics_program->Use();

        // Update sampler uniforms
        graphics_program->SetUniform(GLUniformManager::GetShadowSamplerUniformName(), shadow_map_texture_unit_index);
        graphics_program->SetUniform(GLUniformManager::GetDiffuseSamplerName(), diffuse_map_texture_unit_index);

        // Update uniforms
        math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
        gl_uniform_manager_->UpdateModelUniforms(model_matrix, (view_matrix * model_matrix).Inverse());
        gl_uniform_manager_->UpdateMaterialUniforms(material);

        graphics_program->FlushUniforms();

        // Bind textures
        std::shared_ptr<GLTexture> shadow_map_texture = gl_texture_manager_->GetShadowMapTexture();
        std::shared_ptr<GLTexture> gl_diffuse_texture = gl_texture_manager_->GetGLTexture(material.texture_map_.diffuse_map_);
        glActiveTexture(GL_TEXTURE0 + shadow_map_texture_unit_index);
        glBindTexture(shadow_map_texture->GetTarget(), shadow_map_texture->GetNativeIdentifier());
        if (gl_diffuse_texture)
        {
            glActiveTexture(GL_TEXTURE0 + diffuse_map_texture_unit_index);
            glBindTexture(gl_diffuse_texture->GetTarget(), gl_diffuse_texture->GetNativeIdentifier());
        }

        gl_texture_manager_->SetSampler(shadow_map_sampler_, shadow_map_texture_unit_index);
        gl_texture_manager_->SetSampler(diffuse_map_sampler_, diffuse_map_texture_unit_index);

        // Draw Mesh
        if (registry.has<ModelInstance>(viewable_entity))
        {
            const auto& model_instance = model_view.get(viewable_entity);
            auto model = gl_model_manager_->GetModel(model_instance);
            if (model)
            {
                model->Draw();
            }
        }
        else
        {
            const auto& primitive_instance = primitive_view.get(viewable_entity);
            auto primitive = gl_primitive_mesh_manager_->GetPrimitiveMesh(primitive_instance);
            if (primitive)
            {
                primitive->Draw();
            }
        }

        // Unbind textures
        glActiveTexture(GL_TEXTURE0 + diffuse_map_texture_unit_index);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0 + shadow_map_texture_unit_index);
        glBindTexture(GL_TEXTURE_2D, 0);

        graphics_program->Finish();
    }
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

