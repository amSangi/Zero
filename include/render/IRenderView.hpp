#pragma once

#include "component/Camera.hpp"
#include "component/Light.hpp"
#include "component/Transform.hpp"
#include "component/SkyDome.hpp"
#include "core/TimeDelta.hpp"
#include "render/CascadedShadowMap.hpp"

namespace zero::render
{

    class IRenderView
    {
    public:
        virtual ~IRenderView() = default;

        virtual const Camera& GetCamera() = 0;
        virtual const SkyDome& GetSkyDome() = 0;
        virtual const TimeDelta& GetTimeDelta() = 0;

        virtual const CascadedShadowMap& GetCascadedShadowMap() = 0;

        virtual const std::vector<Entity>& GetRenderableEntities() = 0;
        virtual const std::vector<Entity>& GetShadowCastingEntities(uint32 cascade_index) = 0;

        virtual const std::vector<DirectionalLight>& GetDirectionalLights() = 0;
        virtual const std::vector<std::pair<PointLight, Transform>>& GetPointLights() = 0;
        virtual const std::vector<std::pair<SpotLight, Transform>>& GetSpotLights() = 0;
    }; // interface IRenderView

} // namespace zero::render