#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLModel.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace zero::render;

GLModelManager::GLModelManager()
: file_map_()
, model_map_()
, random_generator_(std::random_device()())
{}

bool GLModelManager::LoadModel(const std::string& filename) {
    if (file_map_.find(filename) != file_map_.end()) return true;

    Assimp::Importer importer;

    auto flags = aiProcess_Triangulate
                | aiProcess_OptimizeMeshes
                | aiProcess_OptimizeGraph
                | aiProcess_GenNormals
                | aiProcess_GenBoundingBoxes
                | aiProcess_ImproveCacheLocality;
    const aiScene* scene = importer.ReadFile(filename.c_str(), flags);

    if (!scene
    || scene->mFlags & (unsigned)AI_SCENE_FLAGS_INCOMPLETE
    || !scene->mRootNode) {
        return false;
    }

    uint32 identifier = random_generator_();
    file_map_.emplace(filename, identifier);
    model_map_.emplace(identifier, GLModel::CreateGLModel(filename, random_generator_, identifier, scene->mRootNode, scene));
    return true;
}

void GLModelManager::ClearModels() {
    file_map_.clear();
    model_map_.clear();
}

std::shared_ptr<GLModel> GLModelManager::GetModel(const std::string& filename) {
    auto model_search = file_map_.find(filename);
    if (model_search == file_map_.end()) return nullptr;
    return GetModel(model_search->second);
}

std::shared_ptr<GLModel> GLModelManager::GetModel(uint32 id) {
    auto model_search = model_map_.find(id);
    if (model_search == model_map_.end()) return nullptr;
    return model_search->second;
}
