#pragma once

#include <memory>
#include "render/renderer/IMesh.hpp"
#include "render/renderer/IProgram.hpp"
#include "render/renderer/ITexture.hpp"
#include "render/renderer/IUniformBuffer.hpp"

namespace zero::render
{
    struct RenderKey
    {
        uint32_t mesh_id_;
        uint32_t shader_program_id_;
        uint32_t uniform_buffer_id_;
        uint32_t texture_id_;
    }; // struct RenderKey

	class IRenderCall
	{
	public:
		virtual const RenderKey& GetRenderKey() = 0;
		virtual std::shared_ptr<IMesh> GetMesh() = 0;
		virtual std::shared_ptr<IProgram> GetShaderProgram() = 0;
		virtual const std::vector<std::shared_ptr<IUniformBuffer>>& GetUniformBufferObjects() = 0;
        virtual const std::vector<std::shared_ptr<ITexture>>& GetTextures() = 0;
        virtual const std::vector<std::shared_ptr<ITexture>>& GetShadowMapTextures() = 0;
        virtual bool IsShadowMapPass() = 0;
	}; // class IRenderCall

} // namespace zero::render