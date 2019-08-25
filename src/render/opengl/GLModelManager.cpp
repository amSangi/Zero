#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLModel.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace zero::render;

bool GLModelManager::LoadModel(const std::string& filename) {
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

    models_.emplace(filename, GLModel::CreateGLModel(scene->mRootNode, scene));
    return true;
}

void GLModelManager::ClearModels() {
    models_.clear();
}

std::shared_ptr<GLModel> GLModelManager::GetModel(const std::string& filename) {
    auto model_search = models_.find(filename);
    if (model_search == models_.end()) return nullptr;
    return model_search->second;
}