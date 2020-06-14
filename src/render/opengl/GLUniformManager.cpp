#include "render/opengl/GLUniformManager.hpp"
#include "render/opengl/GLTexture.hpp"
#include "components/Light.hpp"
#include "components/Material.hpp"
#include "components/Transform.hpp"

namespace zero::render
{

uint32 GLUniformManager::kMaxDirectionalLights = 8U;
uint32 GLUniformManager::kMaxPointLights = 8U;
uint32 GLUniformManager::kMaxSpotLights = 8U;

GLUniformManager::GLUniformManager()
: camera_buffer_id_(0)
, model_buffer_id_(0)
, light_buffer_id_(0)
, material_id_(0)
{
}

GLUniformManager::~GLUniformManager()
{
    Cleanup();
}

void GLUniformManager::Cleanup()
{
    glDeleteBuffers(1, &camera_buffer_id_);
    glDeleteBuffers(1, &model_buffer_id_);
    glDeleteBuffers(1, &light_buffer_id_);
    glDeleteBuffers(1, &material_id_);
}

void GLUniformManager::Initialize()
{
    glGenBuffers(1, &camera_buffer_id_);
    glGenBuffers(1, &model_buffer_id_);
    glGenBuffers(1, &light_buffer_id_);
    glGenBuffers(1, &material_id_);

    uint32 binding_index = 0;

    // region Initialize camera uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, camera_buffer_id_);
    // Projection Matrix (4x4) + View Matrix (4x4) + Camera Position (Vec4f)
    uint64 buffer_size = (2 * sizeof(math::Matrix4x4)) + sizeof(math::Vec4f);
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, binding_index, camera_buffer_id_, 0, buffer_size);
    // endregion

