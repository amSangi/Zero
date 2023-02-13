#pragma once

#include "component/Material.hpp"
#include "render/renderer/IDrawCall.hpp"
#include "render/renderer/UniformBufferData.hpp"

namespace zero::render
{

	class EntityDrawCall: public IDrawCall
	{
	public:
		EntityDrawCall(uint32 mesh_id,
		               Material material,
		               ModelData model_data,
		               std::shared_ptr<IUniformBuffer> model_uniform,
		               std::shared_ptr<IUniformBuffer> material_uniform,
		               std::shared_ptr<IMesh> mesh,
		               std::shared_ptr<IProgram> program,
					   std::vector<std::shared_ptr<IUniformBuffer>> uniform_buffers,
		               std::unordered_map<std::string, std::shared_ptr<ITexture>> textures,
		               std::shared_ptr<ISampler> texture_sampler,
		               std::unordered_map<std::string, std::shared_ptr<ITexture>> shadow_map_textures,
		               std::shared_ptr<ISampler> shadow_map_texture_sampler);
		~EntityDrawCall() override = default;
		const DrawKey& GetDrawKey() override;
		void Draw(IRenderHardware* rhi) override;
	private:
		DrawKey draw_key_;
		Material material_;
		ModelData model_data_;
		std::shared_ptr<IUniformBuffer> model_uniform_buffer_;
		std::shared_ptr<IUniformBuffer> material_uniform_buffer_;
		std::shared_ptr<IMesh> mesh_;
		std::shared_ptr<IProgram> program_;
		std::vector<std::shared_ptr<IUniformBuffer>> uniform_buffers_;
		std::shared_ptr<ISampler> texture_sampler_;
		std::shared_ptr<ISampler> shadow_map_texture_sampler_;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> textures_;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> shadow_map_textures_;

	}; // class EntityDrawCall

} // namespace zero::render