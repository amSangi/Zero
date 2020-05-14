#include "render/opengl/GLAssetImportWrapper.hpp"
#include <math/Box.hpp>
#include <assimp/scene.h>
#include "render/opengl/GLMesh.hpp"

using namespace zero::render;

std::shared_ptr<GLMesh> GLAssetImportWrapper::LoadMesh(aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    // Interleave the vertex data
    for (uint32 i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex{};
        vertex.position_ = math::Vec3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal_ = math::Vec3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.texture_coordinate_ = mesh->mTextureCoords[0] ? math::Vec2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                                                             : math::Vec2f();
        vertices.push_back(vertex);
    }

    // Save the vertex index data
    for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (uint32 j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return std::make_shared<GLMesh>(std::move(vertices), std::move(indices));
}

Material GLAssetImportWrapper::LoadMaterial(aiMaterial* ai_material) {
    Material material;

    material.name_ = ai_material->GetName().C_Str();

    // Textures
    // Only support 1 texture per texture type
    aiString path;
    ai_material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
    material.texture_map_.diffuse_map_ = path.C_Str();
    path.Clear();

    ai_material->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &path);
    material.texture_map_.specular_map_ = path.C_Str();
    path.Clear();

    ai_material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &path);
    material.texture_map_.normal_map_ = path.C_Str();

    return material;
}