#include "render/renderer/opengl/GLModel.hpp"

namespace zero::render
{

GLModel::GLModel(std::shared_ptr<GLMesh> mesh,
                 Transform transform,
                 Material material,
                 Volume volume,
                 ModelInstance model_instance)
: Model(std::move(transform), std::move(material), std::move(volume), std::move(model_instance))
, mesh_(std::move(mesh))
{
}

std::shared_ptr<IMesh> GLModel::GetMesh()
{
    return std::static_pointer_cast<IMesh>(mesh_);
}

} // namespace zero::render