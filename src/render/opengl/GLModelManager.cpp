#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLModel.hpp"
#include "component/ModelInstance.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace zero::render
{

GLModelManager::GLModelManager()
: model_map_()
, random_generator_(std::random_device()())
{
}

bool GLModelManager::LoadModel(const std::string& model_name, const std::string& filename)
{
    if (model_map_.find(model_name) != model_map_.end())
    {
        return true;
    }

    Assimp::Importer importer;

    auto flags = aiProcess_Triangulate
                | aiProcess_OptimizeMeshes
                | aiProcess_OptimizeGraph
                | aiProcess_GenNormals
                | aiProcess_GenBoundingBoxes
                | aiProcess_ImproveCacheLocality
                | aiProcess_FlipUVs;
    const aiScene* scene = importer.ReadFile(filename.c_str(), flags);

    if (!scene
    || scene->mFlags & (unsigned)AI_SCENE_FLAGS_INCOMPLETE
    || !scene->mRootNode)
    {
        return false;
    }

    model_map_.emplace(model_name, GLModel::CreateGLModel(model_name, random_generator_, 0, scene->mRootNode, scene));
    return true;
}

void GLModelManager::ClearModels()
{
    model_map_.clear();
}

std::shared_ptr<IModel> GLModelManager::GetModel(const std::string& model_name)
{
    auto model_search = model_map_.find(model_name);
    if (model_search == model_map_.end())
    {
        return nullptr;
    }
    return model_search->second;
}

std::shared_ptr<IModel> GLModelManager::GetModel(const ModelInstance& model_instance)
{
    auto model = GetModel(model_instance.model_name_);
    if (!model)
    {
        return nullptr;
    }
    if (model_instance.child_identifier_ != 0)
    {
        model = model->FindChild(model_instance.child_identifier_);
    }
    return model;
}

} // namespace zero::render