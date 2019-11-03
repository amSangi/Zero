#include "render/opengl/GLAssetImportWrapper.hpp"
#include <math/Box.hpp>
#include <assimp/scene.h>

using namespace zero::render;

std::shared_ptr<GLMesh> GLAssetImportWrapper::LoadMesh(aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<uint16> indices;
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    // Interleave the vertex data
    for (uint32 i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex{
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
        };
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
    ai_material->GetTexture(aiTextureType::aiTextureType_OPACITY, 0, &path);
    material.texture_map_.alpha_map_ = path.C_Str();
    path.Clear();

    ai_material->GetTexture(aiTextureType::aiTextureType_AMBIENT, 0, &path);
    material.texture_map_.ambient_map_ = path.C_Str();
    path.Clear();

    ai_material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
    material.texture_map_.diffuse_map_ = path.C_Str();
    path.Clear();

    ai_material->GetTexture(aiTextureType::aiTextureType_DISPLACEMENT, 0, &path);
    material.texture_map_.displacement_map_ = path.C_Str();
    path.Clear();

    ai_material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &path);
    material.texture_map_.normal_map_ = path.C_Str();

    // Color
    ai_material->Get(AI_MATKEY_COLOR_AMBIENT, material.ambient_color_);
    ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, material.diffuse_color_);
    ai_material->Get(AI_MATKEY_COLOR_EMISSIVE, material.emissive_color_);
    ai_material->Get(AI_MATKEY_COLOR_SPECULAR, material.specular_color_);
    ai_material->Get(AI_MATKEY_COLOR_TRANSPARENT, material.transparent_color_);
    ai_material->Get(AI_MATKEY_SHININESS, material.specular_exponent_);

    ai_material->Get(AI_MATKEY_OPACITY, material.opacity_);

    ai_material->Get(AI_MATKEY_ENABLE_WIREFRAME, material.wireframe_enabled_);

    return material;
}