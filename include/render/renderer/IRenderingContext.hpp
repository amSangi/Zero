#pragma once

#include "engine/RenderSystemConfig.hpp"
#include "component/Camera.hpp"
#include "component/Material.hpp"
#include "core/NonCopyable.hpp"
#include "render/renderer/IMesh.hpp"
#include "render/renderer/IProgram.hpp"
#include "render/renderer/IRenderTarget.hpp"
#include "render/renderer/ISampler.hpp"
#include "render/renderer/ITexture.hpp"
#include "render/renderer/IUniformBuffer.hpp"
#include "render/renderer/Model.hpp"

namespace zero::render
{
    class IRenderingContext : public NonCopyable
    {
    public:

        enum class FillMode
        {
            WIREFRAME,
            SOLID,
        }; // enum class FillMode

        enum class CullMode
        {
            CULL_NONE,
            CULL_FRONT,
            CULL_BACK,
        }; // enum class CullMode

        virtual ~IRenderingContext() = default;

        virtual void Initialize(const RenderSystemConfig& config) = 0;

        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)  = 0;
        virtual void SetFillMode(FillMode fill_mode) = 0;
        virtual void SetCullMode(CullMode cull_mode) = 0;

        virtual void SetClearColor(const math::Vec4f& color) = 0;
        virtual void Clear() = 0;
        virtual void ClearColor() = 0;
        virtual void ClearDepth() = 0;
        virtual void ClearStencil() = 0;

        virtual void BeginFrame(IRenderTarget* render_target) = 0;
        virtual void EndFrame() = 0;

        virtual void BindShaderProgram(IProgram* shader_program) = 0;
        virtual void BindUniformBuffer(IUniformBuffer* uniform_buffer) = 0;
        virtual void BindTextureSampler(uint32 index, ISampler* sampler) = 0;
        virtual void BindTexture(uint32 index, ITexture* texture) = 0;

        virtual void Draw(IMesh* mesh) = 0;

    }; // interface IRenderingContext

} // namespace zero::render