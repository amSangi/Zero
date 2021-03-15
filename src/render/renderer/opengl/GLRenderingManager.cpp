#include "render/renderer/opengl/GLRenderingManager.hpp"

namespace zero::render
{

GLRenderingManager::GLRenderingManager()
: gl_model_manager_(std::make_unique<GLModelManager>())
, gl_rendering_context_(std::make_unique<GLRenderingContext>())
, gl_shader_manager_(std::make_unique<GLShaderManager>())
, gl_texture_manager_(std::make_unique<GLTextureManager>())
, gl_uniform_manager_(std::make_unique<GLUniformManager>())
{
}

IModelManager* GLRenderingManager::GetModelManager()
{
    return gl_model_manager_.get();
}

IRenderingContext* GLRenderingManager::GetRenderingContext()
{
    return gl_rendering_context_.get();
}

IShaderManager* GLRenderingManager::GetShaderManager()
{
    return gl_shader_manager_.get();
}

ITextureManager* GLRenderingManager::GetTextureManager()
{
    return gl_texture_manager_.get();
}

IUniformManager* GLRenderingManager::GetUniformManager()
{
    return gl_uniform_manager_.get();
}


} // namespace zero::render

