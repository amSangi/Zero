
#include "render/opengl/GLProgram.hpp"

using namespace zero::render;

bool GLProgram::Link() {
    return false;
}

bool GLProgram::Use() {
    return false;
}

bool GLProgram::Finish() {
    return false;
}

void GLProgram::SetUniform(const std::string& name, math::Matrix4x4 value) {

}

void GLProgram::SetUniform(const std::string& name, math::Matrix3x3 value) {

}

void GLProgram::SetUniform(const std::string& name, math::Vec4f value) {

}

void GLProgram::SetUniform(const std::string& name, math::Vec3f value) {

}

void GLProgram::SetUniform(const std::string& name, uint32 value) {

}

void GLProgram::SetUniform(const std::string& name, int32 value) {

}

void GLProgram::SetUniform(const std::string& name, float value) {

}
