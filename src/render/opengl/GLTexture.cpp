#include "render/opengl/GLTexture.hpp"

using namespace zero::render;

GLTexture::GLTexture(GLuint id, GLenum target)
: id_(id)
, target_(target)
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