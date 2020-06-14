#include "engine/EntityInstantiator.hpp"
#include "render/RenderSystem.hpp"

namespace zero
{

EntityInstantiator::EntityInstantiator(render::RenderSystem* render_system)
: render_system_(render_system)
{
}

Entity EntityInstantiator::InstantiateModel(const std::string& model_filename)
{
    return render_system_->CreateModelInstance(model_filename);
}

Entity EntityInstantiator::InstantiatePrimitive(const PrimitiveInstance& primitive)
{
    return render_system_->CreatePrimitiveInstance(primitive);
}

Entity EntityInstantiator::InstantiateLight(const Light& light, Entity entity)
{
    return render_system_->CreateLightInstance(light, entity);
}

} // namespace zero

