#include "render/renderer/opengl/GLTextureManager.hpp"
#include "component/Material.hpp"
#include "render/Constants.hpp"
#include "render/renderer/opengl/GLRenderTarget.hpp"

namespace zero::render
{

GLTextureManager::GLTextureManager()
: texture_map_()
, shadow_map_render_targets_()
, shadow_map_textures_()
, diffuse_map_sampler_(nullptr)
, shadow_map_sampler_(nullptr)
, empty_texture_(nullptr)
{
}

GLTextureManager::~GLTextureManager()
{
}

uint32 GLTextureManager::GetTextureUnitCount() const
{
    GLint count = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &count);
    return static_cast<uint32>(count);
}

void GLTextureManager::Initialize()
{
    SetupEmptyTexture();
    SetupSamplers();
    SetupShadowMapTextures();
    SetupShadowMapRenderTargets();
}

void GLTextureManager::SetupEmptyTexture()
{
    // Create 1x1 black texture for unused/empty/default textures
    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    GLubyte data[] = {0, 0, 0, 0};
    glTexImage2D(GL_TEXTURE_2D,     // Target texture)
                 0,                 // Level of detail. Level n is the nth mipmap reduction image.
                 GL_RGBA,           // Number of color components in the texture.
                 1,                 // Texture image width
                 1,                 // Texture image height
                 0,                 // Border. Must be 0.
                 GL_RGBA,           // Format of the pixel data
                 GL_UNSIGNED_BYTE,  // Data type of the pixel data
                 data);             // Pointer to the image data in memory

    empty_texture_ = std::make_shared<GLTexture>(texture_id, GL_TEXTURE_2D);
}

void GLTextureManager::SetupSamplers()
{
    diffuse_map_sampler_ = std::make_shared<GLSampler>();
    diffuse_map_sampler_->Initialize();
    diffuse_map_sampler_->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    diffuse_map_sampler_->SetMagnificationFilter(ISampler::Filter::LINEAR);
    diffuse_map_sampler_->SetWrappingS(ISampler::Wrapping::REPEAT);
    diffuse_map_sampler_->SetWrappingT(ISampler::Wrapping::REPEAT);

    shadow_map_sampler_ = std::make_shared<GLSampler>();
    shadow_map_sampler_->Initialize();
    shadow_map_sampler_->SetMinificationFilter(ISampler::Filter::LINEAR);
    shadow_map_sampler_->SetMagnificationFilter(ISampler::Filter::LINEAR);
    shadow_map_sampler_->SetWrappingS(ISampler::Wrapping::CLAMP_TO_BORDER);
    shadow_map_sampler_->SetWrappingT(ISampler::Wrapping::CLAMP_TO_BORDER);
    shadow_map_sampler_->SetBorderColour(math::Vec4f{1.0F, 1.0F, 1.0F, 1.0F});
    glSamplerParameteri(shadow_map_sampler_->GetNativeIdentifier(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glSamplerParameteri(shadow_map_sampler_->GetNativeIdentifier(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
}

void GLTextureManager::SetupShadowMapTextures()
{
    for (uint32 i = 0; i < Constants::kShadowCascadeCount; ++i)
    {
        GLuint shadow_map_texture_id = 0;
        glGenTextures(1, &shadow_map_texture_id);
        glBindTexture(GL_TEXTURE_2D, shadow_map_texture_id);
        glTexImage2D(GL_TEXTURE_2D,                  // Target texture.
                     0,                              // Level of detail. Level n is the nth mipmap reduction image.
                     GL_DEPTH_COMPONENT,             // Number of colour components in the texture.
                     Constants::kShadowMapWidth,     // Texture image width.
                     Constants::kShadowMapHeight,    // Texture image height.
                     0,                              // Border. Must be 0.
                     GL_DEPTH_COMPONENT,             // Format of the pixel data.
                     GL_FLOAT,                       // Data type of the pixel data.
                     nullptr);                       // Pointer to the image data in memory.

        auto texture = std::make_shared<GLTexture>(shadow_map_texture_id, GL_TEXTURE_2D);
        shadow_map_textures_.push_back(std::static_pointer_cast<ITexture>(texture));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTextureManager::SetupShadowMapRenderTargets()
{
    assert(shadow_map_textures_.size() > 0);

    for (uint32 i = 0; i < Constants::kShadowCascadeCount; ++i)
    {
        auto shadow_map_render_target = std::make_shared<GLRenderTarget>();
        shadow_map_render_target->Initialize(IRenderTarget::Format::DEPTH,
                                             Constants::kShadowMapWidth,
                                             Constants::kShadowMapHeight,
                                             shadow_map_textures_[i]);
        shadow_map_render_targets_.push_back(std::static_pointer_cast<IRenderTarget>(shadow_map_render_target));
    }
}

bool GLTextureManager::LoadTexture(const std::string& image_name, const std::string& filename)
{
    Image image{filename};
    if (!image.Load())
    {
        return false;
    }

    GLenum texture_unit = GL_TEXTURE0;
    GLenum target = GL_TEXTURE_2D;

    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glActiveTexture(texture_unit);
    glBindTexture(target, texture_id);
    GLenum data_format = GL_RGB;

    switch (image.GetPixelFormat())
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
            return false;
    }

    glTexImage2D(GL_TEXTURE_2D,     // Target texture)
                 0,                 // Level of detail. Level n is the nth mipmap reduction image.
                 GL_RGBA,           // Number of color components in the texture.
                 image.GetWidth(),  // Texture image width
                 image.GetHeight(), // Texture image height
                 0,                 // Border. Must be 0.
                 data_format,       // Format of the pixel data
                 GL_UNSIGNED_BYTE,  // Data type of the pixel data
                 image.GetData());  // Pointer to the image data in memory

    glGenerateMipmap(target);

    glActiveTexture(texture_unit);
    glBindTexture(target, 0);

    auto gl_texture = std::make_shared<GLTexture>(texture_id, target);
    texture_map_.emplace(image_name, gl_texture);
    return true;
}

void GLTextureManager::ReleaseTextures()
{
    texture_map_.clear();
}

ITexture* GLTextureManager::GetTexture(const std::string& image_name)
{
    auto search = texture_map_.find(image_name);
    if (search == texture_map_.end())
    {
        return empty_texture_.get();
    }
    return search->second.get();
}

std::shared_ptr<ISampler> GLTextureManager::GetDiffuseMapSampler()
{
    return std::static_pointer_cast<ISampler>(diffuse_map_sampler_);
}

std::shared_ptr<ISampler> GLTextureManager::GetShadowMapSampler()
{
    return std::static_pointer_cast<ISampler>(shadow_map_sampler_);
}

const std::vector<std::shared_ptr<IRenderTarget>>& GLTextureManager::GetShadowMapRenderTargets()
{
    return shadow_map_render_targets_;
}

const std::vector<std::shared_ptr<ITexture>>& GLTextureManager::GetShadowMapTextures()
{
    return shadow_map_textures_;
}

} // namespace zero::render