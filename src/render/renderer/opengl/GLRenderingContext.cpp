#include "render/renderer/opengl/GLMesh.hpp"
#include "render/renderer/opengl/GLRenderingContext.hpp"
#include "render/renderer/opengl/GLProgram.hpp"
#include "render/renderer/opengl/GLRenderTarget.hpp"
#include "render/renderer/opengl/GLSampler.hpp"
#include "render/renderer/opengl/GLTexture.hpp"
#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

void GLMessageCallback(GLenum /* source */,
                       GLenum type,
                       GLuint /* id */,
                       GLenum /* severity */,
                       GLsizei length,
                       const GLchar* message,
                       const void* /* user_param */)
{
    constexpr auto kOpenGLTitle = "GLMessageCallback";

    if (type == GL_DEBUG_TYPE_ERROR)
    {
        LOG_ERROR(kOpenGLTitle, std::string(message, length));
    }
    else
    {
        LOG_DEBUG(kOpenGLTitle, std::string(message, length));
    }
}

GLRenderingContext::GLRenderingContext()
: current_shader_program_(nullptr)
{
}

GLRenderingContext::~GLRenderingContext()
{
}

//////////////////////////////////////////////////
////////// Initialization and Shutdown
//////////////////////////////////////////////////

constexpr auto kTitle = "GLRenderingContext";

void GLRenderingContext::Initialize(const RenderSystemConfig& config)
{
    LOG_VERBOSE(kTitle, "Initializing GLRenderingContext");

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

#if LOGGING_ENABLED
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLMessageCallback, nullptr);
#endif
}

//////////////////////////////////////////////////
////////// Setters and Clear Methods
//////////////////////////////////////////////////

void GLRenderingContext::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
    glViewport(x, y, width, height);
}

void GLRenderingContext::SetFillMode(FillMode fill_mode)
{
    switch (fill_mode)
    {
        case FillMode::WIREFRAME:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case FillMode::SOLID:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
}

void GLRenderingContext::SetCullMode(CullMode cull_mode)
{
    switch (cull_mode)
    {
        case CullMode::CULL_NONE:
            glDisable(GL_CULL_FACE);
            break;
        case CullMode::CULL_FRONT:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case CullMode::CULL_BACK:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
    }
}

void GLRenderingContext::SetClearColor(const math::Vec4f& color)
{
    glClearColor(color.x_, color.y_, color.z_, color.w_);
}

void GLRenderingContext::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLRenderingContext::ClearColor()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderingContext::ClearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void GLRenderingContext::ClearStencil()
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

//////////////////////////////////////////////////
////////// Render related Methods
//////////////////////////////////////////////////

void GLRenderingContext::BeginFrame(IRenderTarget* render_target)
{
    GLRenderTarget* gl_render_target = static_cast<GLRenderTarget*>(render_target);
    GLuint fbo_id = 0;
    if (gl_render_target)
    {
        fbo_id = gl_render_target->GetNativeIdentifier();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
}

void GLRenderingContext::EndFrame()
{
    // Unbind any existing frame buffers
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    current_shader_program_ = nullptr;
}

void GLRenderingContext::BindShaderProgram(IProgram* shader_program)
{
    current_shader_program_ = reinterpret_cast<GLProgram*>(shader_program);
    current_shader_program_->Use();
}

void GLRenderingContext::BindUniformBuffer(IUniformBuffer* uniform_buffer)
{
    GLBaseUniformBuffer* gl_uniform_buffer = static_cast<GLBaseUniformBuffer*>(uniform_buffer);
    glUniformBlockBinding(current_shader_program_->GetId(),
                          current_shader_program_->GetUniformBlockIndex(gl_uniform_buffer->GetName()),
                          gl_uniform_buffer->GetBindingIndex());
}

void GLRenderingContext::BindTextureSampler(uint32 index, ISampler* sampler)
{
    GLSampler* gl_sampler = static_cast<GLSampler*>(sampler);
    glBindSampler(index, gl_sampler->GetNativeIdentifier());
}

void GLRenderingContext::BindTexture(uint32 index, ITexture* texture)
{
    GLTexture* gl_texture = static_cast<GLTexture*>(texture);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(gl_texture->GetTarget(), gl_texture->GetNativeIdentifier());
}

void GLRenderingContext::Draw(IMesh* mesh)
{
    GLMesh* gl_mesh = static_cast<GLMesh*>(mesh);
    glBindVertexArray(gl_mesh->GetVAO());
    glDrawElements(GL_TRIANGLES, gl_mesh->GetIndexDataSize(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


} // namespace zero::render

