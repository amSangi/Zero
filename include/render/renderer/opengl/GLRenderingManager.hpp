#pragma once

#include "render/renderer/IRenderingManager.hpp"
#include "render/renderer/opengl/GLModelManager.hpp"
#include "render/renderer/opengl/GLRenderingContext.hpp"
#include "render/renderer/opengl/GLShaderManager.hpp"
#include "render/renderer/opengl/GLTextureManager.hpp"
#include "render/renderer/opengl/GLUniformManager.hpp"

namespace zero::render
{

    class GLRenderingManager final : public IRenderingManager
    {
    public:
        GLRenderingManager();
        ~GLRenderingManager() override = default;
        IModelManager* GetModelManager() override;
        IRenderingContext* GetRenderingContext() override;
        IShaderManager* GetShaderManager() override;
        ITextureManager* GetTextureManager() override;
        IUniformManager* GetUniformManager() override;
    private:
        std::unique_ptr<GLModelManager> gl_model_manager_;
        std::unique_ptr<GLRenderingContext> gl_rendering_context_;
        std::unique_ptr<GLShaderManager> gl_shader_manager_;
        std::unique_ptr<GLTextureManager> gl_texture_manager_;
        std::unique_ptr<GLUniformManager> gl_uniform_manager_;
    };

} // namespace zero::render