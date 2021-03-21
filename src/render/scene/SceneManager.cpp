#include "render/scene/SceneManager.hpp"
#include "render/scene/Renderable.hpp"
#include "render/scene/RenderView.hpp"
#include "render/scene/CullingManager.hpp"

namespace zero::render
{

SceneManager::SceneManager()
: render_view_(nullptr)
, cascaded_shadow_map_(std::make_unique<CascadedShadowMap>(Constants::kShadowCascadeCount))
, renderable_cache_()
{
}

SceneManager::~SceneManager()
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
                                                GetRenderables(camera, registry),
                                                GetShadowCastingRenderables(registry),
                                                std::move(directional_lights),
                                                GetPointLights(registry),
                                                GetSpotLights(registry));
    // Cache is no longer needed
    renderable_cache_.clear();
}

std::unique_ptr<IRenderView> SceneManager::GetLatestView()
{
    return std::move(render_view_);
}

const Camera& SceneManager::GetPrimaryCamera(const entt::registry& registry) const
{
    // Assume camera view contains at least one Camera
    auto camera_view = registry.view<const Camera>();
    return camera_view.get<const Camera>(camera_view.front());
}

SkyDome SceneManager::GetSkyDome(const entt::registry& registry) const
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

std::vector<DirectionalLight> SceneManager::GetDirectionalLights(const entt::registry& registry) const
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

std::vector<std::pair<PointLight, Transform>> SceneManager::GetPointLights(const entt::registry& registry) const
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

std::vector<std::pair<SpotLight, Transform>> SceneManager::GetSpotLights(const entt::registry& registry) const
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

std::vector<std::shared_ptr<IRenderable>> SceneManager::GetRenderables(const Camera& camera, const entt::registry& registry)
{
    std::vector<Entity> viewable_entities = CullingManager::GetRenderableEntities(camera, registry);

    // Construct views
    auto renderable_view              = registry.view<const Transform, const Material, const Volume>();
    auto model_instance_view          = registry.view<const ModelInstance>();
    auto animated_model_instance_view = registry.view<const Animator>();
    auto primitive_instance_view      = registry.view<const PrimitiveInstance>();

    std::vector<std::shared_ptr<IRenderable>> renderables{};
    renderables.reserve(viewable_entities.size());

    // Construct a renderable that takes a snapshot of entities and their components
    for (const Entity entity : renderable_view)
    {
        const Transform& transform = renderable_view.get<const Transform>(entity);
        const Material& material = renderable_view.get<const Material>(entity);
        const Volume& volume = renderable_view.get<const Volume>(entity);
        std::unique_ptr<Animator> animator{};
        std::unique_ptr<ModelInstance> model_instance{};
        std::unique_ptr<PrimitiveInstance> primitive_instance{};
        if (model_instance_view.contains(entity))
        {
            model_instance = std::make_unique<ModelInstance>(model_instance_view.get<const ModelInstance>(entity));
            if (animated_model_instance_view.contains(entity))
            {
                animator = std::make_unique<Animator>(animated_model_instance_view.get<const Animator>(entity));
            }
        }
        else
        {
            primitive_instance = std::make_unique<PrimitiveInstance>(primitive_instance_view.get<const PrimitiveInstance>(entity));
        }

        std::shared_ptr<IRenderable> renderable = std::make_shared<Renderable>(material,
                                                                               transform,
                                                                               volume,
                                                                               std::move(animator),
                                                                               std::move(model_instance),
                                                                               std::move(primitive_instance));
        renderable_cache_.emplace(entity, renderable);
        renderables.push_back(renderable);
    }

    return renderables;
}

std::array<std::vector<std::shared_ptr<IRenderable>>, Constants::kShadowCascadeCount> SceneManager::GetShadowCastingRenderables(const entt::registry& registry)
{
    std::array<std::vector<std::shared_ptr<IRenderable>>, Constants::kShadowCascadeCount> shadow_casting_renderables;

    // Construct views in case we need to construct a renderable not in the cache
    auto renderable_view              = registry.view<const Transform, const Material, const Volume>();
    auto model_instance_view          = registry.view<const ModelInstance>();
    auto animated_model_instance_view = registry.view<const Animator>();
    auto primitive_instance_view      = registry.view<const PrimitiveInstance>();

    std::vector<math::Box> world_bounding_boxes = cascaded_shadow_map_->GetWorldBoundingBoxes();
    for (uint32 cascade_index = 0; cascade_index < Constants::kShadowCascadeCount; ++cascade_index)
    {
        const math::Box& world_bounding_box = world_bounding_boxes[cascade_index];

        // Cull shadow casting renderables for the world bounding box
        std::vector<Entity> viewable_entities = CullingManager::GetShadowCastingEntities(world_bounding_box, registry);

        // Fill renderable container at the cascade index
        for (Entity entity : viewable_entities)
        {
            // Check cache for renderable
            auto renderable_search = renderable_cache_.find(entity);
            if (renderable_search != renderable_cache_.end())
            {
                shadow_casting_renderables[cascade_index].push_back(renderable_search->second);
                continue;
            }

            // Generate new renderable and place it in the cache
            const Transform& transform = renderable_view.get<const Transform>(entity);
            const Material& material = renderable_view.get<const Material>(entity);
            const Volume& volume = renderable_view.get<const Volume>(entity);
            std::unique_ptr<Animator> animator{};
            std::unique_ptr<ModelInstance> model_instance{};
            std::unique_ptr<PrimitiveInstance> primitive_instance{};
            if (model_instance_view.contains(entity))
            {
                model_instance = std::make_unique<ModelInstance>(model_instance_view.get<const ModelInstance>(entity));
                if (animated_model_instance_view.contains(entity))
                {
                    animator = std::make_unique<Animator>(animated_model_instance_view.get<const Animator>(entity));
                }
            }
            else
            {
                primitive_instance = std::make_unique<PrimitiveInstance>(primitive_instance_view.get<const PrimitiveInstance>(entity));
            }

            std::shared_ptr<IRenderable> renderable = std::make_shared<Renderable>(material,
                                                                                   transform,
                                                                                   volume,
                                                                                   std::move(animator),
                                                                                   std::move(model_instance),
                                                                                   std::move(primitive_instance));
            shadow_casting_renderables[cascade_index].push_back(renderable);
            renderable_cache_.emplace(entity, renderable);
        }
    }

    return shadow_casting_renderables;
}

} // namespace zero::render

