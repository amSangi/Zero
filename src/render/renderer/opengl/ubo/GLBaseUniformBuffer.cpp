#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"

namespace zero::render
{

GLBaseUniformBuffer::GLBaseUniformBuffer(const std::string& name)
: name_(name)
, buffer_id_(0)
, binding_index_(0)
, initialized_(false)
{
}

GLBaseUniformBuffer::~GLBaseUniformBuffer()
{
    if (initialized_)
    {
        glDeleteBuffers(1, &buffer_id_);
    }
}

uint32 GLBaseUniformBuffer::GetBindingIndex() const
{
    return binding_index_;
}

const std::string& GLBaseUniformBuffer::GetName() const
{
    return name_;
}

} // namespace zero::render

