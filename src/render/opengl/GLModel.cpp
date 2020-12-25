#include "render/opengl/GLModel.hpp"

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

void GLModel::Draw()
{
    mesh_->Draw();
}

} // namespace zero::render