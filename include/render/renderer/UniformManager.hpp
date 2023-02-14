#pragma once

#include <unordered_map>
#include "render/renderer/ITexture.hpp"
#include "render/renderer/IRenderHardware.hpp"
#include "render/renderer/IUniformBuffer.hpp"

namespace zero::render
{

    class UniformManager
    {
    public:
        UniformManager();
        ~UniformManager() = default;
        void Initialize(IRenderHardware* rhi);
        std::shared_ptr<IUniformBuffer> GetCameraUniform() const;
        std::shared_ptr<IUniformBuffer> GetMaterialUniform() const;
        std::shared_ptr<IUniformBuffer> GetModelUniform() const;
        std::shared_ptr<IUniformBuffer> GetLightInformationUniform() const;
        std::shared_ptr<IUniformBuffer> GetDirectionalLightUniform() const;
        std::shared_ptr<IUniformBuffer> GetPointLightUniform() const;
        std::shared_ptr<IUniformBuffer> GetSpotLightUniform() const;
        std::shared_ptr<IUniformBuffer> GetShadowMapUniform() const;
        const std::unordered_map<std::string, std::shared_ptr<ITexture>>& GetShadowTextureUniformMap() const;
        const std::string& GetDiffuseMapUniformSamplerName() const;
        const std::string& GetSkyDomeApexColorUniformName() const;
        const std::string& GetSkyDomeCenterColorUniformName() const;
    private:
        std::shared_ptr<IUniformBuffer> camera_uniform_;
        std::shared_ptr<IUniformBuffer> material_uniform_;
        std::shared_ptr<IUniformBuffer> model_uniform_;
        std::shared_ptr<IUniformBuffer> light_info_uniform_;
        std::shared_ptr<IUniformBuffer> directional_light_uniform_;
        std::shared_ptr<IUniformBuffer> point_light_uniform_;
        std::shared_ptr<IUniformBuffer> spot_light_uniform_;
        std::shared_ptr<IUniformBuffer> shadow_map_uniform_;
        std::unordered_map<std::string, std::shared_ptr<ITexture>> shadow_texture_uniform_map_;
        std::string diffuse_map_uniform_sampler_name_;
        std::string sky_dome_apex_color_uniform_name_;
        std::string sky_dome_center_color_uniform_name_;

    };

} // namespace zero::render