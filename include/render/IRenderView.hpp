#pragma once

#include "component/Camera.hpp"
#include "component/Light.hpp"
#include "component/SkyDome.hpp"
#include "render/IRenderable.hpp"

namespace zero::render
{

    class IRenderView
    {
    public:
        virtual ~IRenderView() = default;
        virtual const std::vector<std::shared_ptr<IRenderable>>& GetRenderables() = 0;
        virtual const std::vector<Camera>& GetCameras() = 0;
        virtual const std::vector<DirectionalLight>& GetDirectionalLights() = 0;
        virtual const std::vector<PointLight>& GetPointLights() = 0;
        virtual const std::vector<SpotLight>& GetSpotLights() = 0;
        virtual const SkyDome& GetSkyDome() = 0;
    }; // interface IRenderView

} // namespace zero::render