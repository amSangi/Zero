#include "render/renderer/opengl/GLProgram.hpp"
#include "render/renderer/opengl/GLShader.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

std::shared_ptr<GLProgram> GLProgram::CreateGLProgram(const std::vector<std::shared_ptr<GLShader>>& shaders)
{

    if (shaders.empty())
    {
        return nullptr;
    }

    GLuint program_identifier = glCreateProgram();
    std::shared_ptr<GLProgram> program = std::make_shared<GLProgram>(program_identifier);

    for (const auto& shader : shaders)
    {
        if (!shader->IsCompiled())
        {
            return nullptr;
        }
        glAttachShader(program_identifier, shader->GetNativeIdentifier());
    }

    if (!program->Link())
    {
        std::string linker_error_message;
        GLint message_length = 0;
        glGetProgramiv(program_identifier, GL_INFO_LOG_LENGTH, &message_length);
        linker_error_message.resize(message_length);
        glGetProgramInfoLog(program_identifier, message_length, &message_length, linker_error_message.data());
        LOG_ERROR("GLProgram", "Failed to link OpenGL program. Error: " + linker_error_message);
        return nullptr;
    }

    for (const auto& shader : shaders)
    {
        glDetachShader(program_identifier, shader->GetNativeIdentifier());
    }

    return program;
}

GLProgram::GLProgram(GLuint id)
: id_(id)
, matrix4x4_map_()
, matrix3x3_map_()
, vec4f_map_()
, vec3f_map_()
, int32_map_()
, float_map_()
{
}

GLProgram::~GLProgram()
{
    Cleanup();
}

bool GLProgram::Link()
{
    if (IsLinked())
    {
        return true;
    }
    glLinkProgram(id_);
    return IsLinked();
}

bool GLProgram::IsLinked() const
{
    GLint result = GL_FALSE;
    glGetProgramiv(id_, GL_LINK_STATUS, &result);
    return (result == GL_TRUE);
}

void GLProgram::Use()
{
    glUseProgram(id_);
}

void GLProgram::Finish()
{
    glUseProgram(0);
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

void GLProgram::FlushUniform(const std::string& name, math::Matrix4x4 value)
{
    matrix4x4_map_[name] = value;
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_TRUE, &value[0][0]);
}

void GLProgram::FlushUniform(const std::string& name, math::Matrix3x3 value)
{
    matrix3x3_map_[name] = value;
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_TRUE, &value[0][0]);
}

void GLProgram::FlushUniform(const std::string& name, math::Vec4f value)
{
    vec4f_map_[name] = value;
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, value.Data());
}

void GLProgram::FlushUniform(const std::string& name, math::Vec3f value)
{
    vec3f_map_[name] = value;
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, value.Data());
}

void GLProgram::FlushUniform(const std::string& name, int32 value)
{
    int32_map_[name] = value;
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void GLProgram::FlushUniform(const std::string& name, float value)
{
    float_map_[name] = value;
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void GLProgram::FlushUniforms()
{
    // Flush Matrix4x4
    for (const auto& iter : matrix4x4_map_)
    {
        glUniformMatrix4fv(glGetUniformLocation(id_, iter.first.c_str()), 1, GL_TRUE, &iter.second[0][0]);
    }

    // Flush Matrix3x3
    for (const auto& iter : matrix3x3_map_)
    {
        glUniformMatrix3fv(glGetUniformLocation(id_, iter.first.c_str()), 1, GL_TRUE, &iter.second[0][0]);
    }

    // Flush Vec4f
    for (const auto& iter : vec4f_map_)
    {
        glUniform4fv(glGetUniformLocation(id_, iter.first.c_str()), 1, (iter.second).Data());
    }

    // Flush Vec3f
    for (const auto& iter : vec3f_map_)
    {
        glUniform3fv(glGetUniformLocation(id_, iter.first.c_str()), 1, (iter.second).Data());
    }

    // Flush int32
    for (const auto& iter : int32_map_)
    {
        glUniform1i(glGetUniformLocation(id_, iter.first.c_str()), iter.second);
    }

    // Flush float
    for (const auto& iter : float_map_)
    {
        glUniform1f(glGetUniformLocation(id_, iter.first.c_str()), iter.second);
    }
}

GLint GLProgram::GetAttribLocation(const std::string& name) const
{
    return glGetAttribLocation(id_, name.c_str());
}

GLuint GLProgram::GetUniformBlockIndex(const std::string& uniform_name) const
{
    return glGetUniformBlockIndex(id_, uniform_name.c_str());
}

void GLProgram::BindBlockIndex(GLuint block_index, GLuint block_binding) const
{
    glUniformBlockBinding(id_, block_index, block_binding);
}

void GLProgram::Cleanup() const
{
    if (glIsProgram(id_))
    {
        glDeleteProgram(id_);
    }
}

} // namespace zero::render