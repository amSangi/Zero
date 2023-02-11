#include "engine/EntityInstantiator.hpp"
#include "render/RenderSystem.hpp"

namespace zero
{

EntityInstantiator::EntityInstantiator(entt::registry& registry, render::RenderSystem* render_system)
: registry_(registry)
, render_system_(render_system)
{
}

Entity EntityInstantiator::InstantiateModel(const std::string& model_filename, Entity parent_entity)
{
    return render_system_->CreateModelInstance(model_filename, parent_entity);
}

Entity EntityInstantiator::InstantiatePrimitive(const PrimitiveInstance& primitive)
{
    return render_system_->CreatePrimitiveInstance(primitive);
}

Entity EntityInstantiator::InstantiateLight(const Light& light, Entity entity)
{
    return render_system_->CreateLightInstance(light, entity);
}

Entity EntityInstantiator::InstantiateSkyDome(const SkyDome& sky_dome)
{
    Entity entity = registry_.create();
    registry_.emplace<SkyDome>(entity, sky_dome);
    return entity;
}

} // namespace zero

