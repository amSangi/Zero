#include "render/renderer/opengl/GLShader.hpp"

namespace zero::render
{

GLShader::GLShader(GLuint shader_id, IShader::Type type)
: id_(shader_id)
, type_(type)
{
}

IShader::Type GLShader::GetType() const
{
    return type_;
}

GLuint GLShader::GetIdentifier() const
{
    return id_;
}
} // namespace zero::render