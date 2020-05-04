#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLAssetImportWrapper.hpp"
#include <math/Box.hpp>
#include <assimp/scene.h>

using namespace zero::render;

std::shared_ptr<GLModel> GLModel::CreateGLModel(const std::string& filename,
                                                std::minstd_rand0 rng,
                                                uint32 identifier,
                                                const aiNode* node,
                                                const aiScene* scene) {
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

    Transform transform = Transform::FromMatrix4x4(transformation);

    ModelInstance model_instance{};
    model_instance.filename_ = filename;
    model_instance.child_identifier_ = identifier;

    auto root_model = std::make_shared<GLModel>(meshes,          // Mesh data
                                                transform,       // Transform prototype
                                                material,        // Material Prototype
                                                volume,          // Volume Prototype
                                                model_instance); // ModelInstance Prototype

    // Create Child GLModels
    std::vector<std::shared_ptr<GLModel>> children;
    for (uint32 i = 0; i < node->mNumChildren; ++i) {
        auto child_model = GLModel::CreateGLModel(filename, rng, rng(), node->mChildren[i], scene);
        child_model->parent_model_ = root_model;
        root_model->child_models_.push_back(std::move(child_model));
    }

    return root_model;
}

GLModel::GLModel(std::vector<std::shared_ptr<GLMesh>> meshes,
                 Transform transform,
                 Material material,
                 Volume volume,
                 ModelInstance model_instance)
: meshes_(std::move(meshes))
, parent_model_(nullptr)
, child_models_()
, transform_(std::move(transform))
, material_(std::move(material))
, volume_(volume)
, model_instance_(std::move(model_instance))
{}

void GLModel::Draw() {
    for (const auto& mesh : meshes_) {
        mesh->Draw();
    }
}

zero::Transform GLModel::GetTransform() const {
    return transform_;
}

Material GLModel::GetMaterial() const {
    return material_;
}

Volume GLModel::GetVolume() const {
    return volume_;
}

ModelInstance GLModel::GetModelInstance() const {
    return model_instance_;
}

std::shared_ptr<IModel> GLModel::GetParent() const {
    return parent_model_;
}

std::shared_ptr<IModel> GLModel::FindChild(uint32 identifier) const {
    for (const auto& model : child_models_) {
        if (model->GetModelInstance().child_identifier_ == identifier) {
            return model;
        }
    }
    for (const auto& model : child_models_) {
        auto search = model->FindChild(identifier);
        if (search) {
            return search;
        }
    }

    return nullptr;
}

const std::vector<std::shared_ptr<IModel>>& GLModel::GetChildren() const {
    return child_models_;
}

const std::vector<std::shared_ptr<GLMesh>>& GLModel::GetMeshes() const {
    return meshes_;
}