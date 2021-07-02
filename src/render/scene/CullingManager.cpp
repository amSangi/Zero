#include "render/scene/CullingManager.hpp"
#include "component/Camera.hpp"
#include "component/Material.hpp"
#include "component/ModelInstance.hpp"
#include "component/PrimitiveInstance.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"
#include "render/scene/ViewVolumeBuilder.hpp"
#include "render/scene/OrthographicViewVolume.hpp"
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
    auto transform_volume_view  = registry.view<const Transform, const Volume>();
    auto material_view          = registry.view<const Material>();
    auto model_view             = registry.view<const ModelInstance>();
    auto primitive_view         = registry.view<const PrimitiveInstance>();

    std::deque<Entity> entities_to_cull{};

    // Get all root entities that are visible
    for (Entity renderable_entity : transform_volume_view)
    {
        const Transform& transform = transform_volume_view.get<const Transform>(renderable_entity);
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

        // Entity is not visible
        if (material_view.contains(entity))
        {
            const Material& material = material_view.get<const Material>(entity);
            if (!material.visible_)
            {
                continue;
            }
        }

        // Entity and its children are culled
        const Volume& volume = transform_volume_view.get<const Volume>(entity);
        if (culler->IsCulled(volume.bounding_volume_))
        {
            continue;
        }

        // Cull the children
        const Transform& transform = transform_volume_view.get<const Transform>(entity);
        entities_to_cull.insert(entities_to_cull.end(), transform.children_.begin(), transform.children_.end());

        // Only entities with a Transform, Volume, Material and either PrimitiveInstance or ModelInstance are viewable
        if (material_view.contains(entity) && (model_view.contains(entity) || primitive_view.contains(entity)))
        {
            viewable_entities.push_back(entity);
        }
    }

    return viewable_entities;
}


} // namespace zero::render