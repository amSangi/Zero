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

    std::shared_ptr<Node> root_node = CreateNode(model_name, ai_scene, ai_scene->mRootNode, math::Matrix4x4::Identity());
    std::shared_ptr<Model> model = std::make_shared<Model>(model_name, root_node);
    model->Initialize(ExtractAnimator(ai_scene));
    return model;
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

std::shared_ptr<Node> AssimpLoader::CreateNode(const std::string& model_name,
                                               const aiScene *ai_scene,
                                               const aiNode *ai_node,
                                               const math::Matrix4x4& parent_transformation)
{
    // ai_matrix is the transformation relative to the parent.
    // Compute transformation relative to the world.
    const aiMatrix4x4& ai_matrix = ai_node->mTransformation;
    math::Matrix4x4 transformation = parent_transformation * math::Matrix4x4{ai_matrix.a1, ai_matrix.a2, ai_matrix.a3, ai_matrix.a4,
                                                                             ai_matrix.b1, ai_matrix.b2, ai_matrix.b3, ai_matrix.b4,
                                                                             ai_matrix.c1, ai_matrix.c2, ai_matrix.c3, ai_matrix.c4,
                                                                             ai_matrix.d1, ai_matrix.d2, ai_matrix.d3, ai_matrix.d4};

    std::shared_ptr<Node> node = std::make_shared<Node>(ai_node->mName.C_Str(), transformation);
    auto bone_search = bone_map_.find(node->GetName());
    if (bone_search != bone_map_.end())
    {
        // Node is a bone
        std::unique_ptr<Bone> bone = std::make_unique<Bone>();
        bone->name_ = node->GetName();
        const aiMatrix4x4& ai_bone_matrix = bone_search->second->mOffsetMatrix;
        bone->offset_matrix_ = math::Matrix4x4{ai_bone_matrix.a1, ai_bone_matrix.a2, ai_bone_matrix.a3, ai_bone_matrix.a4,
                                               ai_bone_matrix.b1, ai_bone_matrix.b2, ai_bone_matrix.b3, ai_bone_matrix.b4,
                                               ai_bone_matrix.c1, ai_bone_matrix.c2, ai_bone_matrix.c3, ai_bone_matrix.c4,
                                               ai_bone_matrix.d1, ai_bone_matrix.d2, ai_bone_matrix.d3, ai_bone_matrix.d4};
        node->SetBone(std::move(bone));
    }

    // Create EntityPrototypes associated with each mesh
    // If the node does not contain a mesh and is not renderable, the instantiated entity associated with the node
    // will only contain Transform and Volume components
    for (uint32 mesh_index = 0; mesh_index < ai_node->mNumMeshes; ++mesh_index)
    {
        aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[mesh_index]];
        aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
        std::unique_ptr<EntityPrototype> entity_prototype = std::make_unique<EntityPrototype>(mesh_loader_->LoadMesh(ExtractMesh(ai_mesh)),
                                                                                              ExtractMaterial(ai_material),
                                                                                              ExtractVolume(ai_mesh),
                                                                                              ExtractModelInstance(model_name, ai_node, mesh_index));
        node->AddEntityPrototype(std::move(entity_prototype));
    }

    // Create child nodes
    for (uint32 child_index = 0; child_index < ai_node->mNumChildren; ++child_index)
    {
        const aiNode* child_ai_node = ai_node->mChildren[child_index];
        std::shared_ptr<Node> child_node = CreateNode(model_name, ai_scene, child_ai_node, transformation);

        node->AddChild(child_node);
        child_node->SetParent(node);
    }
    return node;
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

std::unique_ptr<Animator> AssimpLoader::ExtractAnimator(const aiScene* ai_scene) const
{
    if (ai_scene->mNumAnimations == 0)
    {
        return nullptr;
    }

    std::unique_ptr<Animator> animator = std::make_unique<Animator>();

    // Load animation data
    for (uint32 animation_index = 0; animation_index < ai_scene->mNumAnimations; ++animation_index)
    {
        aiAnimation* ai_animation = ai_scene->mAnimations[animation_index];
        std::unique_ptr<Animation> animation = std::make_unique<Animation>();
        animation->name_ = ai_animation->mName.C_Str();
        animation->tick_rate_ = ai_animation->mTicksPerSecond;
        animation->duration_ = ai_animation->mDuration;
        for (uint32 channel_index = 0; channel_index < ai_animation->mNumChannels; ++channel_index)
        {
            aiNodeAnim* ai_node_anim = ai_animation->mChannels[channel_index];
            std::unique_ptr<AnimationChannel> animation_channel = std::make_unique<AnimationChannel>();
            animation_channel->name_ = ai_node_anim->mNodeName.C_Str();

            for (uint32 i = 0; i < ai_node_anim->mNumPositionKeys; ++i)
            {
                aiVectorKey ai_translation_key = ai_node_anim->mPositionKeys[i];
                VectorKey translation_key{};
                translation_key.vector_ = math::Vec3f{ai_translation_key.mValue.x,
                                                      ai_translation_key.mValue.y,
                                                      ai_translation_key.mValue.z};
                translation_key.time_ = ai_translation_key.mTime;
                animation_channel->translations_.push_back(translation_key);
            }

            for (uint32 i = 0; i < ai_node_anim->mNumRotationKeys; ++i)
            {
                aiQuatKey ai_rotation_key = ai_node_anim->mRotationKeys[i];
                QuaternionKey rotation_key{};
                rotation_key.quaternion_ = math::Quaternion{ai_rotation_key.mValue.w,
                                                            ai_rotation_key.mValue.x,
                                                            ai_rotation_key.mValue.y,
                                                            ai_rotation_key.mValue.z};
                rotation_key.time_ = ai_rotation_key.mTime;
                animation_channel->rotations_.push_back(rotation_key);
            }

            for (uint32 i = 0; i < ai_node_anim->mNumScalingKeys; ++i)
            {
                aiVectorKey ai_scaling_key = ai_node_anim->mScalingKeys[i];
                VectorKey scaling_key{};
                scaling_key.vector_ = math::Vec3f{ai_scaling_key.mValue.x,
                                                  ai_scaling_key.mValue.y,
                                                  ai_scaling_key.mValue.z};
                scaling_key.time_ = ai_scaling_key.mTime;
                animation_channel->scalings_.push_back(scaling_key);
            }

            animation->channel_map_.emplace(ai_node_anim->mNodeName.C_Str(), std::move(animation_channel));
        }

        animator->AddAnimation(ai_animation->mName.C_Str(), std::move(animation));
    }
    return std::move(animator);
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

ModelInstance AssimpLoader::ExtractModelInstance(const std::string& model_name, const aiNode* ai_node, uint32 mesh_index) const
{
    ModelInstance model_instance{};
    model_instance.model_name_ = model_name;
    model_instance.node_name_ = ai_node->mName.C_Str();
    model_instance.mesh_index_ = mesh_index;
    return model_instance;
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