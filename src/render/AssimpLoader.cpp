#include "render/AssimpLoader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


namespace zero::render
{

AssimpLoader::AssimpLoader(IModelManager* model_manager)
: model_manager_(model_manager)
{
}

void AssimpLoader::LoadModel(const std::string& model_name, const std::string& model_file_path)
{
    Assimp::Importer importer{};
    auto flags = aiProcess_Triangulate
               | aiProcess_OptimizeMeshes
               | aiProcess_OptimizeGraph
               | aiProcess_GenNormals
               | aiProcess_GenBoundingBoxes
               | aiProcess_ImproveCacheLocality
               | aiProcess_FlipUVs;
    const aiScene* scene = importer.ReadFile(model_file_path.c_str(), flags);

    if (!scene || (scene->mFlags & static_cast<unsigned>(AI_SCENE_FLAGS_INCOMPLETE)) || !scene->mRootNode)
    {
        return;
    }

    LoadModel(model_name, scene, scene->mRootNode);
}

std::shared_ptr<Model> AssimpLoader::LoadModel(const std::string& model_name, const aiScene* scene, const aiNode* node)
{
    // Load transform prototype
    const aiMatrix4x4& mat = node->mTransformation;
    math::Matrix4x4 transformation{mat.a1, mat.a2, mat.a3, mat.a4,
                                   mat.b1, mat.b2, mat.b3, mat.b4,
                                   mat.c1, mat.c2, mat.c3, mat.c4,
                                   mat.d1, mat.d2, mat.d3, mat.d4};
    Transform transform = Transform::FromMatrix4x4(transformation);

    // Load material prototype
    Material material{};
    if (scene->mNumMaterials > 0)
    {
        aiMaterial* ai_material = scene->mMaterials[0];
        material.name_ = ai_material->GetName().C_Str();

        aiString path{};
        ai_material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
        material.texture_map_.diffuse_map_ = path.C_Str();

        ai_material->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &path);
        material.texture_map_.specular_map_  = path.C_Str();

        ai_material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &path);
        material.texture_map_.normal_map_  = path.C_Str();
    }

    // Load volume prototype and mesh data
    Volume volume{};
    std::vector<Mesh> meshes{};
    for (uint32 i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(LoadMesh(ai_mesh));

        // Expand volume
        aiAABB& aabb = ai_mesh->mAABB;
        math::Vec3f min{aabb.mMin.x, aabb.mMin.y, aabb.mMin.z};
        math::Vec3f max{aabb.mMax.x, aabb.mMax.y, aabb.mMax.z};
        volume.Engulf(Volume{min, max});
    }

    // Load model instance prototype and construct root model
    ModelInstance model_instance{};
    model_instance.model_name_ = model_name;
    model_instance.node_name_ = node->mName.C_Str();
    auto root_model = model_manager_->CreateModel(model_name, meshes, transform, material, volume, model_instance);

    // Construct child models
    for (uint32 i = 0; i < node->mNumChildren; ++i)
    {
        aiNode* child_node = node->mChildren[i];
        auto child_model = LoadModel(model_name, scene, child_node);
        child_model->SetParent(root_model);
        root_model->AddChild(child_model);
    }

    return root_model;
}

Mesh AssimpLoader::LoadMesh(aiMesh* mesh) const
{
    std::vector<Vertex> vertices{};
    std::vector<uint32> indices{};
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    // Interleave the vertex data
    for (uint32 i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex{};
        vertex.position_ = math::Vec3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal_ = math::Vec3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.texture_coordinate_ = mesh->mTextureCoords[0] ? math::Vec2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                                                             : math::Vec2f{};

        vertices.push_back(vertex);
    }

    for (uint32 i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        for (uint32 j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh{std::move(vertices), std::move(indices)};
}

} // namespace zero::render