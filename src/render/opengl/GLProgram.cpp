#include "render/opengl/GLProgram.hpp"

using namespace zero::render;

std::shared_ptr<GLProgram> GLProgram::CreateGLProgram(
        const std::vector<std::shared_ptr<zero::render::GLShader>>& shaders) {

    if (shaders.empty()) return nullptr;

    GLuint program_identifier = glCreateProgram();
    std::shared_ptr<GLProgram> program = std::make_shared<GLProgram>(program_identifier);

    for (auto& shader : shaders) {
        if (!shader->IsCompiled()) return nullptr;
        glAttachShader(program_identifier, shader->GetNativeIdentifier());
    }

    if (!program->Link()) return nullptr;

    for (auto& shader : shaders) {
        glDetachShader(program_identifier, shader->GetNativeIdentifier());
    }

    return program;
}

GLProgram::GLProgram(GLuint id) : id_(id) {}

GLProgram::~GLProgram() {
    Cleanup();
}

bool GLProgram::Link() {
    if (IsLinked()) return true;
    glLinkProgram(id_);
    return IsLinked();
}

bool GLProgram::IsLinked() const {
    GLint result = GL_FALSE;
    glGetProgramiv(id_, GL_LINK_STATUS, &result);
    return (result == GL_TRUE);
}

void GLProgram::Use() {
    glUseProgram(id_);
}

void GLProgram::Finish() {
    glUseProgram(0);
}

void GLProgram::SetUniform(const std::string& name, math::Matrix4x4 value) {
    matrix4x4_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, math::Matrix3x3 value) {
    matrix3x3_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, math::Vec4f value) {
    vec4f_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, math::Vec3f value) {
    vec3f_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, uint32 value) {
    uint32_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, int32 value) {
    int32_map_[name] = value;
}

void GLProgram::SetUniform(const std::string& name, float value) {
    float_map_[name] = value;
}

void GLProgram::FlushUniform(const std::string& name, math::Matrix4x4 value) {
    matrix4x4_map_[name] = value;
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, value[0]);
}

void GLProgram::FlushUniform(const std::string& name, math::Matrix3x3 value) {
    matrix3x3_map_[name] = value;
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, value[0]);
}

void GLProgram::FlushUniform(const std::string& name, math::Vec4f value) {
    vec4f_map_[name] = value;
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, value.Data());
}

void GLProgram::FlushUniform(const std::string& name, math::Vec3f value) {
    vec3f_map_[name] = value;
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, value.Data());
}

void GLProgram::FlushUniform(const std::string& name, uint32 value) {
    uint32_map_[name] = value;
    glUniform1ui(glGetUniformLocation(id_, name.c_str()), value);
}

void GLProgram::FlushUniform(const std::string& name, int32 value) {
    int32_map_[name] = value;
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void GLProgram::FlushUniform(const std::string& name, float value) {
    float_map_[name] = value;
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void GLProgram::FlushUniforms() {
    // Flush Matrix4x4
    for (const auto& iter : matrix4x4_map_) {
        glUniformMatrix4fv(glGetUniformLocation(id_, iter.first.c_str()), 1, GL_FALSE, iter.second[0]);
    }

    // Flush Matrix3x3
    for (const auto& iter : matrix3x3_map_) {
        glUniformMatrix3fv(glGetUniformLocation(id_, iter.first.c_str()), 1, GL_FALSE, iter.second[0]);
    }

    // Flush Vec4f
    for (const auto& iter : vec4f_map_) {
        glUniform4fv(glGetUniformLocation(id_, iter.first.c_str()), 1, (iter.second).Data());
    }

    // Flush Vec3f
    for (const auto& iter : vec3f_map_) {
        glUniform3fv(glGetUniformLocation(id_, iter.first.c_str()), 1, (iter.second).Data());
    }

    // Flush uint32
    for (const auto& iter : uint32_map_) {
        glUniform1ui(glGetUniformLocation(id_, iter.first.c_str()), iter.second);
    }

    // Flush int32
    for (const auto& iter : int32_map_) {
        glUniform1i(glGetUniformLocation(id_, iter.first.c_str()), iter.second);
    }

    // Flush float
    for (const auto& iter : float_map_) {
        glUniform1f(glGetUniformLocation(id_, iter.first.c_str()), iter.second);
    }
}

GLint GLProgram::GetAttribLocation(const std::string& name) const {
    return glGetAttribLocation(id_, name.c_str());
}

void GLProgram::Cleanup() {
    if (glIsProgram(id_)) {
        glDeleteProgram(id_);
    }
}
