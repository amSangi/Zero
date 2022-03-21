#include "render/AssimpLoader.hpp"
#include "core/Logger.hpp"
#include <assimp/Importer.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <queue>

namespace zero::render
{

constexpr auto kLogTitle = "AssimpLoader";

AssimpLoader::AssimpLoader(IMeshLoader* mesh_loader)
: mesh_loader_(mesh_loader)
, bone_map_()
{
}

std::shared_ptr<Model> AssimpLoader::LoadModel(const std::string& model_name, const std::string& file_path)
{
    Assimp::Importer importer{};
    const aiScene* ai_scene = importer.ReadFile(file_path.c_str(), GetImportFlags());

    if (!IsValidScene(ai_scene))
    {
        LOG_ERROR(kLogTitle, "Failed to load model because the scene structure is invalid. Model: " + file_path);
        return nullptr;
    }

    LoadBoneMap(ai_scene);

//    std::shared_ptr<Node> root_node = CreateNode(model_name, ai_scene, ai_scene->mRootNode);
//    std::shared_ptr<Model> model = std::make_shared<Model>(model_name, root_node);
    return nullptr;
}

bool AssimpLoader::IsValidScene(const aiScene* ai_scene) const
{
    return ai_scene != nullptr             // Non-null scene
        && ai_scene->mRootNode != nullptr; // Non-null root node
}

void AssimpLoader::LoadBoneMap(const aiScene* ai_scene)
{
    std::queue<const aiNode*> node_queue{};
    node_queue.push(ai_scene->mRootNode);

    // Add bones to map iteratively using breadth first search
    while (!node_queue.empty())
    {
        const aiNode* ai_node = node_queue.front();
        node_queue.pop();

        for (uint32 mesh_index = 0; mesh_index < ai_node->mNumMeshes; ++mesh_index)
        {
            aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[mesh_index]];
            for (uint32 ai_bone_index = 0; ai_bone_index < ai_mesh->mNumBones; ++ai_bone_index)
            {
                aiBone* ai_bone = ai_mesh->mBones[ai_bone_index];
                bone_map_.emplace(ai_bone->mName.C_Str(), ai_bone);
            }
        }

        // Add children to queue
        for (uint32 child_index = 0; child_index < ai_node->mNumChildren; ++child_index)
        {
            node_queue.push(ai_node->mChildren[child_index]);
        }
    }
}

std::unique_ptr<Mesh> AssimpLoader::ExtractMesh(aiMesh* ai_mesh) const
{
    std::vector<Vertex> vertices{};
    std::vector<uint32> indices{};
    // List of bone names in the order that the mesh is rigged
    std::vector<std::string> bone_names{};

    vertices.reserve(ai_mesh->mNumVertices);
    indices.reserve(ai_mesh->mNumFaces * 3);
    // Allow direct indexing of the list of bone names
    bone_names.resize(ai_mesh->mNumBones);

    // Load interleaved position, normal, and UV attributes
    for (uint32 i = 0; i < ai_mesh->mNumVertices; ++i)
    {
        Vertex vertex{};
        vertex.position_ = math::Vec3f(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
        vertex.normal_ = math::Vec3f(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
        if (ai_mesh->HasTextureCoords(0))
        {
            vertex.texture_coordinate_ = math::Vec2f(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y);
        }
        vertices.push_back(vertex);
    }

    // Load index data
    for (uint32 i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        const aiFace& ai_face = ai_mesh->mFaces[i];
        for (uint32 j = 0; j < ai_face.mNumIndices; ++j)
        {
            indices.push_back(ai_face.mIndices[j]);
        }
    }

    // Load the bone ID and weight attributes
    for (uint32 ai_bone_index = 0; ai_bone_index < ai_mesh->mNumBones; ++ai_bone_index)
    {
        const aiBone* ai_bone = ai_mesh->mBones[ai_bone_index];
        bone_names[ai_bone_index] = ai_bone->mName.C_Str();

        for (uint32 weight_index = 0; weight_index < ai_bone->mNumWeights; ++weight_index)
        {
            const aiVertexWeight& ai_vertex_weight = ai_bone->mWeights[weight_index];
            const uint32 vertex_index = ai_vertex_weight.mVertexId;
            const float bone_weight = static_cast<float>(ai_vertex_weight.mWeight);

            // Set the bone attributes in the next available slot
            Vertex& vertex = vertices[vertex_index];
            for (uint32 i = 0; i < vertex.bone_ids_.GetDimensions(); ++i)
            {
                if (vertex.bone_ids_[i] == Vertex::kInvalidBoneID)
                {
                    vertex.bone_ids_[i] = ai_bone_index;
                    vertex.bone_weights_[i] = bone_weight;
                    break;
                }
            }
        }
    }

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices), std::move(bone_names));
}

Material AssimpLoader::ExtractMaterial(const aiMaterial* ai_material) const
{
    Material material{};
    material.name_ = ai_material->GetName().C_Str();

    aiString ai_path{};
    ai_material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &ai_path);
    material.texture_map_.diffuse_map_ = ai_path.C_Str();

    ai_material->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &ai_path);
    material.texture_map_.specular_map_  = ai_path.C_Str();

    ai_material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &ai_path);
    material.texture_map_.normal_map_  = ai_path.C_Str();
    return material;
}

Volume AssimpLoader::ExtractVolume(aiMesh* ai_mesh) const
{
    // Compute the spherical volume of the mesh
    Volume volume{};
    aiAABB& aabb = ai_mesh->mAABB;
    math::Vec3f min{aabb.mMin.x, aabb.mMin.y, aabb.mMin.z};
    math::Vec3f max{aabb.mMax.x, aabb.mMax.y, aabb.mMax.z};
    volume.Engulf(Volume{min, max});
    return volume;
}

constexpr uint32 AssimpLoader::GetImportFlags() const
{
    constexpr uint32 flags = aiProcess_FlipUVs
                           | aiProcess_GenBoundingBoxes
                           | aiProcess_GenNormals
                           | aiProcess_LimitBoneWeights
                           | aiProcess_OptimizeMeshes
                           | aiProcess_OptimizeGraph
                           | aiProcess_Triangulate;
    return flags;
}

} // namespace zero::render