#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"

using namespace zero::render;

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

std::shared_ptr<GLTexture> GLTextureManager::CreateTexture(const std::string& filename, zero::uint8 index) {
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

    return std::make_shared<GLTexture>(texture_id, target);
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