#include <utility>
#include "render/renderer/drawcall/EntityDrawCall.hpp"

namespace zero::render
{

EntityDrawCall::EntityDrawCall(uint32 mesh_id,
                               Material material,
                               ModelData model_data,
                               std::shared_ptr<IMesh> mesh,
                               std::shared_ptr<IProgram> program,
                               std::shared_ptr<ISampler> texture_sampler,
                               std::shared_ptr<ISampler> shadow_map_texture_sampler,
                               std::shared_ptr<UniformManager> uniform_manager,
                               std::shared_ptr<ITexture> diffuse_texture)
: draw_key_()
, material_(std::move(material))
, model_data_(model_data)
, mesh_(std::move(mesh))
, program_(std::move(program))
, texture_sampler_(std::move(texture_sampler))
, shadow_map_texture_sampler_(std::move(shadow_map_texture_sampler))
, uniform_manager_(std::move(uniform_manager))
, diffuse_texture_(std::move(diffuse_texture))
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

    // Update uniforms
    rhi->UpdateUniformData(uniform_manager_->GetModelUniform(), &model_data_, sizeof(model_data_), 0);
    MaterialData material_data{material_};
    rhi->UpdateUniformData(uniform_manager_->GetMaterialUniform(), &material_data, sizeof(material_data), 0);

    // Bind uniforms
    rhi->BindUniformBuffer(uniform_manager_->GetCameraUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetModelUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetMaterialUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetLightInformationUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetDirectionalLightUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetPointLightUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetSpotLightUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetShadowMapUniform());

    // Bind textures
    rhi->BindTexture(diffuse_texture_, texture_sampler_, uniform_manager_->GetDiffuseMapUniformSamplerName());
    for (const auto& [uniform_name, shadow_map_texture]: uniform_manager_->GetShadowTextureUniformMap())
    {
        rhi->BindTexture(shadow_map_texture, shadow_map_texture_sampler_, uniform_name);
    }
    rhi->DrawMesh(mesh_);
}

} // namespace zero::render
