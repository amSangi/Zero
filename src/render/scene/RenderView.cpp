#include "render/scene/RenderView.hpp"

namespace zero::render
{

RenderView::RenderView(const Camera& camera,
                       const SkyDome& sky_dome,
                       const TimeDelta& time_delta,
                       std::shared_ptr<CascadedShadowMap> cascaded_shadow_map,
                       std::vector<std::shared_ptr<IRenderable>>&& renderables,
                       std::array<std::vector<std::shared_ptr<IRenderable>>, Constants::kShadowCascadeCount>&& shadow_casting_renderables,
                       std::vector<DirectionalLight>&& directional_lights,
                       std::vector<std::pair<PointLight, Transform>>&& point_lights,
                       std::vector<std::pair<SpotLight, Transform>>&& spot_lights)
: camera_(camera)
, sky_dome_(sky_dome)
, time_delta_(time_delta)
, cascaded_shadow_map_(std::move(cascaded_shadow_map))
, renderables_(std::move(renderables))
, shadow_casting_renderables_(std::move(shadow_casting_renderables))
, directional_lights_(std::move(directional_lights))
, point_lights_(std::move(point_lights))
, spot_lights_(std::move(spot_lights))
{
}

RenderView::~RenderView()
{
}

const Camera& RenderView::GetCamera()
{
    return camera_;
}

const SkyDome& RenderView::GetSkyDome()
{
    return sky_dome_;
}

const TimeDelta& RenderView::GetTimeDelta()
{
    return time_delta_;
}

const CascadedShadowMap& RenderView::GetCascadedShadowMap()
{
    return *cascaded_shadow_map_;
}

const std::vector<std::shared_ptr<IRenderable>>& RenderView::GetShadowCastingRenderables(uint32 cascade_index)
{
    return shadow_casting_renderables_[cascade_index];
}

const std::vector<std::shared_ptr<IRenderable>>& RenderView::GetRenderables()
{
    return renderables_;
}

const std::vector<DirectionalLight>& RenderView::GetDirectionalLights()
{
    return directional_lights_;
}

const std::vector<std::pair<PointLight, Transform>>& RenderView::GetPointLights()
{
    return point_lights_;
}

const std::vector<std::pair<SpotLight, Transform>>& RenderView::GetSpotLights()
{
    return spot_lights_;
}


} // namespace zero::render

