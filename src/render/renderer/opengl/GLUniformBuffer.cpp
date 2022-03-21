#include "render/renderer/opengl/GLUniformBuffer.hpp"

namespace zero::render
{

GLUniformBuffer::GLUniformBuffer(GLuint ubo_id, uint32 buffer_size, std::string name)
: ubo_id_(ubo_id)
, buffer_size_(buffer_size)
, name_(std::move(name))
{
}

uint32 GLUniformBuffer::GetSize()
{
    return buffer_size_;
}

const std::string& GLUniformBuffer::GetName()
{
    return name_;
}

GLuint GLUniformBuffer::GetIdentifier() const
{
    return ubo_id_;
}

} // namespace zero::render