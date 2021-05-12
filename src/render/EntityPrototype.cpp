#include "render/EntityPrototype.hpp"

namespace zero::render
{

EntityPrototype::EntityPrototype(std::unique_ptr<IMesh> mesh,
                                 std::unique_ptr<Animator> animator,
                                 Material material,
                                 Volume volume,
                                 ModelInstance model_instance)
: mesh_(std::move(mesh))
, animator_(std::move(animator))
, material_(std::move(material))
, volume_(std::move(volume))
, model_instance_(std::move(model_instance))
{
}

IMesh* EntityPrototype::GetMesh() const
{
    return mesh_.get();
}

Animator* EntityPrototype::GetAnimator() const
{
    return animator_.get();
}

const Material& EntityPrototype::GetMaterial() const
{
    return material_;
}

const Volume& EntityPrototype::GetVolume() const
{
    return volume_;
}

const ModelInstance& EntityPrototype::GetModelInstance() const
{
    return model_instance_;
}

} // namespace zero::render

