#include "render/renderer/opengl/GLRenderTarget.hpp"
#include "render/renderer/opengl/GLTexture.hpp"

namespace zero::render
{

GLRenderTarget::GLRenderTarget()
, fbo_id_(0)
{
}

GLRenderTarget::~GLRenderTarget()
{
}

void GLRenderTarget::Initialize(IRenderTarget::Format format, uint32 width, uint32 height, std::shared_ptr<ITexture> texture)
{
    auto gl_texture = std::reinterpret_pointer_cast<GLTexture>(texture);
    glGenFramebuffers(1, &fbo_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gl_texture->GetNativeIdentifier(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GLRenderTarget::GetNativeIdentifier() const
{
    return fbo_id_;
}

} // namespace zero::render

