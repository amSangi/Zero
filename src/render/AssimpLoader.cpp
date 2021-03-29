#include "render/AssimpLoader.hpp"
#include "core/Logger.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace zero::render
{

constexpr auto kLogTitle = "AssimpLoader";

AssimpLoader::AssimpLoader(IModelBuilder* model_builder)
: model_builder_(model_builder)
, ai_node_map_()
, ai_animation_map_()
{
}

void AssimpLoader::LoadModel(const std::string& model_name, const std::string& file_path)
{
    Assimp::Importer importer{};
    const aiScene* ai_scene = importer.ReadFile(file_path.c_str(), GetImportFlags());

    if (!IsValidScene(ai_scene))
    {
        LOG_ERROR(kLogTitle, "Failed to load model because the scene structure is invalid. Model: " + file_path);
        return;
    }

    // Load the {node name, node} and {node name, animation channel} maps
    LoadNodeMap(ai_scene->mRootNode);
    LoadAnimationMap(ai_scene);

    // Construct root model
    aiNode* ai_root_node = ai_scene->mRootNode;
    aiMesh* ai_mesh = ai_scene->mMeshes[ai_root_node->mMeshes[0]];
    math::Matrix4x4 transformation = ExtractTransformation(ai_root_node);
    std::shared_ptr<Model> root_model = model_builder_->BuildModel(model_name,
                                                                   ExtractMesh(ai_mesh),
                                                                   Transform::FromMatrix4x4(transformation),
                                                                   ExtractMaterial(ai_scene, ai_root_node),
                                                                   ExtractVolume(ai_mesh),
                                                                   ExtractModelInstance(model_name, ai_root_node));
    // Construct child models
    for (uint32 i = 0; i < ai_root_node->mNumChildren; ++i)
    {
        aiNode* ai_child_node = ai_root_node->mChildren[i];
        LoadModel(root_model, model_name, ai_scene, ai_child_node, transformation);
    }

    // Clear cache because the scene will be released
    ai_node_map_.clear();
    ai_animation_map_.clear();
}

bool AssimpLoader::IsValidScene(const aiScene* ai_scene) const
{
    return ai_scene != nullptr                   // Non-null scene
        && ai_scene->mRootNode != nullptr        // Non-null root node
        && ai_scene->mRootNode->mNumMeshes > 0;  // Root node contains at least 1 mesh
}

void AssimpLoader::LoadNodeMap(aiNode* ai_node)
{
    ai_node_map_.emplace(std::string(ai_node->mName.C_Str()), ai_node);
    for (uint32 i = 0; i < ai_node->mNumChildren; ++i)
    {
        LoadNodeMap(ai_node->mChildren[i]);
    }
}

void AssimpLoader::LoadAnimationMap(const aiScene* ai_scene)
{
    for (uint32 animation_index = 0; animation_index < ai_scene->mNumAnimations; ++animation_index)
    {
        const aiAnimation* ai_animation = ai_scene->mAnimations[animation_index];
        for (uint32 channel_index = 0; channel_index < ai_animation->mNumChannels; ++channel_index)
        {
            aiNodeAnim* ai_node_anim = ai_animation->mChannels[channel_index];
            ai_animation_map_.emplace(std::string(ai_node_anim->mNodeName.C_Str()), ai_node_anim);
        }
    }
}

void AssimpLoader::LoadModel(std::shared_ptr<Model> parent_model,
                             const std::string& model_name,
                             const aiScene* ai_scene,
                             const aiNode* ai_node,
                             const math::Matrix4x4& parent_node_transformation)
{
    math::Matrix4x4 transformation = parent_node_transformation * ExtractTransformation(ai_node);
    if (ai_node->mNumMeshes == 0)
    {
        // Do not construct model but propagate node transform
        for (uint32 i = 0; i < ai_node->mNumChildren; ++i)
        {
            aiNode* ai_child_node = ai_node->mChildren[i];
            LoadModel(parent_model, model_name, ai_scene, ai_child_node, parent_node_transformation);
        }

        return;
    }

    if (ai_node->mNumMeshes > 1)
    {
        LOG_WARN(kLogTitle, "Loader does not support multi-mesh nodes. Extra meshes will be ignored. Model: " + model_name);
    }

    // Assume each node has at most 1 mesh
    aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[0]];
    std::shared_ptr<Model> model = model_builder_->BuildModel(model_name,
                                                              ExtractMesh(ai_mesh),
                                                              Transform::FromMatrix4x4(transformation),
                                                              ExtractMaterial(ai_scene, ai_node),
                                                              ExtractVolume(ai_mesh),
                                                              ExtractModelInstance(model_name, ai_node));
    model->SetParent(parent_model);
    parent_model->AddChild(model);

    // Recursively load child models
    for (uint32 i = 0; i < ai_node->mNumChildren; ++i)
    {
        aiNode* ai_child_node = ai_node->mChildren[i];
        LoadModel(model, model_name, ai_scene, ai_child_node, parent_node_transformation);
    }
}

math::Matrix4x4 AssimpLoader::ExtractTransformation(const aiNode* ai_node) const
{
    const aiMatrix4x4& ai_matrix = ai_node->mTransformation;
    return math::Matrix4x4{ai_matrix.a1, ai_matrix.a2, ai_matrix.a3, ai_matrix.a4,
                           ai_matrix.b1, ai_matrix.b2, ai_matrix.b3, ai_matrix.b4,
                           ai_matrix.c1, ai_matrix.c2, ai_matrix.c3, ai_matrix.c4,
                           ai_matrix.d1, ai_matrix.d2, ai_matrix.d3, ai_matrix.d4};
}

std::unique_ptr<Mesh> AssimpLoader::ExtractMesh(aiMesh* ai_mesh) const
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

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

Material AssimpLoader::ExtractMaterial(const aiScene* ai_scene, const aiNode* ai_node) const
{
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

ModelInstance AssimpLoader::ExtractModelInstance(const std::string& model_name, const aiNode* ai_node) const
{
    ModelInstance model_instance{};
    model_instance.model_name_ = model_name;
    model_instance.node_name_ = ai_node->mName.C_Str();
    return model_instance;
}

uint32 AssimpLoader::GetImportFlags() const
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