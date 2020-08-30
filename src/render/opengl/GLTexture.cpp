#include "render/opengl/GLTexture.hpp"

namespace zero::render
{

GLTexture::GLTexture(GLuint id, GLenum target)
: id_(id)
, target_(target)
{
}

GLTexture::~GLTexture()
{
    Cleanup();
}

GLenum GLTexture::GetTarget() const
{
    return target_;
}

GLuint GLTexture::GetNativeIdentifier() const
{
    return id_;
}

void GLTexture::Cleanup()
{
    glDeleteTextures(1, &id_);
}

} // namespace zero::render