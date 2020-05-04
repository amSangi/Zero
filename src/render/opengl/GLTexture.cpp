#include "render/opengl/GLTexture.hpp"

using namespace zero::render;

GLTexture::GLTexture(GLuint id, GLenum target)
: id_(id)
, target_(target)
, name_()
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

std::string GLTexture::GetName() const {
    return name_;
}

void GLTexture::SetName(const std::string& name) {
    name_ = name;
}