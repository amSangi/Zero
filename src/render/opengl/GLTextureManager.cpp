#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"
#include "component/Material.hpp"
#include "render/Image.hpp"

namespace zero::render
{

constexpr zero::uint8 kDiffuseTextureIndex = 0;
constexpr zero::uint8 kSpecularTextureIndex = 1;
constexpr zero::uint8 kNormalTextureIndex = 2;

constexpr auto kDiffuseTextureUniformName = "diffuse_texture";
constexpr auto kSpecularTextureUniformName = "specular_texture";
constexpr auto kNormalTextureUniformName = "normal_texture";

GLTextureManager::GLTextureManager()
: image_map_()
{
}

zero::uint8 GLTextureManager::GetTextureUnitCount() const
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

std::vector<std::shared_ptr<GLTexture>> GLTextureManager::CreateTextureMap(const Material& material)
{
    std::vector<std::shared_ptr<GLTexture>> gl_textures;
    auto gl_diffuse_texture = CreateTexture(material.texture_map_.diffuse_map_, kDiffuseTextureIndex, kDiffuseTextureUniformName);
    auto gl_specular_texture = CreateTexture(material.texture_map_.specular_map_, kSpecularTextureIndex, kSpecularTextureUniformName);
    auto gl_normal_texture = CreateTexture(material.texture_map_.normal_map_, kNormalTextureIndex, kNormalTextureUniformName);
    if (gl_diffuse_texture)
    {
        gl_textures.push_back(gl_diffuse_texture);
    }
    if (gl_specular_texture)
    {
        gl_textures.push_back(gl_specular_texture);
    }
    if (gl_normal_texture)
    {
        gl_textures.push_back(gl_normal_texture);
    }
    return gl_textures;
}


std::shared_ptr<GLTexture> GLTextureManager::CreateTexture(const std::string& image_name,
                                                           uint8 index,
                                                           const std::string& name)
{
    if (index >= GetTextureUnitCount())
    {
        return nullptr;
    }

    auto search = image_map_.find(image_name);
    if (search == image_map_.end())
    {
        return nullptr;
    }

    GLenum texture_unit = GL_TEXTURE0 + index;
    GLenum target = GL_TEXTURE_2D;

    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glActiveTexture(texture_unit);
    glBindTexture(target, texture_id);

    // Load image
    auto image = search->second;
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

    auto gl_texture = std::make_shared<GLTexture>(texture_id, target);
    gl_texture->SetName(name);
    return gl_texture;
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

void GLTextureManager::ClearImages()
{
    image_map_.clear();
}

void GLTextureManager::UnloadImages()
{
    for (auto& image : image_map_)
    {
        image.second->Release();
    }
}

} // namespace zero::render