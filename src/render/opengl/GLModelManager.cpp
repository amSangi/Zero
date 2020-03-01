#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLModel.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace zero::render;

GLModelManager::GLModelManager()
: model_map_()
, random_generator_(std::random_device()())
{}

bool GLModelManager::LoadModel(const std::string& filename) {
    if (model_map_.find(filename) != model_map_.end()) return true;

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
    || !scene->mRootNode) {
        return false;
    }

    model_map_.emplace(filename, GLModel::CreateGLModel(filename, random_generator_, 0, scene->mRootNode, scene));
    return true;
}

void GLModelManager::ClearModels() {
    model_map_.clear();
}

std::shared_ptr<GLModel> GLModelManager::GetModel(const std::string& filename) {
    auto model_search = model_map_.find(filename);
    if (model_search == model_map_.end()) return nullptr;
    return model_search->second;
}

std::shared_ptr<GLModel> GLModelManager::GetModel(const ModelInstance& model_instance) {
    auto model = GetModel(model_instance.filename_);
    if (!model) {
        return nullptr;
    }
    if (model_instance.child_identifier_ != 0) {
        model = model->FindChild(model_instance.child_identifier_);
    }
    return model;
}