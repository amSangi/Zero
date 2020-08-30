#include "render/Optimizer.hpp"
#include "component/Camera.hpp"
#include "component/Light.hpp"
#include "component/Material.hpp"
#include "component/ModelInstance.hpp"
#include "component/PrimitiveInstance.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"
#include "render/ViewVolumeBuilder.hpp"
#include "render/IViewVolume.hpp"
#include <deque>

namespace zero::render
{

std::vector<Entity> Optimizer::ExtractRenderableEntities(const Camera& camera, const entt::registry& registry)
{
    return GetViewableEntities(camera, registry);
}

std::vector<Entity> Optimizer::ExtractPointLightShadowCasters(const Camera& camera, const entt::registry& registry)
{
    std::vector<Entity> point_light_entities;
    auto point_light_view = registry.view<const Transform, const PointLight>();
    for (Entity entity : point_light_view)
    {
        const PointLight& point_light = point_light_view.get<const PointLight>(entity);
        if (point_light.casts_shadows_)
        {
            // TODO: Perform point light culling
            point_light_entities.push_back(entity);
        }
    }
    return point_light_entities;
}

std::vector<Entity> Optimizer::ExtractDirectionalLightShadowCasters(const Camera& camera, const entt::registry& registry)
{
    std::vector<Entity> directional_light_entities;
    auto directional_light_view = registry.view<const DirectionalLight>();
    for (Entity entity : directional_light_view)
    {
        const DirectionalLight& directional_light = directional_light_view.get(entity);
        if (directional_light.casts_shadows_)
        {
            // Directional lights are not culled
            directional_light_entities.push_back(entity);
        }
    }
    return directional_light_entities;
}

std::vector<Entity> Optimizer::ExtractSpotLightShadowCasters(const Camera& camera, const entt::registry& registry)
{
    std::vector<Entity> spot_light_entities;
    auto spot_light_view = registry.view<const Transform, const SpotLight>();
    for (Entity entity : spot_light_view)
    {
        const SpotLight& spot_light = spot_light_view.get<const SpotLight>(entity);
        if (spot_light.casts_shadows_)
        {
            // TODO: Perform spot light culling
            spot_light_entities.push_back(entity);
        }
    }
    return spot_light_entities;
}

std::vector<Entity> Optimizer::GetViewableEntities(const Camera& camera, const entt::registry& registry)
{
    auto renderable_view = registry.view<const Transform, const Material, const Volume>();

    auto culler = ViewVolumeBuilder::create(camera);
    std::deque<Entity> entities_to_cull;
    std::vector<Entity> viewable_entities;

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

    // Cull entities
    while (!entities_to_cull.empty())
    {
        Entity entity = entities_to_cull.front();
        entities_to_cull.pop_front();

        // Ignore entities that do not have a mesh
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

        // Entity is renderable
        viewable_entities.push_back(entity);
    }

    return viewable_entities;
}

} // namespace zero::render