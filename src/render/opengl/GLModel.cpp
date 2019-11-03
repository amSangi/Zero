#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLAssetImportWrapper.hpp"
#include <math/Box.hpp>
#include <assimp/scene.h>

using namespace zero::render;

std::shared_ptr<GLModel> GLModel::CreateGLModel(const std::string& filename, const aiNode* node, const aiScene* scene) {
    std::vector<std::shared_ptr<GLMesh>> meshes;
    meshes.reserve(node->mNumMeshes);

    Volume volume;

    Material material;
    // Use the first material for the entire model
    if (scene->mNumMaterials > 0) {
        material = GLAssetImportWrapper::LoadMaterial(scene->mMaterials[0]);
    }

    for (uint32 i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(GLAssetImportWrapper::LoadMesh(mesh));

        // The volume prototype enlarges to contain all the meshes
        aiAABB& aabb = mesh->mAABB;
        math::Vec3f min{aabb.mMin.x, aabb.mMin.y, aabb.mMin.z};
        math::Vec3f max{aabb.mMax.x, aabb.mMax.y, aabb.mMax.z};
        volume.Engulf(Volume(min, max));
    }

    const aiMatrix4x4& mat = node->mTransformation;
    math::Matrix4x4 transformation(mat.a1, mat.a2, mat.a3, mat.a4,
                                   mat.b1, mat.b2, mat.b3, mat.b4,
                                   mat.c1, mat.c2, mat.c3, mat.c4,
                                   mat.d1, mat.d2, mat.d3, mat.d4);

    MeshInstance mesh_instance;
    mesh_instance.model_file_ = filename;


    auto root_model = std::make_shared<GLModel>(meshes,                // Mesh data
                                                transformation,        // Transformation matrix relative to parent
                                                material,              // Material Prototype
                                                volume,                // Volume Prototype
                                                mesh_instance);        // MeshInstance Prototype

    // Create Child GLModels
    std::vector<std::shared_ptr<GLModel>> children;
    for (uint32 i = 0; i < node->mNumChildren; ++i) {
        auto child_model = GLModel::CreateGLModel("", node->mChildren[i], scene);
        child_model->parent_model_ = root_model;
        root_model->child_models_.push_back(std::move(child_model));
    }

    return root_model;
}

GLModel::GLModel(std::vector<std::shared_ptr<GLMesh>> meshes,
                 math::Matrix4x4 transformation,
                 Material material,
                 Volume volume,
                 MeshInstance mesh_instance)
: meshes_(std::move(meshes))
, parent_model_(nullptr)
, child_models_()
, transformation_(transformation)
, material_(std::move(material))
, volume_(volume)
, mesh_instance_(std::move(mesh_instance))
{}

zero::math::Matrix4x4 GLModel::GetTransformation() const {
    return transformation_;
}

Material GLModel::GetMaterial() const {
    return material_;
}

Volume GLModel::GetVolume() const {
    return volume_;
}

MeshInstance GLModel::GetMeshInstance() const {
    return mesh_instance_;
}

std::shared_ptr<GLModel> GLModel::GetParent() const {
    return parent_model_;
}

const std::vector<std::shared_ptr<GLModel>>& GLModel::GetChildren() const {
    return child_models_;
}