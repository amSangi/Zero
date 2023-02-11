#pragma once

#include "render/renderer/drawcall/EntityDrawCall.hpp"
#include "render/renderer/drawcall/ShadowMapDrawCall.hpp"
#include "render/renderer/drawcall/SkyDomeDrawCall.hpp"

namespace zero::render
{

	class EntityDrawCallBuilder
	{
	public:
		EntityDrawCallBuilder() = default;

		EntityDrawCallBuilder& WithRenderPass(uint8 render_pass) {
			render_pass_ = render_pass;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		EntityDrawCallBuilder& WithMaterial(const Material& material) {
			material_ = material;
			return *this;
		}
		std::unique_ptr<EntityDrawCall> Build() {
			return std::make_unique<
		}
	private:
		uint8 render_pass_;
		Material material_;
		math::Matrix4x4 model_matrix_;
		std::shared_ptr<IUniformBuffer> model_uniform_buffer_;
		std::shared_ptr<IMesh> mesh_;
		std::shared_ptr<IProgram> program_;
		std::shared_ptr<ISampler> texture_sampler_;
		std::shared_ptr<ISampler> shadow_map_texture_sampler_;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> textures_;
		std::unordered_map<std::string, std::shared_ptr<ITexture>> shadow_map_textures_;
		std::vector<std::shared_ptr<IUniformBuffer>> uniform_buffers_;
	};

} // namespace zero::render