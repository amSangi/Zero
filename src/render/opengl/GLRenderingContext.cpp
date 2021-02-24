#include "render/opengl/GLRenderingContext.hpp"
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

void GLRenderingContext::Shutdown()
{

}

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

void GLRenderingContext::BeginFrame()
{

}

void GLRenderingContext::EndFrame()
{

}

void GLRenderingContext::BeginCamera(const Camera& camera)
{

}

void GLRenderingContext::EndCamera()
{

}

void GLRenderingContext::BindShaderProgram()
{

}

void GLRenderingContext::BindUniformBuffer(uint32 index, IUniformBuffer* uniform_buffer)
{

}

void GLRenderingContext::BindTextureSampler(uint32 index, ISampler* sampler)
{

}

void GLRenderingContext::BindTexture(uint32 index, ITexture* texture)
{

}

void GLRenderingContext::BindMaterial(const Material& material)
{

}

void GLRenderingContext::Draw(const Mesh& mesh)
{

}


} // namespace zero::render

