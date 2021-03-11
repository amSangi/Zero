#pragma once

#include "render/renderer/IRenderingContext.hpp"

namespace zero::render
{

    class GLRenderingContext final : public IRenderingContext
    {
    public:
        GLRenderingContext();
        ~GLRenderingContext() override;

        void Initialize(const RenderSystemConfig& config) override;
        void Shutdown() override;

        void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
        void SetFillMode(FillMode fill_mode) override;
        void SetCullMode(CullMode cull_mode) override;

        void SetClearColor(const math::Vec4f& color) override;
        void Clear() override;
        void ClearColor() override;
        void ClearDepth() override;
        void ClearStencil() override;

        void BeginFrame() override;
        void EndFrame() override;

        void BeginCamera(const Camera& camera) override;
        void EndCamera() override;

        void BindShaderProgram() override;
        void BindUniformBuffer(uint32 index, IUniformBuffer* uniform_buffer) override;
        void BindTextureSampler(uint32 index, ISampler* sampler) override;
        void BindTexture(uint32 index, ITexture* texture) override;
        void BindMaterial(const Material& material) override;

        void Draw(const Mesh& mesh) override;

    private:
        std::shared_ptr<GLUniformManager> gl_uniform_manager_;
        std::shared_ptr<GLTextureManager> gl_texture_manager_;

    }; // class GLRenderingContext

} // namespace zero::render