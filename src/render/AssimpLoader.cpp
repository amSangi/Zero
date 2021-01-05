#include "render/AssimpLoader.hpp"
#include "core/Logger.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace zero::render
{

constexpr auto kLogTitle = "AssimpLoader";

AssimpLoader::AssimpLoader(IModelManager* model_manager)
: model_manager_(model_manager)
{
}

void AssimpLoader::LoadOBJModel(const std::string& model_name, const std::string& obj_file_path)
{
    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(obj_file_path.c_str(), GetOBJImportFlags());

    if (!scene || !scene->mRootNode)
    {
        LOG_ERROR(kLogTitle, "Failed to load Wavefront OBJ model: " + obj_file_path);
        return;
    }

    auto model = LoadModel(model_name, scene, scene->mRootNode);
    if (model == nullptr)
    {
        LOG_ERROR(kLogTitle, "Failed to load Wavefront OBJ model: " + obj_file_path);
    }
}

void AssimpLoader::LoadFBXModel(const std::string& model_name, const std::string& fbx_file_path)
{
    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(fbx_file_path.c_str(), GetFBXImportFlags());

    if (!scene || !scene->mRootNode)
    {
        LOG_ERROR(kLogTitle, "Failed to load FBX model: " + fbx_file_path);
        return;
    }

    auto model = LoadModel(model_name, scene, scene->mRootNode);
    if (model == nullptr)
    {
        LOG_ERROR(kLogTitle, "Failed to load FBX model: " + fbx_file_path);
    }
}

std::shared_ptr<Model> AssimpLoader::LoadModel(const std::string& model_name, const aiScene* ai_scene, const aiNode* ai_node)
{
    if (ai_node->mNumMeshes != 1)
    {
        // Each node in a model must have exactly one mesh
        LOG_ERROR(kLogTitle, "Could not load 3D model node because it does not have a name. Model name: " + model_name);
        return nullptr;
    }

    // Load transform prototype
    const aiMatrix4x4& ai_matrix = ai_node->mTransformation;
    math::Matrix4x4 transformation{ai_matrix.a1, ai_matrix.a2, ai_matrix.a3, ai_matrix.a4,
                                   ai_matrix.b1, ai_matrix.b2, ai_matrix.b3, ai_matrix.b4,
                                   ai_matrix.c1, ai_matrix.c2, ai_matrix.c3, ai_matrix.c4,
                                   ai_matrix.d1, ai_matrix.d2, ai_matrix.d3, ai_matrix.d4};
    Transform transform = Transform::FromMatrix4x4(transformation);

    // Load material prototype
    Material material{};
    if (ai_scene->mNumMaterials > 0)
    {
        aiMaterial* ai_material = ai_scene->mMaterials[0];
        material.name_ = ai_material->GetName().C_Str();

        aiString ai_path{};
        ai_material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &ai_path);
        material.texture_map_.diffuse_map_ = ai_path.C_Str();

        ai_material->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &ai_path);
        material.texture_map_.specular_map_  = ai_path.C_Str();

        ai_material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &ai_path);
        material.texture_map_.normal_map_  = ai_path.C_Str();
    }

    // Compute the spherical volume of the mesh
    aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[0]];
    Volume volume{};
    aiAABB& aabb = ai_mesh->mAABB;
    math::Vec3f min{aabb.mMin.x, aabb.mMin.y, aabb.mMin.z};
    math::Vec3f max{aabb.mMax.x, aabb.mMax.y, aabb.mMax.z};
    volume.Engulf(Volume{min, max});

    // Load ModelInstance prototype
    ModelInstance model_instance{};
    model_instance.model_name_ = model_name;
    model_instance.node_name_ = ai_node->mName.C_Str();

    // TODO: Load all the animations for this node
    // Preload all animations LoadFBXModel and pass them into ths method
    // Iterate over aiScene->mAnimations
    // Iterate over all aiAnimation->mChannels
    // If ai_node->mName is the same as aiNodeAnim->mNodeName, load the animations

    auto root_model = model_manager_->CreateModel(model_name,
                                                  LoadMesh(ai_mesh),
                                                  transform,
                                                  material,
                                                  volume,
                                                  model_instance);

    // Construct child models
    for (uint32 i = 0; i < ai_node->mNumChildren; ++i)
    {
        aiNode* ai_child_node = ai_node->mChildren[i];
        auto child_model = LoadModel(model_name, ai_scene, ai_child_node);
        child_model->SetParent(root_model);
        root_model->AddChild(child_model);
    }

    return root_model;
}

Mesh AssimpLoader::LoadMesh(aiMesh* ai_mesh) const
{
    std::vector<Vertex> vertices{};
    std::vector<uint32> indices{};
    vertices.reserve(ai_mesh->mNumVertices);
    indices.reserve(ai_mesh->mNumFaces * 3);

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
                }
            }
        }
    }

    return Mesh{std::move(vertices), std::move(indices)};
}

uint32 AssimpLoader::GetOBJImportFlags() const
{
    uint32 flags = aiProcess_FlipUVs
                 | aiProcess_GenBoundingBoxes
                 | aiProcess_GenNormals
                 | aiProcess_ImproveCacheLocality
                 | aiProcess_OptimizeGraph
                 | aiProcess_OptimizeMeshes
                 | aiProcess_Triangulate;
    return flags;
}

uint32 AssimpLoader::GetFBXImportFlags() const
{
    uint32 flags = aiProcess_FlipUVs
                   | aiProcess_GenBoundingBoxes
                   | aiProcess_GenNormals
                   | aiProcess_LimitBoneWeights
                   | aiProcess_OptimizeMeshes
                   | aiProcess_OptimizeGraph
                   | aiProcess_Triangulate;
    return flags;
}


} // namespace zero::render