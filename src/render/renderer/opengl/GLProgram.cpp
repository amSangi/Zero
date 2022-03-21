#include "render/renderer/opengl/GLProgram.hpp"
#include "render/renderer/opengl/GLShader.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

GLProgram::GLProgram(GLuint program_id)
: program_id_(program_id)
, matrix4x4_map_()
, matrix3x3_map_()
, vec4f_map_()
, vec3f_map_()
, int32_map_()
, float_map_()
{
}

void GLProgram::SetUniform(const std::string& name, math::Matrix4x4 value)
{
    matrix4x4_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, math::Matrix3x3 value)
{
    matrix3x3_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, math::Vec4f value)
{
    vec4f_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, math::Vec3f value)
{
    vec3f_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, int32 value)
{
    int32_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, float value)
{
    float_map_[name] = value;
}

const GLProgram::UniformMap<math::Matrix4x4>& GLProgram::GetMatrix4x4Map() const
{
    return matrix4x4_map_;
}

const GLProgram::UniformMap<math::Matrix3x3>& GLProgram::GetMatrix3x3Map() const
{
    return matrix3x3_map_;
}

const GLProgram::UniformMap<math::Vec4f>& GLProgram::GetVec4fMap() const
{
    return vec4f_map_;
}

const GLProgram::UniformMap<math::Vec3f>& GLProgram::GetVec3fMap() const
{
    return vec3f_map_;
}

const GLProgram::UniformMap<int32>& GLProgram::GetInt32Map() const
{
    return int32_map_;
}

const GLProgram::UniformMap<float>& GLProgram::GetFloatMap() const
{
    return float_map_;
}

GLuint GLProgram::GetIdentifier() const
{
    return program_id_;
}

} // namespace zero::render