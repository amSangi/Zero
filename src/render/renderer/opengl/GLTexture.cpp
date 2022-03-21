#include "render/renderer/opengl/GLTexture.hpp"

namespace zero::render
{

GLTexture::GLTexture(GLuint id, GLenum target)
: id_(id)
, target_(target)
{
}

GLenum GLTexture::GetTarget() const
{
    return target_;
}

GLuint GLTexture::GetIdentifier() const
{
    return id_;
}

} // namespace zero::render