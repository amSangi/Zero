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
    UpdateGLSettings();

    // Convert from NDC [-1, 1] to Texture Coordinates [0, 1]
    const math::Matrix4x4 coordinate_correction_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(0.5F))
            .Translate(math::Vec3f(0.5F));
    math::Matrix4x4 light_view_matrix = math::Matrix4x4::LookAt(math::Vec3f::Zero(),
                                                                math::Vec3f::Normalize(directional_light.direction_),
                                                                math::Vec3f(0.0F, 0.0F, 1.0F));
    // Set the far z position for each cascade
    float z_distance = camera.far_clip_ - camera.near_clip_;
    // Set cascade far clip boundaries
    // Each cascade is capped to a certain z distance
    std::vector<float> cascade_end_clip_spaces{ math::Min(25.0F, math::Ceil(z_distance * 0.10F)), // First cascade may cover first 10% of the scene
                                                math::Min(125.0F, math::Ceil(z_distance * 0.40F)), // Second cascade may cover the next 30% of the scene
                                                math::Ceil(z_distance)};                           // Third cascade may cover the remaining 60% of the scene
    std::vector<math::Matrix4x4> projection_matrices = ComputeCascadedProjectionMatrices(camera, light_view_matrix, cascade_end_clip_spaces);

    // Render depth maps
    std::vector<math::Matrix4x4> light_matrices{};
    light_matrices.reserve(kShadowCascadeCount);
    for (uint32 cascade_index = 0; cascade_index < kShadowCascadeCount; ++cascade_index)
    {
        std::shared_ptr<GLTexture> cascade_texture = shadow_map_textures_[cascade_index];
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, cascade_texture->GetNativeIdentifier(), 0);

        // Render cascaded shadow maps
        gl_uniform_manager_->UpdateCameraUniforms(projection_matrices[cascade_index], light_view_matrix, math::Vec3f::Zero());
        RenderEntities(light_view_matrix, registry, viewable_entities);

        // Keep track of light matrix
        math::Matrix4x4 light_matrix = (coordinate_correction_matrix * projection_matrices[cascade_index] * light_view_matrix);
        light_matrices.push_back(light_matrix.Transpose());
    }
    gl_uniform_manager_->UpdateShadowMapMatrices(light_matrices, cascade_end_clip_spaces);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLShadowMapPass::UpdateGLSettings()
{
    glViewport(0, 0, shadow_map_width_, shadow_map_height_);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

bool GLShadowMapPass::GetShadowCastingDirectionalLight(entt::registry& registry, DirectionalLight& out_directional_light) const
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

std::vector<math::Matrix4x4> GLShadowMapPass::ComputeCascadedProjectionMatrices(const Camera& camera,
                                                                                const math::Matrix4x4& light_view_matrix,
                                                                                const std::vector<float>& cascade_end_clip_spaces) const
{
    std::vector<math::Matrix4x4> projection_matrices{};
    projection_matrices.reserve(kShadowCascadeCount);

    // Compute near and far boundaries for all of the cascades
    std::vector<float> z_boundaries{};
    z_boundaries.reserve(kShadowCascadeCount + 1);
    z_boundaries.push_back(camera.near_clip_);
    for (float end_clip_space : cascade_end_clip_spaces)
    {
        z_boundaries.push_back(end_clip_space);
    }

    // Inverse view transform
    math::Matrix4x4 inverse_view_transform = camera.GetCameraToWorldMatrix();
    float aspect_ratio = camera.viewport_.GetAspectRatio();
    float tan_half_h_fov = math::Tan(camera.horizontal_field_of_view_.ToRadian().rad_ * 0.5F);
    float tan_half_v_fov = math::Tan(camera.horizontal_field_of_view_.ToRadian().rad_ * 0.5F * aspect_ratio);

    // Compute projection matrices corners
    constexpr uint32 kFrustrumCornerCount = 8;
    for (uint32 i = 0; i < kShadowCascadeCount; ++i)
    {
        float z_near = z_boundaries[i];
        float z_far  = z_boundaries[i + 1];

        float x_near = z_near * tan_half_h_fov;
        float x_far  = z_far * tan_half_h_fov;

        float y_near = z_near * tan_half_v_fov;
        float y_far  = z_far * tan_half_v_fov;

        // Compute frustrum corners for cascade sub-frustrum
        std::array<math::Vec4f, kFrustrumCornerCount> frustrum_corners =
        {
            // Near plane corners
            math::Vec4f( x_near,  y_near, z_near, 1.0F), // Top Right
            math::Vec4f(-x_near,  y_near, z_near, 1.0F), // Top Left
            math::Vec4f( x_near, -y_near, z_near, 1.0F), // Bottom Right
            math::Vec4f(-x_near, -y_near, z_near, 1.0F), // Bottom Left
            // Far plane corners
            math::Vec4f( x_far,  y_far, z_far, 1.0F), // Top Right
            math::Vec4f(-x_far,  y_far, z_far, 1.0F), // Top Left
            math::Vec4f( x_far, -y_far, z_far, 1.0F), // Bottom Right
            math::Vec4f(-x_far, -y_far, z_far, 1.0F), // Bottom Left
        };

        float min = std::numeric_limits<float>::min();
        float max = std::numeric_limits<float>::max();
        float min_x = min;
        float max_x = max;
        float min_y = min;
        float max_y = max;
        float min_z = min;
        float max_z = max;

        // Compute min/max corners of the bounding box in the light view space
        for (uint32 j = 0; j < kFrustrumCornerCount; ++j)
        {
            // Convert frustrum corner into world space coordinates
            math::Vec4f world_frustrum_corner = inverse_view_transform * frustrum_corners[j];
            // Convert world frustrum corner into light view coordinates
            math::Vec4f light_view_frustrum_corner = light_view_matrix * world_frustrum_corner;

            min_x = math::Min(min_x, light_view_frustrum_corner.x_);
            max_x = math::Min(max_x, light_view_frustrum_corner.x_);
            min_y = math::Min(min_y, light_view_frustrum_corner.y_);
            max_y = math::Min(max_y, light_view_frustrum_corner.y_);
            min_z = math::Min(min_z, light_view_frustrum_corner.z_);
            max_z = math::Min(max_z, light_view_frustrum_corner.z_);
        }

        math::Matrix4x4 projection_matrix = math::Matrix4x4::Orthographic(min_x, max_x, min_y, max_y, min_z, max_z);
        projection_matrices.push_back(projection_matrix);
    }

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

