#include "render/renderer/UniformManager.hpp"
#include "render/renderer/UniformBufferData.hpp"

namespace zero::render
{

UniformManager::UniformManager()
: camera_uniform_(nullptr)
, material_uniform_()
, model_uniform_()
, light_info_uniform_()
, directional_light_uniform_()
, point_light_uniform_()
, spot_light_uniform_()
, shadow_map_uniform_()
, shadow_texture_uniform_map_()
, diffuse_map_uniform_sampler_name_("u_diffuse_texture")
, sky_dome_center_color_uniform_name_("u_center_color")
, sky_dome_apex_color_uniform_name_("u_apex_color")
{
}

void UniformManager::Initialize(IRenderHardware* rhi)
{
    camera_uniform_ = rhi->CreateUniformBuffer("Camera", nullptr, sizeof(CameraData));
    material_uniform_ = rhi->CreateUniformBuffer("Material", nullptr, sizeof(MaterialData));
    model_uniform_ = rhi->CreateUniformBuffer("Model", nullptr, sizeof(ModelData));
    light_info_uniform_ = rhi->CreateUniformBuffer("LightInformation", nullptr, sizeof(LightInformationData));
    directional_light_uniform_ = rhi->CreateUniformBuffer("DirectionalLights", nullptr, sizeof(DirectionalLightData) * Constants::kMaxDirectionalLights);
    point_light_uniform_ = rhi->CreateUniformBuffer("PointLights", nullptr, sizeof(PointLightData) * Constants::kMaxPointLights);
    spot_light_uniform_ = rhi->CreateUniformBuffer("SpotLights", nullptr, sizeof(SpotLightData) * Constants::kMaxSpotLights);
    shadow_map_uniform_ = rhi->CreateUniformBuffer("ShadowMapInformation", nullptr, sizeof(ShadowMapInformation));

    const std::vector<std::shared_ptr<ITexture>> cascaded_shadow_map_textures = rhi->GetShadowMapTextures();
    for (uint32 cascade_index = 0; cascade_index < cascaded_shadow_map_textures.size(); ++cascade_index)
    {
        shadow_texture_uniform_map_.emplace("u_cascaded_shadow_map[" + std::to_string(cascade_index) + "]", cascaded_shadow_map_textures[cascade_index]);
    }
}

std::shared_ptr<IUniformBuffer> UniformManager::GetCameraUniform() const
{
    return camera_uniform_;
}

std::shared_ptr<IUniformBuffer> UniformManager::GetMaterialUniform() const
{
    return material_uniform_;
}

std::shared_ptr<IUniformBuffer> UniformManager::GetModelUniform() const
{
    return model_uniform_;
}

std::shared_ptr<IUniformBuffer> UniformManager::GetLightInformationUniform() const
{
    return light_info_uniform_;
}

std::shared_ptr<IUniformBuffer> UniformManager::GetDirectionalLightUniform() const
{
    return directional_light_uniform_;
}

std::shared_ptr<IUniformBuffer> UniformManager::GetPointLightUniform() const
{
    return point_light_uniform_;
}

std::shared_ptr<IUniformBuffer> UniformManager::GetSpotLightUniform() const
{
    return spot_light_uniform_;
}

std::shared_ptr<IUniformBuffer> UniformManager::GetShadowMapUniform() const
{
    return shadow_map_uniform_;
}

const std::unordered_map<std::string, std::shared_ptr<ITexture>>& UniformManager::GetShadowTextureUniformMap() const
{
    return shadow_texture_uniform_map_;
}

const std::string& UniformManager::GetDiffuseMapUniformSamplerName() const
{
    return diffuse_map_uniform_sampler_name_;
}

const std::string& UniformManager::GetSkyDomeApexColorUniformName() const
{
    return sky_dome_apex_color_uniform_name_;
}

const std::string& UniformManager::GetSkyDomeCenterColorUniformName() const
{
    return sky_dome_center_color_uniform_name_;
}

} // namespace zero::render