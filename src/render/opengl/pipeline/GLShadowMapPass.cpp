#include "render/opengl/pipeline/GLShadowMapPass.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLUniformManager.hpp"
#include "render/opengl/GLModel.hpp"

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
, shadow_map_width_(width)
, shadow_map_height_(height)
, fbo_id_(0)
, shadow_map_textures_()
{
}

GLShadowMapPass::~GLShadowMapPass()
{
    glDeleteFramebuffers(1, &fbo_id_);
}

void GLShadowMapPass::Initialize()
{
    InitializeTextures();
    InitializeFrameBufferObject();
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

void GLShadowMapPass::InitializeFrameBufferObject()
{
    assert(!shadow_map_textures_.empty());
    glGenFramebuffers(1, &fbo_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map_textures_[0]->GetNativeIdentifier(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
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

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    glViewport(0, 0, shadow_map_width_, shadow_map_height_);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Compute matrices
    const math::Matrix4x4 coordinate_correction_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(0.5F))
            .Translate(math::Vec3f(0.5F));
    std::vector<math::Matrix4x4> projection_matrices = ComputeCascadedProjectionMatrices(camera);
    math::Matrix4x4 view_matrix = math::Matrix4x4::LookAt(math::Vec3f::Zero(),
                                                          math::Vec3f::Normalize(directional_light.direction_),
                                                          math::Vec3f(0.0F, 0.0F, 1.0F));

    std::array<math::Matrix4x4, kShadowCascadeCount> light_matrices{};
    // TODO: Create end clip spaces
    std::array<float, kShadowCascadeCount> cascade_end_clip_space{20.0F, 50.0F, 75.0F};
    for (uint32 cascade_index = 0; cascade_index < kShadowCascadeCount; ++cascade_index)
    {
        std::shared_ptr<GLTexture> cascade_texture = shadow_map_textures_[cascade_index];
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, cascade_texture->GetNativeIdentifier(), 0);

        // Render cascaded shadow maps
        gl_uniform_manager_->UpdateCameraUniforms(projection_matrices[cascade_index], view_matrix, math::Vec3f::Zero());
        RenderEntities(view_matrix, registry, viewable_entities);

        // Keep track of light matrix
        math::Matrix4x4 light_matrix = (coordinate_correction_matrix * projection_matrices[cascade_index] * view_matrix);
        light_matrices[cascade_index] = (light_matrix.Transpose());
    }
    gl_uniform_manager_->UpdateShadowMapMatrices(light_matrices, cascade_end_clip_space);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool GLShadowMapPass::GetShadowCastingDirectionalLight(entt::registry& registry, DirectionalLight& out_directional_light) const
{
    auto directional_light_view = registry.view<const DirectionalLight>();
    for (Entity directional_light_entity : directional_light_view)
    {
        const DirectionalLight& directional_light = directional_light_view.get(directional_light_entity);
        if (directional_light.casts_shadows_)
        {
            out_directional_light = directional_light;
            return true;
        }
    }
    return false;
}

std::vector<math::Matrix4x4> GLShadowMapPass::ComputeCascadedProjectionMatrices(const Camera& camera) const
{
    std::vector<math::Matrix4x4> projection_matrices{};
    projection_matrices.reserve(kShadowCascadeCount);

    // TODO: Compute all 3 cascaded shadow maps. Replace dummy projection matrices
    float left   = -10;    float right = 10;
    float bottom = -10;    float top   = 10;
    float near   = -10.0F; float far   = 20;
    math::Matrix4x4 projection_matrix = math::Matrix4x4::Orthographic(left, right, bottom, top, near, far);
    projection_matrices.push_back(projection_matrix);
    projection_matrices.push_back(projection_matrix);
    projection_matrices.push_back(projection_matrix);

    return projection_matrices;
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

