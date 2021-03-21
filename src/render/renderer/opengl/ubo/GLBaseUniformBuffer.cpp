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

void GLBaseUniformBuffer::InitializeBaseBuffer(uint32 binding_index, uint32 data_size)
{
    binding_index_ = binding_index;
    glGenBuffers(1, &buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    glBufferData(GL_UNIFORM_BUFFER, data_size, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding_index_, buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    initialized_ = true;
}

} // namespace zero::render

