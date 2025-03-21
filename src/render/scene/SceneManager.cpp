#include "render/scene/SceneManager.hpp"
#include "render/scene/RenderView.hpp"
#include "render/scene/CullingManager.hpp"
#include "render/scene/RenderView.hpp"
#include "render/CascadedShadowMap.hpp"
#include "component/Material.hpp"
#include "component/Mesh.hpp"
#include "component/Volume.hpp"
#include <queue>

namespace zero::render
{

SceneManager::SceneManager()
: render_view_(nullptr)
, cascaded_shadow_map_(std::make_unique<CascadedShadowMap>(Constants::kShadowCascadeCount))
, renderable_cache_()
{
}

void SceneManager::UpdateView(entt::registry& registry, const TimeDelta& time_delta)
{
	const Camera& camera = GetPrimaryCamera(registry);
	std::vector<DirectionalLight> directional_lights = GetDirectionalLights(registry);
	for (const DirectionalLight& directional_light : directional_lights)
	{
		// Update the cascaded shadow map with the first directional light that casts shadows
		if (directional_light.casts_shadows_)
		{
			cascaded_shadow_map_->Update(camera, directional_light);
			break;
		}
	}

	render_view_ = std::make_unique<RenderView>(camera,
	                                            GetSkyDome(registry),
	                                            time_delta,
	                                            cascaded_shadow_map_,
	                                            GetRenderableEntities(camera, registry),
	                                            GetShadowCastingEntities(registry),
	                                            std::move(directional_lights),
	                                            GetPointLights(registry),
	                                            GetSpotLights(registry));
	// Cache is no longer needed
	renderable_cache_.clear();
}

std::unique_ptr<RenderView> SceneManager::GetLatestView()
{
	return std::move(render_view_);
}

const Camera& SceneManager::GetPrimaryCamera(const entt::registry& registry)
{
	// Assume camera view contains at least one Camera
	auto camera_view = registry.view<const Camera>();
	return camera_view.get<const Camera>(camera_view.front());
}

SkyDome SceneManager::GetSkyDome(const entt::registry& registry)
{
	auto sky_dome_view = registry.view<const SkyDome>();

	SkyDome inactive_sky_dome{};
	inactive_sky_dome.is_active_ = false;

	for (Entity sky_dome_entity : sky_dome_view)
	{
		// Get the first active sky dome
		const SkyDome& sky_dome = sky_dome_view.get<const SkyDome>(sky_dome_entity);
		if (sky_dome.is_active_)
		{
			return sky_dome;
		}
	}

	return inactive_sky_dome;
}

std::vector<DirectionalLight> SceneManager::GetDirectionalLights(const entt::registry& registry)
{
	std::vector<DirectionalLight> directional_lights;

	auto directional_light_view = registry.view<const DirectionalLight>();
	directional_lights.reserve(directional_light_view.size());

	for (Entity entity : directional_light_view)
	{
		const DirectionalLight& directional_light = directional_light_view.get<const DirectionalLight>(entity);
		directional_lights.push_back(directional_light);
	}

	if (directional_lights.size() > Constants::kMaxDirectionalLights)
	{
		directional_lights.resize(Constants::kMaxDirectionalLights);
	}

	return directional_lights;
}

std::vector<std::pair<PointLight, Transform>> SceneManager::GetPointLights(const entt::registry& registry)
{
	std::vector<std::pair<PointLight, Transform>> point_lights;

	auto point_light_view = registry.view<const PointLight, const Transform>();
	point_lights.reserve(point_light_view.size_hint());

	for (Entity entity : point_light_view)
	{
		const PointLight& point_light = point_light_view.get<const PointLight>(entity);
		const Transform& transform = point_light_view.get<const Transform>(entity);
		point_lights.emplace_back(point_light, transform);
	}

	if (point_lights.size() > Constants::kMaxPointLights)
	{
		point_lights.resize(Constants::kMaxPointLights);
	}

	return point_lights;
}

std::vector<std::pair<SpotLight, Transform>> SceneManager::GetSpotLights(const entt::registry& registry)
{
	std::vector<std::pair<SpotLight, Transform>> spot_lights;

	auto spot_light_view = registry.view<const SpotLight, const Transform>();
	spot_lights.reserve(spot_light_view.size_hint());

	for (Entity entity : spot_light_view)
	{
		const SpotLight& spot_light = spot_light_view.get<const SpotLight>(entity);
		const Transform& transform = spot_light_view.get<const Transform>(entity);
		spot_lights.emplace_back(spot_light, transform);
	}

	if (spot_lights.size() > Constants::kMaxSpotLights)
	{
		spot_lights.resize(Constants::kMaxSpotLights);
	}

	return spot_lights;
}

std::vector<Entity> SceneManager::GetRenderableEntities(const Camera& camera, const entt::registry& registry)
{
	return CullingManager::GetRenderableEntities(camera, registry);
}

std::array<std::vector<Entity>, Constants::kShadowCascadeCount> SceneManager::GetShadowCastingEntities(const entt::registry& registry)
{

	std::array<std::vector<Entity>, Constants::kShadowCascadeCount> shadow_casting_renderables;
	const std::vector<math::Box> world_bounding_boxes = cascaded_shadow_map_->GetWorldBoundingBoxes();
	const math::Vec3f light_direction = cascaded_shadow_map_->GetLightDirection();
	for (uint32 cascade_index = 0; cascade_index < Constants::kShadowCascadeCount; ++cascade_index)
	{
		// The amount the shadow map's bounding box is increased to avoid culling shadow casting entities
		constexpr float bounding_box_expansion_factor = 2.0F;

		math::Box world_bounding_box = world_bounding_boxes[cascade_index];

		// Avoid culling shadow casting entities that are just outside the world bounding box of the cascaded shadow map
		// by increasing the size of the box in the opposite direction of the light.
		world_bounding_box.min_ -= light_direction * bounding_box_expansion_factor;

		// Cull shadow casting renderables for the world bounding box
		shadow_casting_renderables[cascade_index] = CullingManager::GetShadowCastingEntities(world_bounding_box, registry);
	}

	return shadow_casting_renderables;
}

} // namespace zero::render

