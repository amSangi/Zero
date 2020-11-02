#include "render/CullingManager.hpp"
#include "component/Camera.hpp"
#include "component/Material.hpp"
#include "component/ModelInstance.hpp"
#include "component/PrimitiveInstance.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"
#include "render/ViewVolumeBuilder.hpp"
#include "render/OrthographicViewVolume.hpp"
#include <deque>

namespace zero::render
{

std::vector<Entity> CullingManager::GetRenderableEntities(const Camera& camera, const entt::registry& registry)
{
    return GetViewableEntities(camera, registry);
}

std::vector<Entity> CullingManager::GetShadowCastingEntities(const math::Box& box, const entt::registry& registry)
{
    // Allow for larger distance between bounding box and entity volume to avoid disappearing shadows
    // when entity is not in the bounding box but the shadow is
    constexpr float cull_border_distance = 40.0F;

    OrthographicViewVolume culler{box.min_, box.max_};
    culler.SetPadding(cull_border_distance);
    return CullEntities(&culler, registry);
}

std::vector<Entity> CullingManager::GetViewableEntities(const Camera& camera, const entt::registry& registry)
{
    // Allow for small margin of error between bounding volume and entity volume to allow
    // for smoother disappearing entities
    constexpr float cull_border_distance = 5.0F;

    auto culler = ViewVolumeBuilder::create(camera);
    culler->SetPadding(cull_border_distance);
    return CullEntities(culler.get(), registry);
}

std::vector<Entity> CullingManager::CullEntities(const IViewVolume* culler, const entt::registry& registry)
{
    // Viewable entities must have Transform, Material, and Volume components
    auto renderable_view = registry.view<const Transform, const Material, const Volume>();

    std::deque<Entity> entities_to_cull{};

    // Get all root entities that are visible
    for (Entity renderable_entity : renderable_view)
    {
        const auto& transform = renderable_view.get<const Transform>(renderable_entity);
        const auto& material = renderable_view.get<const Material>(renderable_entity);
        if (!material.visible_)
        {
            continue;
        }
        if (transform.parent_ == NullEntity)
        {
            entities_to_cull.push_front(renderable_entity);
        }
    }

    std::vector<Entity> viewable_entities{};

    // Cull entities
    while (!entities_to_cull.empty())
    {
        Entity entity = entities_to_cull.front();
        entities_to_cull.pop_front();

        // Ignore entities that do not have mesh data
        if (!registry.has<ModelInstance>(entity) && !registry.has<PrimitiveInstance>(entity))
        {
            continue;
        }

        // Entity is not visible
        const auto& material = renderable_view.get<const Material>(entity);
        if (!material.visible_)
        {
            continue;
        }

        // Entity and its children are culled
        const auto& volume = renderable_view.get<const Volume>(entity);
        if (culler->IsCulled(volume.bounding_volume_))
        {
            continue;
        }

        // Cull the children
        const auto& transform = renderable_view.get<const Transform>(entity);
        entities_to_cull.insert(entities_to_cull.end(), transform.children_.begin(), transform.children_.end());

        // Entity is viewable
        viewable_entities.push_back(entity);
    }

    return viewable_entities;
}


} // namespace zero::render