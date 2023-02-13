#include <utility>
#include "render/renderer/drawcall/EntityDrawCall.hpp"

namespace zero::render
{

EntityDrawCall::EntityDrawCall(uint32 mesh_id,
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
                               std::shared_ptr<ISampler> shadow_map_texture_sampler)
: draw_key_()
, material_(std::move(material))
, model_data_(model_data)
, model_uniform_buffer_(std::move(model_uniform))
, material_uniform_buffer_(std::move(material_uniform))
, mesh_(std::move(mesh))
, program_(std::move(program))
, uniform_buffers_(std::move(uniform_buffers))
, texture_sampler_(std::move(texture_sampler))
, shadow_map_texture_sampler_(std::move(shadow_map_texture_sampler))
, textures_(std::move(textures))
, shadow_map_textures_(std::move(shadow_map_textures))
{
	draw_key_.mesh_id_ = mesh_id;
	draw_key_.material_id_ = material_.GetShaderID();
	draw_key_.texture_id_ = material_.GetTextureID();
}

const DrawKey& EntityDrawCall::GetDrawKey()
{
	return draw_key_;
}

void EntityDrawCall::Draw(IRenderHardware *rhi)
{
	rhi->BindShaderProgram(program_);
	rhi->SetCullMode(material_.two_sided_ ? IRenderHardware::CullMode::CULL_MODE_NONE : IRenderHardware::CullMode::CULL_MODE_BACK);
	rhi->SetFillMode(material_.wireframe_enabled_ ? IRenderHardware::FillMode::FILL_MODE_WIREFRAME : IRenderHardware::FillMode::FILL_MODE_SOLID);
	rhi->UpdateUniformData(model_uniform_buffer_, &model_data_, sizeof(model_data_), 0);
	MaterialData material_data{material_};
	rhi->UpdateUniformData(material_uniform_buffer_, &material_data, sizeof(material_data), 0);
	rhi->BindUniformBuffer(model_uniform_buffer_);
	rhi->BindUniformBuffer(material_uniform_buffer_);
	for (const std::shared_ptr<IUniformBuffer>& uniform_buffer: uniform_buffers_)
	{
		rhi->BindUniformBuffer(uniform_buffer);
	}
	for (const auto& [uniform_name, texture] : textures_)
	{
		rhi->BindTexture(texture, texture_sampler_, uniform_name);
	}
	for (const auto& [uniform_name, shadow_map_texture]: shadow_map_textures_)
	{
		rhi->BindTexture(shadow_map_texture, shadow_map_texture_sampler_, uniform_name);
	}
	rhi->DrawMesh(mesh_);
}

} // namespace zero::render
