#include "GL/glew.h"
#include "render/opengl/GLTextureManager.hpp"

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

bool GLTextureManager::InitializeImage(const std::string& filename) {
    auto image = std::make_shared<Image>(filename);

    if (!image->Load()) {
        return false;
    }

    image->Release();
    image_map_[filename] = image;
    return true;
}

std::shared_ptr<GLTexture> GLTextureManager::CreateTexture(const std::string& filename, zero::uint8 index) {
    return std::shared_ptr<GLTexture>(CreateRawTexture(filename, index));
}

GLTexture* GLTextureManager::CreateRawTexture(const std::string& filename, zero::uint8 index) {
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
        case Image::PixelFormat::PIXEL_FORMAT_BGR:
            data_format = GL_BGR;
            break;
        case Image::PixelFormat::PIXEL_FORMAT_RGB:
            data_format = GL_RGB;
            break;
        case Image::PixelFormat::PIXEL_FORMAT_RGBA:
            data_format = GL_RGBA;
            break;
        case Image::PixelFormat::PIXEL_FORMAT_BGRA:
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

    return new GLTexture(texture_id, target, texture_unit);
}