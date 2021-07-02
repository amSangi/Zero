#include "render/scene/Renderable.hpp"

namespace zero::render
{

Renderable::Renderable(const Material& material,
                       const Transform& transform,
                       const Volume& volume,
                       std::unique_ptr<ModelInstance> model_instance,
                       std::unique_ptr<PrimitiveInstance> primitive_instance,
                       std::shared_ptr<std::vector<math::Matrix4x4>> bone_matrices)
: material_(material)
, transform_(transform)
, volume_(volume)
, model_instance_(std::move(model_instance))
, primitive_instance_(std::move(primitive_instance))
, bone_matrices_(std::move(bone_matrices))
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

ModelInstance* Renderable::GetModelInstance()
{
    return model_instance_.get();
}

PrimitiveInstance* Renderable::GetPrimitiveInstance()
{
    return primitive_instance_.get();
}

std::shared_ptr<std::vector<math::Matrix4x4>> Renderable::GetBoneMatrices()
{
    return bone_matrices_;
}

} // namespace zero::render

