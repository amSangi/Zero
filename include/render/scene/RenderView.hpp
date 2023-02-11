#pragma once

#include "render/CascadedShadowMap.hpp"
#include "render/Constants.hpp"
#include "render/IRenderView.hpp"

namespace zero::render
{

    class RenderView : public IRenderView
    {
    public:
        RenderView(const Camera& camera,
                   const SkyDome& sky_dome,
                   const TimeDelta& time_delta,
                   std::shared_ptr<CascadedShadowMap> cascaded_shadow_map,
                   std::vector<Entity>&& renderable_entities,
                   std::array<std::vector<Entity>, Constants::kShadowCascadeCount>&& shadow_casting_entities,
                   std::vector<DirectionalLight>&& directional_lights,
                   std::vector<std::pair<PointLight, Transform>>&& point_lights,
                   std::vector<std::pair<SpotLight, Transform>>&& spot_lights);
        ~RenderView() override = default;

        const Camera& GetCamera() override;
        const SkyDome& GetSkyDome() override;
        const TimeDelta& GetTimeDelta() override;

        const CascadedShadowMap& GetCascadedShadowMap() override;

        const std::vector<Entity>& GetRenderableEntities() override;
        const std::vector<Entity>& GetShadowCastingEntities(uint32 cascade_index) override;

        const std::vector<DirectionalLight>& GetDirectionalLights() override;
        const std::vector<std::pair<PointLight, Transform>>& GetPointLights() override;
        const std::vector<std::pair<SpotLight, Transform>>& GetSpotLights() override;

    private:
        Camera camera_;
        SkyDome sky_dome_;
        TimeDelta time_delta_;
        std::shared_ptr<CascadedShadowMap> cascaded_shadow_map_;
        std::vector<Entity> renderable_entities_;
        std::array<std::vector<Entity>, Constants::kShadowCascadeCount> shadow_casting_entities_;
        std::vector<DirectionalLight> directional_lights_;
        std::vector<std::pair<PointLight, Transform>> point_lights_;
        std::vector<std::pair<SpotLight, Transform>> spot_lights_;

    }; // class RenderView

} // namespace zero