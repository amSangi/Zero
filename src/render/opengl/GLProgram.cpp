#include <GL/glew.h>
#include "render/opengl/GLProgram.hpp"

using namespace zero::render;

std::shared_ptr<GLProgram> GLProgram::CreateGLProgram(
        const std::vector<std::shared_ptr<zero::render::GLShader>>& shaders) {

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

GLProgram::GLProgram(GLuint identifier) : identifier_(identifier) {}

GLProgram::~GLProgram() {
    Cleanup();
}

bool GLProgram::Link() {
    glLinkProgram(identifier_);
    return IsLinked();
}

bool GLProgram::IsLinked() const {
    GLint result = GL_FALSE;
    glGetProgramiv(identifier_, GL_LINK_STATUS, &result);
    return (result == GL_TRUE);
}

void GLProgram::Use() {
    glUseProgram(identifier_);
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

GLuint GLProgram::GetNativeIdentifier() {
    return identifier_;
}

void GLProgram::Cleanup() {
    if (glIsProgram(identifier_)) {
        glDeleteProgram(identifier_);
    }
}
