#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLModel.hpp"

namespace zero::render
{

GLModelManager::GLModelManager()
: model_map_()
{
}

std::shared_ptr<Model> GLModelManager::CreateModel(const std::string& model_name,
                                                   const std::vector<Mesh>& meshes,
                                                   const Transform& transform,
                                                   const Material& material,
                                                   const Volume& volume,
                                                   const ModelInstance& model_instance)
{
    std::vector<std::shared_ptr<GLMesh>> gl_meshes{};
    gl_meshes.reserve(meshes.size());
    for (const Mesh& mesh : meshes)
    {
        gl_meshes.push_back(std::make_shared<GLMesh>(mesh.GetVertices(), mesh.GetIndices()));
    }

    auto model = std::make_shared<GLModel>(std::move(gl_meshes), transform, material, volume, model_instance);
    model_map_.emplace(model_name, model);
    return model;
}

void GLModelManager::ClearModels()
{
    model_map_.clear();
}

std::shared_ptr<Model> GLModelManager::GetModel(const std::string& model_name)
{
    auto model_search = model_map_.find(model_name);
    if (model_search == model_map_.end())
    {
        return nullptr;
    }
    return model_search->second;
}

std::shared_ptr<Model> GLModelManager::GetModel(const ModelInstance& model_instance)
{
    auto model = GetModel(model_instance.model_name_);
    if (!model)
    {
        return nullptr;
    }

    if (model->GetModelInstance().node_name_ == model_instance.node_name_)
    {
        return model;
    }

    // Search for the correct child node
    return model->FindChild(model_instance.node_name_);
}

} // namespace zero::render