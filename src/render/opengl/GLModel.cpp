#include "render/opengl/GLModel.hpp"
#include <assimp/scene.h>
#include <queue>

using namespace zero::render;

std::shared_ptr<GLModel> GLModel::CreateGLModel(const std::string& filename, const aiNode* node, const aiScene* scene) {
    // Construct meshes
    std::vector<std::shared_ptr<GLMesh>> meshes;
    meshes.reserve(node->mNumMeshes);
    for (uint32 i = 0; i < node->mNumMeshes; ++i) {
        meshes.push_back(LoadMesh(scene->mMeshes[node->mMeshes[i]]));
    }

    // Construct Transformation
    const aiMatrix4x4& mat = node->mTransformation;
    math::Matrix4x4 transformation(mat.a1, mat.a2, mat.a3, mat.a4,
                                   mat.b1, mat.b2, mat.b3, mat.b4,
                                   mat.c1, mat.c2, mat.c3, mat.c4,
                                   mat.d1, mat.d2, mat.d3, mat.d4);

    // Material Prototype
    Material material;
    // TODO: Finish Implementation

    // Volume Prototype
    Volume volume;
    // TODO: Finish Implementation

    // MeshInstance Prototype
    MeshInstance mesh_instance;
    mesh_instance.model_file_ = filename;

    // Root GLModel
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

std::shared_ptr<GLMesh> GLModel::LoadMesh(const aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    // Vertices
    for (uint32 i = 0; i < mesh->mNumVertices; ++i) {
        vertices.push_back({
                // Position
                math::Vec3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
                // Normal
                math::Vec3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
                // Texture Coordinate
                mesh->mTextureCoords[0] ? math::Vec2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                                        : math::Vec2f(),
                // Tangent
                math::Vec3f(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z),
                // Bitangent
                math::Vec3f(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z)
        });
    }

    // Indices
    for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (uint32 j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return std::make_shared<GLMesh>(std::move(vertices), std::move(indices));
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