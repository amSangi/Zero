#include "render/opengl/GLTexture.hpp"

using namespace zero::render;

GLTexture::GLTexture(GLuint id, GLenum target)
: id_(id)
, target_(target)
, uniform_name_()
{}

GLTexture::~GLTexture() {
    Cleanup();
}

void GLTexture::Cleanup() {
    glDeleteTextures(1, &id_);
}

void GLTexture::Bind(GLenum texture_unit) const {
    glActiveTexture(texture_unit);
    glBindTexture(target_, id_);
}

void GLTexture::GenerateMipMap(GLenum texture_unit) const {
    Bind(texture_unit);
    glGenerateMipmap(target_);
    glBindTexture(target_, 0);
}

GLenum GLTexture::GetTarget() const {
    return target_;
}

GLuint GLTexture::GetIdentifier() const {
    return id_;
}

std::string GLTexture::GetUniformName() const {
    return uniform_name_;
}

void GLTexture::SetUniformName(const std::string& uniform_name) {
    uniform_name_ = uniform_name;
}