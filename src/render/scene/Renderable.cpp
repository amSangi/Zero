#include "render/scene/Renderable.hpp"

namespace zero::render
{

Renderable::Renderable(const Material& material,
                       const Transform& transform,
                       const Volume& volume,
                       std::unique_ptr<Animator> animator,
                       std::unique_ptr<ModelInstance> model_instance,
                       std::unique_ptr<PrimitiveInstance> primitive_instance)
: material_(material)
, transform_(transform)
, volume_(volume)
, animator_(std::move(animator))
, model_instance_(std::move(model_instance))
, primitive_instance_(std::move(primitive_instance))
{
}

Renderable::~Renderable()
{
}

const Material& Renderable::GetMaterial()
{
    return material_;
}

const Transform& Renderable::GetTransform()
{
    return transform_;
}

const Volume& Renderable::GetVolume()
{
    return volume_;
}

Animator* Renderable::GetAnimator()
{
    return animator_.get();
}

ModelInstance* Renderable::GetModelInstance()
{
    return model_instance_.get();
}

PrimitiveInstance* Renderable::GetPrimitiveInstance()
{
    return primitive_instance_.get();
}

} // namespace zero::render

