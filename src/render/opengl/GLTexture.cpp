#include "render/opengl/GLTexture.hpp"

using namespace zero::render;

/* ********** GLTexture Implementation ********** */

GLTexture::GLTexture(GLuint id, GLenum target)
: id_(id)
, target_(target) {}

GLTexture::~GLTexture() {
    Cleanup();
}

void GLTexture::Cleanup() {
    glDeleteTextures(1, &id_);
}

void GLTexture::Use(GLenum texture) {
    glBindTexture(target_, id_);
    glActiveTexture(texture);
}

/* ********** GLTextureBuilder Implementation ********** */

GLTextureBuilder::GLTextureBuilder(std::shared_ptr<Image> image, GLenum target)
: image_(std::move(image))
, target_(target)
, wrappings_{ ITexture::Wrapping::REPEAT, ITexture::Wrapping::REPEAT }
, min_filter_(ITexture::Filter::NEAREST)
, mag_filter_(ITexture::Filter::NEAREST)
, border_colour_(math::Vec3f::One()) {}

GLTextureBuilder& GLTextureBuilder::SetWrappingS(ITexture::Wrapping s) {
    wrappings_[0] = s;
    return *this;
}

GLTextureBuilder& GLTextureBuilder::SetWrappingT(ITexture::Wrapping t) {
    wrappings_[1] = t;
    return *this;
}

GLTextureBuilder& GLTextureBuilder::SetMinificationFilter(ITexture::Filter filter) {
    min_filter_ = filter;
    return *this;
}

GLTextureBuilder& GLTextureBuilder::SetMagnificationFilter(ITexture::Filter filter) {
    mag_filter_ = filter;
    return *this;
}

GLTextureBuilder& GLTextureBuilder::SetBorderColour(zero::math::Vec3f colour) {
    border_colour_ = colour;
    return *this;
}

std::shared_ptr<GLTexture> GLTextureBuilder::Build() {
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(target_, texture_id);

    // Set texture wrapping
    switch (wrappings_[0])
    {
        case ITexture::Wrapping::CLAMP_TO_EDGE:
            glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            break;
        case ITexture::Wrapping::CLAMP_TO_BORDER:
            glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            break;
        case ITexture::Wrapping::MIRRORED_REPEAT:
            glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            break;
        default:
            glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_REPEAT);
            break;
    }

    switch (wrappings_[1])
    {
        case ITexture::Wrapping::CLAMP_TO_EDGE:
            glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        case ITexture::Wrapping::CLAMP_TO_BORDER:
            glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            break;
        case ITexture::Wrapping::MIRRORED_REPEAT:
            glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        default:
            glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
    }

    // Set texture filtering
    bool generate_mipmap = true;
    switch (min_filter_)
    {
        case ITexture::Filter::LINEAR:
            glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            generate_mipmap = false;
            break;
        case ITexture::Filter::NEAREST_MIPMAP_NEAREST:
            glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case ITexture::Filter::LINEAR_MIPMAP_NEAREST:
            glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case ITexture::Filter::NEAREST_MIPMAP_LINEAR:
            glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        case ITexture::Filter::LINEAR_MIPMAP_LINEAR:
            glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        default:
            glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            generate_mipmap = false;
            break;
    }

    switch (mag_filter_)
    {
        case ITexture::Filter::LINEAR:
            glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        default:
            glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
    }

    // Load image
    if (!image_->Load()) {
        glDeleteTextures(1, &texture_id);
        return nullptr;
    }

    // Create texture
    GLenum data_format;
    switch (image_->GetPixelFormat())
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

    glTexImage2D(target_,              // Target texture.
                 0,                    // Level of detail. Level n is the nth mipmap reduction image.
                 GL_RGB,               // Number of colour components in the texture.
                 image_->GetWidth(),   // Texture image width.
                 image_->GetHeight(),  // Texture image height.
                 0,                    // Border. Must be 0.
                 data_format,          // Format of the pixel data.
                 GL_UNSIGNED_BYTE,     // Data type of the pixel data.
                 image_->GetData());   // Pointer to the image data in memory.

    // Generate mipmaps
    if (generate_mipmap) {
        glGenerateMipmap(target_);
    }

    return std::make_shared<GLTexture>(texture_id, target_);
}
