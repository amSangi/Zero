#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/Components.hpp"
#include "render/Image.hpp"

using namespace zero::render;

constexpr zero::uint8 kAlphaTextureIndex = 0;
constexpr zero::uint8 kAmbientTextureIndex = 1;
constexpr zero::uint8 kDiffuseTextureIndex = 2;
constexpr zero::uint8 kDisplacementTextureIndex = 3;
constexpr zero::uint8 kNormalTextureIndex = 4;

constexpr auto kAlphaTextureUniformName = "alpha_texture";
constexpr auto kAmbientTextureUniformName = "ambient_texture";
constexpr auto kDiffuseTextureUniformName = "diffuse_texture";
constexpr auto kDisplacementTextureUniformName = "displacement_texture";
constexpr auto kNormalTextureUniformName = "normal_texture";

GLTextureManager::GLTextureManager()
: image_map_()
{}

zero::uint8 GLTextureManager::GetTextureUnitCount() const {
    GLint count = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &count);
    return count;
}

void GLTextureManager::SetSampler(const std::shared_ptr<GLSampler>& sampler, uint8 index) {
    if (index >= GetTextureUnitCount() || !sampler) {
        return;
    }
    glBindSampler(index, sampler->GetNativeIdentifier());
}

std::vector<std::shared_ptr<GLTexture>> GLTextureManager::CreateTextureMap(const Material& material) {
    std::vector<std::shared_ptr<GLTexture>> gl_textures;
    auto gl_alpha_texture = CreateTexture(material.texture_map_.alpha_map_, kAlphaTextureIndex, kAlphaTextureUniformName);
    auto gl_ambient_texture = CreateTexture(material.texture_map_.ambient_map_, kAmbientTextureIndex, kAmbientTextureUniformName);
    auto gl_diffuse_texture = CreateTexture(material.texture_map_.diffuse_map_, kDiffuseTextureIndex, kDiffuseTextureUniformName);
    auto gl_displacement_texture = CreateTexture(material.texture_map_.displacement_map_, kDisplacementTextureIndex, kDisplacementTextureUniformName);
    auto gl_normal_texture = CreateTexture(material.texture_map_.normal_map_, kNormalTextureIndex, kNormalTextureUniformName);
    if (gl_alpha_texture) {
        gl_textures.push_back(gl_alpha_texture);
    }
    if (gl_ambient_texture) {
        gl_textures.push_back(gl_ambient_texture);
    }
    if (gl_diffuse_texture) {
        gl_textures.push_back(gl_diffuse_texture);
    }
    if (gl_displacement_texture) {
        gl_textures.push_back(gl_displacement_texture);
    }
    if (gl_normal_texture) {
        gl_textures.push_back(gl_normal_texture);
    }
    return gl_textures;
}


std::shared_ptr<GLTexture> GLTextureManager::CreateTexture(const std::string& filename,
                                                           zero::uint8 index,
                                                           const std::string& uniform_name) {
    if (index >= GetTextureUnitCount()) {
        return nullptr;
    }

    auto search = image_map_.find(filename);
    if (search == image_map_.end()) {
        return nullptr;
    }

    GLenum texture_unit = GL_TEXTURE0 + index;
    GLenum target = GL_TEXTURE_2D;

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glActiveTexture(texture_unit);
    glBindTexture(target, texture_id);

    // Load image
    auto image = search->second;
    if (!image->Load()) {
        glDeleteTextures(1, &texture_id);
        return nullptr;
    }

    // Create texture
    GLenum data_format;
    switch (image->GetPixelFormat())
    {
        case Image::PixelFormat::BGR:
            data_format = GL_BGR;
            break;
        case Image::PixelFormat::RGB:
            data_format = GL_RGB;
            break;
        case Image::PixelFormat::RGBA:
            data_format = GL_RGBA;
            break;
        case Image::PixelFormat::BGRA:
            data_format = GL_BGRA;
            break;
        default:
            return nullptr;
    }

    glTexImage2D(target,              // Target texture.
                 0,                   // Level of detail. Level n is the nth mipmap reduction image.
                 data_format,         // Number of colour components in the texture.
                 image->GetWidth(),   // Texture image width.
                 image->GetHeight(),  // Texture image height.
                 0,                   // Border. Must be 0.
                 data_format,         // Format of the pixel data.
                 GL_UNSIGNED_BYTE,    // Data type of the pixel data.
                 image->GetData());   // Pointer to the image data in memory.

    auto gl_texture = std::make_shared<GLTexture>(texture_id, target);
    gl_texture->SetUniformName(uniform_name);
    return gl_texture;
}

bool GLTextureManager::InitializeImage(const std::string& filename) {
    if (image_map_.find(filename) != image_map_.end()) return true;

    auto image = std::make_shared<Image>(filename);
    if (!image->Load()) return false;

    image->Release();
    image_map_[filename] = std::move(image);
    return true;
}

void GLTextureManager::ClearImages() {
    image_map_.clear();
}

void GLTextureManager::UnloadImages(){
    for (auto& image : image_map_) {
        image.second->Release();
    }
}