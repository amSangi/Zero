#include "render/renderer/opengl/GLFrameBuffer.hpp"

namespace zero::render
{

GLFrameBuffer::GLFrameBuffer(GLuint fbo_id)
: fbo_id_(fbo_id)
{
}

GLuint GLFrameBuffer::GetIdentifier() const
{
    return fbo_id_;
}

} // namespace zero::render