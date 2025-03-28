#include "render/scene/CullingManager.hpp"
#include "component/Camera.hpp"
#include "component/Material.hpp"
#include "component/Mesh.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"
#include "render/scene/ViewVolumeBuilder.hpp"
#include "render/scene/OrthographicViewVolume.hpp"

namespace zero::render
{

std::vector<Entity> CullingManager::GetRenderableEntities(const Camera& camera, const entt::registry& registry)
{
	return GetViewableEntities(camera, registry);
}

std::vector<Entity> CullingManager::GetShadowCastingEntities(const math::Box& box, const entt::registry& registry)
{
	const OrthographicViewVolume culler{box.min_, box.max_};
	return CullEntities(&culler, registry);
}

std::vector<Entity> CullingManager::GetViewableEntities(const Camera& camera, const entt::registry& registry)
{
	const std::unique_ptr<IViewVolume> culler = ViewVolumeBuilder::create(camera);
	return CullEntities(culler.get(), registry);
}

std::vector<Entity> CullingManager::CullEntities(const IViewVolume* culler, const entt::registry& registry)
{
	// Viewable entities must have Transform, Material, and Volume components
	auto renderable_view = registry.view<const Transform, const Volume, const Material, const Mesh>();
	std::vector<Entity> viewable_entities{};
	// Get all root entities that are visible
	for (Entity renderable_entity : renderable_view)
	{
		const Material& material = renderable_view.get<const Material>(renderable_entity);
		const Volume& volume = renderable_view.get<const Volume>(renderable_entity);
		if (!material.visible_ || culler->IsCulled(volume.bounding_volume_))
		{
			continue;
		}
		viewable_entities.push_back(renderable_entity);
	}
	return viewable_entities;
}

} // namespace zero::render