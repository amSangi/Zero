#include "GL/glew.h"
#include "render/opengl/GLTexture.hpp"

using namespace zero::render;

GLTexture::GLTexture(GLuint id, GLenum target, GLenum texture_unit)
: id_(id)
, target_(target)
, texture_unit_(texture_unit)
{}

GLTexture::~GLTexture() {
    Cleanup();
}

void GLTexture::Cleanup() {
    glDeleteTextures(1, &id_);
}

void GLTexture::Use() const {
    glActiveTexture(texture_unit_);
    glBindTexture(target_, id_);
}

void GLTexture::GenerateMipMap() const {
    Use();
    glGenerateMipmap(target_);
}

void GLTexture::SetTextureUnit(GLenum texture_unit) {
    texture_unit_ = texture_unit;
}

GLuint GLTexture::GetNativeIdentifier() const {
    return id_;
}

GLenum GLTexture::GetTarget() const {
    return target_;
}

GLenum GLTexture::GetTextureUnit() const {
    return texture_unit_;
}