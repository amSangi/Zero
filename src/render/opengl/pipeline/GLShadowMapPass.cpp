#include "render/opengl/pipeline/GLShadowMapPass.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLUniformManager.hpp"
#include "render/opengl/GLModel.hpp"
#include <limits>

namespace zero::render
{

const uint32 GLShadowMapPass::kShadowCascadeCount = GLUniformManager::kShadowCascadeCount;

GLShadowMapPass::GLShadowMapPass(GLCompiler* gl_compiler,
                                 GLModelManager* gl_model_manager,
                                 GLPrimitiveMeshManager* gl_primitive_mesh_manager,
                                 GLTextureManager* gl_texture_manager,
                                 GLUniformManager* gl_uniform_manager,
                                 uint32 width,
                                 uint32 height)
: gl_compiler_(gl_compiler)
, gl_model_manager_(gl_model_manager)
, gl_primitive_mesh_manager_(gl_primitive_mesh_manager)
, gl_uniform_manager_(gl_uniform_manager)
, gl_texture_manager_(gl_texture_manager)
, cascaded_shadow_map_(kShadowCascadeCount)
, shadow_map_width_(width)
, shadow_map_height_(height)
, fbo_ids_(kShadowCascadeCount, 0)
, shadow_map_textures_()
{
}

GLShadowMapPass::~GLShadowMapPass()
{
    glDeleteFramebuffers(fbo_ids_.size(), fbo_ids_.data());
}

void GLShadowMapPass::Initialize()
{
    InitializeTextures();
    InitializeFrameBufferObjects();
    gl_texture_manager_->SetShadowMapTextures(shadow_map_textures_);
}

void GLShadowMapPass::InitializeTextures()
{
    for (uint32 i = 0; i < kShadowCascadeCount; ++i)
    {
        GLuint shadow_map_texture_id = 0;
        glGenTextures(1, &shadow_map_texture_id);
        glBindTexture(GL_TEXTURE_2D, shadow_map_texture_id);
        glTexImage2D(GL_TEXTURE_2D,         // Target texture.
                     0,                     // Level of detail. Level n is the nth mipmap reduction image.
                     GL_DEPTH_COMPONENT,    // Number of colour components in the texture.
                     shadow_map_width_,     // Texture image width.
                     shadow_map_height_,    // Texture image height.
                     0,                     // Border. Must be 0.
                     GL_DEPTH_COMPONENT,    // Format of the pixel data.
                     GL_FLOAT,              // Data type of the pixel data.
                     nullptr);              // Pointer to the image data in memory.

        shadow_map_textures_.push_back(std::make_shared<GLTexture>(shadow_map_texture_id, GL_TEXTURE_2D));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLShadowMapPass::InitializeFrameBufferObjects()
{
    assert(!shadow_map_textures_.empty());
    glGenFramebuffers(fbo_ids_.size(), fbo_ids_.data());
    for (uint32 i = 0; i < kShadowCascadeCount; ++i)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_ids_[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map_textures_[i]->GetNativeIdentifier(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLShadowMapPass::Execute(const Camera& camera,
                              entt::registry& registry,
                              const std::vector<Entity>& viewable_entities)
{
    DirectionalLight directional_light{};
    if (!GetShadowCastingDirectionalLight(registry, directional_light))
    {
        return;
    }

    cascaded_shadow_map_.Update(camera, directional_light);
    std::vector<math::Matrix4x4> light_view_matrices = cascaded_shadow_map_.GetLightViewMatrices();
    std::vector<math::Matrix4x4> light_projection_matrices = cascaded_shadow_map_.GetProjectionMatrices();

    // Render depth maps
    for (uint32 cascade_index = 0; cascade_index < kShadowCascadeCount; ++cascade_index)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_ids_[cascade_index]);
        UpdateGLSettings();

        // Render cascaded shadow maps
        gl_uniform_manager_->UpdateCameraUniforms(light_projection_matrices[cascade_index], light_view_matrices[cascade_index], math::Vec3f::Zero());
        RenderEntities(light_view_matrices[cascade_index], registry, viewable_entities);
    }
    gl_uniform_manager_->UpdateShadowMapMatrices(cascaded_shadow_map_.GetTextureMatrices(), cascaded_shadow_map_.GetViewFarBounds());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLShadowMapPass::UpdateGLSettings() const
{
    glViewport(0, 0, shadow_map_width_, shadow_map_height_);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

bool GLShadowMapPass::GetShadowCastingDirectionalLight(entt::registry& registry, DirectionalLight& out_directional_light)
{
    auto directional_light_view = registry.view<const DirectionalLight>();
    for (Entity directional_light_entity : directional_light_view)
    {
        const DirectionalLight& directional_light = directional_light_view.get(directional_light_entity);
        // Retrieve the first shadow casting directional light
        if (directional_light.casts_shadows_)
        {
            out_directional_light = directional_light;
            return true;
        }
    }
    return false;
}


void GLShadowMapPass::RenderEntities(const math::Matrix4x4& light_view_matrix,
                                     entt::registry& registry,
                                     const std::vector<Entity>& viewable_entities)
{
    auto renderable_view = registry.view<const Transform, const Material, const Volume>();
    auto model_view      = registry.view<const Transform, const Material, const Volume, const ModelInstance>();
    auto primitive_view  = registry.view<const Transform, const Material, const Volume, const PrimitiveInstance>();

    for (Entity viewable_entity : viewable_entities)
    {
        assert(renderable_view.contains(viewable_entity));
        assert(model_view.contains(viewable_entity) || primitive_view.contains(viewable_entity));

        const Transform& transform = renderable_view.get<const Transform>(viewable_entity);
        const Material& material = renderable_view.get<const Material>(viewable_entity);

        math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
        std::shared_ptr<GLProgram> graphics_program = gl_compiler_->CreateProgram(material.shaders_.vertex_shader_,
                                                                                  "shadow_map.fragment.glsl");
        graphics_program->Use();
        gl_uniform_manager_->BindCameraUniforms(graphics_program);
        gl_uniform_manager_->BindModelUniforms(graphics_program);
        gl_uniform_manager_->UpdateModelUniforms(model_matrix, (light_view_matrix * model_matrix).Inverse());

        // Draw Mesh
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

        graphics_program->Finish();
    }
}

} // namespace zero::render

