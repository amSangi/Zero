#pragma once

#include <memory>
#include "render/Image.hpp"
#include "render/MeshData.hpp"
#include "render/renderer/ShaderStage.hpp"
#include "render/renderer/IFrameBuffer.hpp"
#include "render/renderer/IMesh.hpp"
#include "render/renderer/IProgram.hpp"
#include "render/renderer/ITexture.hpp"
#include "render/renderer/ISampler.hpp"
#include "render/renderer/IUniformBuffer.hpp"

namespace zero::render
{
    class IRenderHardware
    {
    public:
        enum class FillMode
        {
            FILL_MODE_WIREFRAME,
            FILL_MODE_SOLID,
        }; // enum class FillMode

        enum class CullMode
        {
            CULL_MODE_NONE,
            CULL_MODE_FRONT,
            CULL_MODE_BACK,
        }; // enum class CullMode

        virtual ~IRenderHardware() = default;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)  = 0;
        virtual void SetFillMode(FillMode fill_mode) = 0;
        virtual void SetCullMode(CullMode cull_mode) = 0;

        virtual void SetClearColor(const math::Vec4f& color) = 0;
        virtual void Clear() = 0;

        virtual std::shared_ptr<ISampler> GetDiffuseMapSampler() = 0;
        virtual std::shared_ptr<ISampler> GetShadowMapSampler() = 0;

        virtual const std::vector<std::shared_ptr<ITexture>>& GetShadowMapTextures() = 0;
        virtual const std::vector<std::shared_ptr<IFrameBuffer>>& GetShadowMapFrameBuffers() = 0;

        virtual void UpdateUniformData(std::shared_ptr<IUniformBuffer> uniform_buffer, const void* data, uint32 data_size, uint32 data_offset) = 0;

        virtual std::shared_ptr<IMesh> CreateMesh(MeshData* mesh_data) = 0;
        virtual std::shared_ptr<IShader> CreateShader(const ShaderStage& shader_stage) = 0;
        virtual std::shared_ptr<IProgram> CreateShaderProgram(const std::vector<std::shared_ptr<IShader>>& shaders) = 0;
        virtual std::shared_ptr<ITexture> CreateTexture(std::unique_ptr<Image> image) = 0;
        virtual std::shared_ptr<IUniformBuffer> CreateUniformBuffer(std::string buffer_name, const void* initial_data, uint32 buffer_size) = 0;

        virtual void BeginFrame(std::shared_ptr<IFrameBuffer> frame_buffer) = 0;
        virtual void EndFrame() = 0;

        virtual void BindShaderProgram(std::shared_ptr<IProgram> shader_program) = 0;
        virtual void BindTexture(std::shared_ptr<ITexture> texture, std::shared_ptr<ISampler> texture_sampler, const std::string& uniform_name) = 0;
        virtual void BindUniformBuffer(std::shared_ptr<IUniformBuffer> uniform_buffer) = 0;

        virtual void DrawMesh(std::shared_ptr<IMesh> mesh) = 0;

    }; // class IRenderHardware

} // namespace zero::render