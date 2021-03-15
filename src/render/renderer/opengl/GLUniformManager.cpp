#include "render/renderer/opengl/GLUniformManager.hpp"

namespace zero::render
{

//////////////////////////////////////////////////
///// Binding Indices
//////////////////////////////////////////////////
constexpr uint32 kCameraBufferBindingIndex           = 0U;
constexpr uint32 kMaterialBufferBindingIndex         = 1U;
constexpr uint32 kModelBufferBindingIndex            = 2U;
constexpr uint32 kShadowMapBufferBindingIndex        = 3U;

constexpr uint32 kLightInfoBufferBindingIndex        = 4U;
constexpr uint32 kDirectionalLightBufferBindingIndex = 5U;
constexpr uint32 kPointLightBufferBindingIndex       = 6U;
constexpr uint32 kSpotLightBufferBindingIndex        = 7U;

//////////////////////////////////////////////////
///// GLUniformManager
//////////////////////////////////////////////////
GLUniformManager::GLUniformManager()
: camera_buffer_(nullptr)
, material_buffer_(nullptr)
, model_buffer_(nullptr)
, shadow_map_buffer_(nullptr)
, light_info_buffer_(nullptr)
, directional_light_buffer_(nullptr)
, point_light_buffer_(nullptr)
, spot_light_buffer_(nullptr)
{
}

GLUniformManager::~GLUniformManager()
{
}

void GLUniformManager::Initialize()
{
    camera_buffer_ = std::make_unique<GLCameraUniformBuffer>();
    material_buffer_ = std::make_unique<GLMaterialUniformBuffer>();
    model_buffer_ = std::make_unique<GLModelUniformBuffer>();
    shadow_map_buffer_ = std::make_unique<GLShadowMapUniformBuffer>();
    light_info_buffer_ = std::make_unique<GLLightInfoUniformBuffer>();
    directional_light_buffer_ = std::make_unique<GLDirectionalLightUniformBuffer>();
    point_light_buffer_ = std::make_unique<GLPointLightUniformBuffer>();
    spot_light_buffer_ = std::make_unique<GLSpotLightUniformBuffer>();

    camera_buffer_->Initialize(kCameraBufferBindingIndex);
    material_buffer_->Initialize(kMaterialBufferBindingIndex);
    model_buffer_->Initialize(kModelBufferBindingIndex);
    shadow_map_buffer_->Initialize(kShadowMapBufferBindingIndex);
    light_info_buffer_->Initialize(kLightInfoBufferBindingIndex);
    directional_light_buffer_->Initialize(kDirectionalLightBufferBindingIndex);
    point_light_buffer_->Initialize(kPointLightBufferBindingIndex);
    spot_light_buffer_->Initialize(kSpotLightBufferBindingIndex);
}

void GLUniformManager::SetShadowSamplerName(IProgram* shader_program, uint32 texture_index)
{
    std::string uniform_sampler_name = "u_cascaded_shadow_map[" + std::to_string(texture_index) + "]";
    shader_program->SetUniform(uniform_sampler_name, static_cast<float>(texture_index));
}

void GLUniformManager::SetDiffuseSamplerName(IProgram* shader_program, uint32 texture_index)
{
    shader_program->SetUniform("u_diffuse_texture", static_cast<float>(texture_index));
}

void GLUniformManager::SetSkyDomeUniforms(IProgram* sky_dome_program, const Camera& camera, const SkyDome& sky_dome)
{
    constexpr float kSkyDomeSphereScale = 1000.0F;

    // Sphere scaled and centered around the camera
    math::Matrix4x4 model_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(kSkyDomeSphereScale))
            .Translate(camera.position_);

    sky_dome_program->SetUniform("u_projection_matrix", camera.GetProjectionMatrix());
    sky_dome_program->SetUniform("u_view_matrix", camera.GetViewMatrix());
    sky_dome_program->SetUniform("u_model_matrix", model_matrix);
    sky_dome_program->SetUniform("u_camera_position", camera.position_);
    sky_dome_program->SetUniform("u_apex_color", sky_dome.apex_color_);
    sky_dome_program->SetUniform("u_center_color", sky_dome.apex_color_);
}

IUniformBuffer* GLUniformManager::GetUniformBuffer(IUniformManager::UniformBufferType buffer_type)
{
    switch (buffer_type)
    {
        case IUniformManager::UniformBufferType::CAMERA_BUFFER:
            return camera_buffer_.get();
        case IUniformManager::UniformBufferType::LIGHT_INFO_BUFFER:
            return light_info_buffer_.get();
        case IUniformManager::UniformBufferType::DIRECTIONAL_LIGHT_BUFFER:
            return directional_light_buffer_.get();
        case IUniformManager::UniformBufferType::POINT_LIGHT_BUFFER:
            return point_light_buffer_.get();
        case IUniformManager::UniformBufferType::SPOT_LIGHT_BUFFER:
            return spot_light_buffer_.get();
        case IUniformManager::UniformBufferType::MATERIAL_BUFFER:
            return material_buffer_.get();
        case IUniformManager::UniformBufferType::MODEL_BUFFER:
            return model_buffer_.get();
        case IUniformManager::UniformBufferType::SHADOW_MAP_BUFFER:
            return shadow_map_buffer_.get();
        default:
            return nullptr;
    }
}

//////////////////////////////////////////////////
///// Update Uniforms
//////////////////////////////////////////////////
void GLUniformManager::UpdateCameraUniforms(const math::Matrix4x4& projection_matrix,
                                            const math::Matrix4x4& view_matrix,
                                            const math::Vec3f& camera_position)
{
    camera_buffer_->UpdateUniforms(projection_matrix, view_matrix, camera_position);
}

void GLUniformManager::UpdateLightUniforms(const std::vector<DirectionalLight>& directional_lights,
                                           const std::vector<Transform>& point_light_transforms,
                                           const std::vector<PointLight>& point_lights,
                                           const std::vector<Transform>& spot_light_transforms,
                                           const std::vector<SpotLight>& spot_lights)
{
    light_info_buffer_->UpdateUniforms(directional_lights.size(), point_lights.size(), spot_lights.size());
    directional_light_buffer_->UpdateUniforms(directional_lights);
    point_light_buffer_->UpdateUniforms(point_light_transforms, point_lights);
    spot_light_buffer_->UpdateUniforms(spot_light_transforms, spot_lights);
}

void GLUniformManager::UpdateMaterialUniforms(const Material& material)
{
    material_buffer_->UpdateUniforms(material);
}

void GLUniformManager::UpdateModelUniforms(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix)
{
    model_buffer_->UpdateUniforms(model_matrix, normal_matrix);
}

void GLUniformManager::UpdateShadowMapUniforms(const std::vector<math::Matrix4x4>& light_matrices,
                                               const std::vector<float>& cascade_end_clip_spaces)
{
    shadow_map_buffer_->UpdateUniforms(light_matrices, cascade_end_clip_spaces);
}

} // namespace zero::render