#include "render/ModelLoader.hpp"
#include "core/Logger.hpp"
#include "render/MeshData.hpp"
#include "math/Matrix4x4.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <queue>

namespace zero::render
{

namespace assimp
{
bool IsValidScene(const aiScene* ai_scene)
{
    return ai_scene != nullptr             // Non-null scene
           && ai_scene->mRootNode != nullptr; // Non-null root node
}

constexpr uint32 GetImportFlags()
{
    constexpr uint32 flags = aiProcess_FlipUVs
                             | aiProcess_GenBoundingBoxes
                             | aiProcess_GenNormals
                             | aiProcess_OptimizeMeshes
                             | aiProcess_OptimizeGraph
                             | aiProcess_Triangulate;
    return flags;
}

std::unique_ptr<MeshData> ExtractMesh(aiMesh* ai_mesh)
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

    return std::make_unique<MeshData>(std::move(vertices), std::move(indices));
}

std::unique_ptr<Material> ExtractMaterial(aiMaterial* ai_material)
{
    std::unique_ptr<Material> material = std::make_unique<Material>();
    material->name_ = ai_material->GetName().C_Str();

    Material::TextureMap texture_map{};
    aiString ai_path{};
    ai_material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &ai_path);
    texture_map.diffuse_map_ = ai_path.C_Str();

    ai_material->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &ai_path);
    texture_map.specular_map_  = ai_path.C_Str();

    ai_material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &ai_path);
    texture_map.normal_map_  = ai_path.C_Str();

    material->SetTextureMap(texture_map);
    return std::move(material);
}

math::Matrix4x4 ExtractTransformation(const aiMatrix4x4& ai_matrix)
{
    return math::Matrix4x4{ai_matrix.a1, ai_matrix.a2, ai_matrix.a3, ai_matrix.a4,
                           ai_matrix.b1, ai_matrix.b2, ai_matrix.b3, ai_matrix.b4,
                           ai_matrix.c1, ai_matrix.c2, ai_matrix.c3, ai_matrix.c4,
                           ai_matrix.d1, ai_matrix.d2, ai_matrix.d3, ai_matrix.d4};
}

Volume ExtractVolume(aiMesh* ai_mesh)
{
    // Compute the spherical volume of the mesh
    Volume volume{};
    aiAABB& aabb = ai_mesh->mAABB;
    math::Vec3f min{aabb.mMin.x, aabb.mMin.y, aabb.mMin.z};
    math::Vec3f max{aabb.mMax.x, aabb.mMax.y, aabb.mMax.z};
    volume.Engulf(Volume{min, max});
    return volume;
}

std::unique_ptr<Model> LoadModelFromScene(const std::string& model_name, const aiScene *ai_scene, const aiNode *root_ai_node)
{
    std::shared_ptr<Node> root_node = nullptr;
    std::queue<std::pair<const aiNode*, std::shared_ptr<Node>>> node_queue{};
    node_queue.emplace(root_ai_node, root_node);
    while (!node_queue.empty())
    {
        std::pair<const aiNode*, std::shared_ptr<Node>> queue_entry = node_queue.front();
        node_queue.pop();
        const aiNode* ai_node = queue_entry.first;
        std::shared_ptr<Node> parent_node = queue_entry.second;

        Volume volume{};
        std::vector<uint32> geometry_indices{};
        for (uint32 mesh_index = 0; mesh_index < ai_node->mNumMeshes; ++mesh_index)
        {
            aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[mesh_index]];
            volume.Engulf(ExtractVolume(ai_mesh));

            geometry_indices.push_back(mesh_index);
        }

        std::shared_ptr<Node> node = std::make_shared<Node>(volume, ExtractTransformation(ai_node->mTransformation));
        node->geometry_indices_ = geometry_indices;
        if (root_node == nullptr)
        {
            root_node = node;
        }
        if (parent_node)
        {
            parent_node->child_nodes_.push_back(node);
            parent_node->volume_.Engulf(node->volume_);
        }

        for (uint32 child_node_index = 0; child_node_index < ai_node->mNumChildren; ++child_node_index)
        {
            node_queue.emplace(ai_node->mChildren[child_node_index], node);
        }
    }

    std::unique_ptr<Model> model = std::make_unique<Model>(model_name, root_node);
    for (uint32 mesh_index = 0; mesh_index < ai_scene->mNumMeshes; ++mesh_index)
    {
        aiMesh* ai_mesh = ai_scene->mMeshes[mesh_index];
        aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
        model->geometries_.emplace_back(ExtractMesh(ai_mesh), ExtractMaterial(ai_material), ExtractVolume(ai_mesh));
    }
    return model;
}

} // namespace assimp

GeometryData::GeometryData(std::unique_ptr<MeshData> mesh_data, std::unique_ptr<Material> material, const Volume& volume)
: mesh_data_(std::move(mesh_data))
, material_(std::move(material))
, volume_(volume)
, geometry_id_(0)
{
}

Node::Node(const Volume& volume, const math::Matrix4x4& transform)
: volume_(volume)
, transform_(transform)
, geometry_indices_()
, child_nodes_()
{
}

Model::Model(std::string model_name, std::shared_ptr<Node> root_node)
: model_name_(std::move(model_name))
, root_node_(std::move(root_node))
{
}

std::unique_ptr<Model> ModelLoader::LoadModel(const std::string& model_name, const std::string& file_path)
{
    Assimp::Importer importer{};
    const aiScene* ai_scene = importer.ReadFile(file_path.c_str(), assimp::GetImportFlags());
    if (!assimp::IsValidScene(ai_scene))
    {
        LOG_ERROR("ModelLoader", "Failed to load model because the scene structure is invalid. Model: " + file_path);
        return nullptr;
    }
    return assimp::LoadModelFromScene(model_name, ai_scene, ai_scene->mRootNode);
}

} // namespace zero::render