    // region Initialize model uniform buffer
    binding_index += 1;
    glBindBuffer(GL_UNIFORM_BUFFER, model_buffer_id_);
    // Model Matrix (4x4) + Normal Matrix (4x4)
    buffer_size = (2 * sizeof(math::Matrix4x4));
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, binding_index, model_buffer_id_, 0, buffer_size);
    // endregion

    // region Initialize light uniform buffer
    binding_index += 1;
    glBindBuffer(GL_UNIFORM_BUFFER, light_buffer_id_);

    // color (math::Vec4f) + direction (math::Vec4f) + intensity (float)
    uint64 directional_light_size = (2 * sizeof(math::Vec4f)) + sizeof(float);
    uint64 total_directional_light_size =  directional_light_size * GetMaxDirectionalLightCount();

    // color (math::Vec4f) + position (math::Vec4f) + attenuation constant/linear/quadratic values (3 * float)
    uint64 point_light_size = (2 * sizeof(math::Vec4f)) + (3 * sizeof(float));
    uint64 total_point_light_size = point_light_size * GetMaxPointLightCount();

    // color (math::Vec4f) + position (math::Vec4f) + direction (math::Vec4f) + inner/outer cosines (float)
    uint64 spot_light_size = (3 * sizeof(math::Vec4f)) + (2 * sizeof(float));
    uint64 total_spot_light_size = spot_light_size * GetMaxSpotLightCount();

    // 3 integers for the light counts in the scene + total light data size
    buffer_size = (3 * sizeof(int32)) + total_directional_light_size + total_point_light_size + total_spot_light_size;
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, binding_index, light_buffer_id_, 0, buffer_size);
    // endregion

    // region Initialize material uniform buffer
    binding_index += 1;
    glBindBuffer(GL_UNIFORM_BUFFER, material_id_);
    // diffuse color (math::Vec4f) + specular exponent (float)
    buffer_size = sizeof(math::Vec4f) + sizeof(float);
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, binding_index, material_id_, 0, buffer_size);
    // endregion

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateCameraUniforms(const math::Matrix4x4& projection_matrix,
                                            const math::Matrix4x4& view_matrix,
                                            const math::Vec3f& camera_position) const
{
    // Transpose matrices to switch from row-major to column-major order
    math::Matrix4x4 transpose_projection_matrix = projection_matrix.Transpose();
    math::Matrix4x4 transpose_view_matrix = view_matrix.Transpose();

    glBindBuffer(GL_UNIFORM_BUFFER, camera_buffer_id_);

    // Transpose Projection Matrix (math::Matrix4x4)
    uint64 offset = 0;
    uint64 size = sizeof(transpose_projection_matrix);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &transpose_projection_matrix[0][0]);

    // Transpose View Matrix (math::Matrix4x4)
    offset += size;
    size = sizeof(transpose_view_matrix);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &transpose_view_matrix[0][0]);

    // Camera Position (math::Vec4f)
    math::Vec4f position = math::Vec4f(camera_position.x_, camera_position.y_, camera_position.z_, 0.0F);
    offset += size;
    size = sizeof(position);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, position.Data());

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateModelUniforms(const math::Matrix4x4& model_matrix,
                                           const math::Matrix4x4& normal_matrix) const
{
    // Transpose matrices to switch from row-major to column-major order
    math::Matrix4x4 transpose_model_matrix = model_matrix.Transpose();
    math::Matrix4x4 transpose_normal_matrix = normal_matrix.Transpose();

    glBindBuffer(GL_UNIFORM_BUFFER, model_buffer_id_);

    // Transpose Model Matrix (math::Matrix4x4)
    uint64 offset = 0;
    uint64 size = sizeof(transpose_model_matrix);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &transpose_model_matrix[0][0]);

    // Transpose Normal Matrix (math::Matrix4x4)
    offset += size;
    size = sizeof(transpose_normal_matrix);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &transpose_normal_matrix[0][0]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateLightUniforms(const entt::registry& registry) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, light_buffer_id_);

    auto directional_light_view = registry.view<const DirectionalLight>();
    auto point_light_view = registry.view<const Transform, const PointLight>();
    auto spot_light_view = registry.view<const Transform, const SpotLight>();

    auto directional_light_count = static_cast<int32>(math::Min(kMaxDirectionalLights, directional_light_view.size()));
    auto point_light_count = static_cast<int32>(math::Min(kMaxPointLights, point_light_view.size()));
    auto spot_light_count = static_cast<int32>(math::Min(kMaxSpotLights, spot_light_view.size()));

    uint64 offset = 0;
    uint64 size = sizeof(int32);

    // Light counts
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &directional_light_count);

    offset += size;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &point_light_count);

    offset += size;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &spot_light_count);

    uint32 i = 0;

    // Directional Lights
    for (Entity entity : directional_light_view)
    {
        if (i >= kMaxDirectionalLights)
        {
            break;
        }

        const auto& directional_light = directional_light_view.get(entity);

        math::Vec4f color = math::Vec4f(directional_light.color_.x_,
                                        directional_light.color_.y_,
                                        directional_light.color_.z_,
                                        1.0F);
        offset += size;
        size = sizeof(color);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, color.Data());

        math::Vec4f direction = math::Vec4f(directional_light.direction_.x_,
                                            directional_light.direction_.y_,
                                            directional_light.direction_.z_,
                                            0.0F);
        offset += size;
        size = sizeof(direction);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, direction.Data());

        offset += size;
        size = sizeof(directional_light.intensity_);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &directional_light.intensity_);

        ++i;
    }

    // Point Lights
    for (Entity entity: point_light_view)
    {
        if (i >= kMaxPointLights)
        {
            break;
        }

        const auto& transform = point_light_view.get<const Transform>(entity);
        const auto& point_light = point_light_view.get<const PointLight>(entity);

        math::Vec4f color = math::Vec4f(point_light.color_.x_,
                                        point_light.color_.y_,
                                        point_light.color_.z_,
                                        1.0F);
        offset += size;
        size = sizeof(color);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, color.Data());


        const math::Vec3f& position = transform.GetPosition();
        math::Vec4f position_4D = math::Vec4f(position.x_,
                                              position.y_,
                                              position.z_,
                                              0.0F);
        offset += size;
        size = sizeof(position_4D);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, position_4D.Data());

        offset += size;
        size = sizeof(point_light.attenuation_constant_);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &point_light.attenuation_constant_);

        offset += size;
        size = sizeof(point_light.attenuation_linear_);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &point_light.attenuation_linear_);

        offset += size;
        size = sizeof(point_light.attenuation_quadratic_);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &point_light.attenuation_quadratic_);

        ++i;
    }

    // Spot Lights
    for (Entity entity: spot_light_view)
    {
        if (i >= kMaxSpotLights)
        {
            break;
        }
        const auto& transform = spot_light_view.get<const Transform>(entity);
        const auto& spot_light = spot_light_view.get<const SpotLight>(entity);

        // Color
        math::Vec4f color = math::Vec4f(spot_light.color_.x_,
                                        spot_light.color_.y_,
                                        spot_light.color_.z_,
                                        1.0F);
        offset += size;
        size = sizeof(color);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, color.Data());

        // Position
        const math::Vec3f& position = transform.GetPosition();
        math::Vec4f position_4D = math::Vec4f(position.x_,
                                              position.y_,
                                              position.z_,
                                              0.0F);
        offset += size;
        size = sizeof(position_4D);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, position_4D.Data());

        // Direction
        const math::Vec3f direction = transform.GetOrientation() * spot_light.direction_;
        math::Vec4f direction_4D = math::Vec4f(direction.x_,
                                               direction.y_,
                                               direction.z_,
                                               0.0F);
        offset += size;
        size = sizeof(direction_4D);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, direction_4D.Data());

        // Inner Cosine
        const float inner_cosine = math::Cos(spot_light.inner_cone_angle_.ToRadian().rad_);
        offset += size;
        size = sizeof(inner_cosine);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &inner_cosine);

        // Outer Cosine
        const float outer_cosine = math::Cos(spot_light.outer_cone_angle_.ToRadian().rad_);
        offset += size;
        size = sizeof(outer_cosine);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &outer_cosine);

        ++i;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateMaterialUniforms(const Material& material) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, material_id_);

    math::Vec4f color = math::Vec4f(material.diffuse_color_.x_,
                                    material.diffuse_color_.y_,
                                    material.diffuse_color_.z_,
                                    1.0F);
    uint64 offset = 0;
    uint64 size = sizeof(color);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, color.Data());

    offset += size;
    size = sizeof(material.specular_exponent_);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &material.specular_exponent_);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateSamplerUniforms(const std::shared_ptr<GLProgram>& program,
                                             const std::vector<std::shared_ptr<GLTexture>>& textures)
{
    for (int i = 0; i < textures.size(); ++i)
    {
        const auto& gl_texture = textures[i];
        GLenum texture_unit = GL_TEXTURE0 + i;
        gl_texture->GenerateMipMap(texture_unit);
        gl_texture->Bind(texture_unit);
        program->SetUniform("texture_map." + gl_texture->GetName(), i);
    }
}

void GLUniformManager::BindGraphicsProgram(const std::shared_ptr<GLProgram>& program)
{
    GLuint camera_index = program->GetUniformBlockIndex("Camera");
    GLuint model_index = program->GetUniformBlockIndex("Model");
    GLuint light_index = program->GetUniformBlockIndex("Lights");
    GLuint material_index = program->GetUniformBlockIndex("Material");

    program->BindBlockIndex(camera_index, 0U);
    program->BindBlockIndex(model_index, 1U);
    program->BindBlockIndex(light_index, 2U);
    program->BindBlockIndex(material_index, 3U);
}

uint32 GLUniformManager::GetMaxDirectionalLightCount()
{
    return kMaxDirectionalLights;
}

uint32 GLUniformManager::GetMaxPointLightCount()
{
    return kMaxPointLights;
}

uint32 GLUniformManager::GetMaxSpotLightCount()
{
    return kMaxSpotLights;
}

} // namespace zero::render