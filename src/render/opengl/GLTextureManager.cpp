#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"
#include "component/Material.hpp"
#include "render/Image.hpp"

namespace zero::render
{

GLTextureManager::GLTextureManager()
: image_map_()
, shadow_map_textures_()
{
}

uint8 GLTextureManager::GetTextureUnitCount() const
{
    GLint count = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &count);
    return count;
}

void GLTextureManager::SetSampler(const std::shared_ptr<GLSampler>& sampler, uint8 index) const
{
    if (index >= GetTextureUnitCount() || !sampler)
    {
        return;
    }
    glBindSampler(index, sampler->GetNativeIdentifier());
}

bool GLTextureManager::InitializeImage(const std::string& image_name, const std::string& filename)
{
    if (image_map_.find(image_name) != image_map_.end())
    {
        return true;
    }

    auto image = std::make_shared<Image>(filename);
    if (!image->Load())
    {
        return false;
    }

    image->Release();
    image_map_.emplace(image_name, std::move(image));
    return true;
}

bool GLTextureManager::InitializeGLTextures()
{
    for (const auto& image_pair : image_map_)
    {
        const std::string& image_name = image_pair.first;
        std::shared_ptr<Image> image = image_pair.second;

        std::shared_ptr<GLTexture> gl_texture = CreateTexture(image);

        if (gl_texture == nullptr)
        {
            return false;
        }
        gl_texture_map_.emplace(image_name, gl_texture);
    }

    // Create 1x1 black texture for unused samplers
    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    GLubyte data[] = {0, 0, 0, 0};
    glTexImage2D(GL_TEXTURE_2D,       // Target texture.
                 0,                   // Level of detail. Level n is the nth mipmap reduction image.
                 GL_RGBA,             // Number of colour components in the texture.
                 1,                   // Texture image width.
                 1,                   // Texture image height.
                 0,                   // Border. Must be 0.
                 GL_RGBA,             // Format of the pixel data.
                 GL_UNSIGNED_BYTE,    // Data type of the pixel data.
                 data);               // Pointer to the image data in memory.
    empty_texture_ = std::make_shared<GLTexture>(texture_id, GL_TEXTURE_2D);
    return true;
}

std::shared_ptr<GLTexture> GLTextureManager::GetGLTexture(const std::string& image_name) const
{
    auto search = gl_texture_map_.find(image_name);
    if (search == gl_texture_map_.end())
    {
        return empty_texture_;
    }
    return search->second;
}

const std::vector<std::shared_ptr<GLTexture>>& GLTextureManager::GetShadowMapTextures() const
{
    return shadow_map_textures_;
}

void GLTextureManager::SetShadowMapTextures(const std::vector<std::shared_ptr<GLTexture>>& textures)
{
    shadow_map_textures_ = textures;
}

void GLTextureManager::UnloadImages()
{
    for (const auto& image_pair : image_map_)
    {
        image_pair.second->Release();
    }
}

void GLTextureManager::UnloadGLTextures()
{
    gl_texture_map_.clear();
}

std::shared_ptr<GLTexture> GLTextureManager::CreateTexture(std::shared_ptr<Image> image)
{
    GLenum texture_unit = GL_TEXTURE0;
    GLenum target = GL_TEXTURE_2D;

    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glActiveTexture(texture_unit);
    glBindTexture(target, texture_id);

    // Load image
    if (!image->Load())
    {
        glDeleteTextures(1, &texture_id);
        return nullptr;
    }

    // Create texture
    GLenum data_format = GL_RGB;
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

    glGenerateMipmap(target);

    // Unbind the texture
    glActiveTexture(texture_unit);
    glBindTexture(target, 0);

    return std::make_shared<GLTexture>(texture_id, target);
}

} // namespace zero::render