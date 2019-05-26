#include "render/opengl/GLShader.hpp"

using namespace zero::render;

GLShader::GLShader(const std::string& source) {

}

GLShader::~GLShader() {
    Cleanup();
}

bool GLShader::Compile() {
    return false;
}

bool GLShader::IsCompiled() const {
    return false;
}

IShader::Type GLShader::GetType() const {
    return IShader::Type::TYPE_VERTEX_SHADER;
}

GLuint GLShader::GetNativeIdentifier() {
    return 0;
}

void GLShader::Cleanup() {

}