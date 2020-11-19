#include "render/opengl/GLModel.hpp"

namespace zero::render
{

GLModel::GLModel(std::vector<std::shared_ptr<GLMesh>> meshes,
                 Transform transform,
                 Material material,
                 Volume volume,
                 ModelInstance model_instance)
: Model(std::move(transform), std::move(material), std::move(volume), std::move(model_instance))
, meshes_(std::move(meshes))
{
}

void GLModel::Draw()
{
    for (const auto& mesh : meshes_)
    {
        mesh->Draw();
    }
}

} // namespace zero::render