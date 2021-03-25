#pragma once

#include "render/renderer/IRenderingContext.hpp"
#include "render/renderer/opengl/GLProgram.hpp"

namespace zero::render
{

    class GLRenderingContext final : public IRenderingContext
    {
    public:
        GLRenderingContext();
        ~GLRenderingContext() override;

        void Initialize(const RenderSystemConfig& config) override;

        void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
        void SetFillMode(FillMode fill_mode) override;
        void SetCullMode(CullMode cull_mode) override;

        void SetClearColor(const math::Vec4f& color) override;
        void Clear() override;
        void ClearColor() override;
        void ClearDepth() override;
        void ClearStencil() override;

        void BeginFrame(IRenderTarget* render_target) override;
        void EndFrame() override;

        void BindShaderProgram(IProgram* shader_program) override;
        void BindUniformBuffer(IUniformBuffer* uniform_buffer) override;
        void BindTextureSampler(uint32 index, ISampler* sampler) override;
        void BindTexture(uint32 index, ITexture* texture) override;

        void Draw(IMesh* mesh) override;

    private:
        uint32 largest_texture_unit_index_;
        GLProgram* current_shader_program_;

    }; // class GLRenderingContext

} // namespace zero::render