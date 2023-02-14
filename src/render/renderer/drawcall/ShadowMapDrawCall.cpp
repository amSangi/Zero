#include "render/renderer/drawcall/ShadowMapDrawCall.hpp"

namespace zero::render
{

ShadowMapDrawCall::ShadowMapDrawCall(uint32 mesh_id,
                                     Material material,
                                     ModelData model_data,
                                     std::shared_ptr<IMesh> mesh,
                                     std::shared_ptr<IProgram> program,
                                     std::shared_ptr<UniformManager> uniform_manager)
: draw_key_()
, material_(std::move(material))
, model_data_(model_data)
, mesh_(std::move(mesh))
, program_(std::move(program))
, uniform_manager_(std::move(uniform_manager))
{
    draw_key_.mesh_id_ = mesh_id;
    draw_key_.material_id_ = material_.GetShaderID();
    draw_key_.texture_id_ = material_.GetTextureID();
}

const DrawKey& ShadowMapDrawCall::GetDrawKey()
{
    return draw_key_;
}

void ShadowMapDrawCall::Draw(IRenderHardware* rhi)
{
    rhi->BindShaderProgram(program_);
    rhi->UpdateUniformData(uniform_manager_->GetModelUniform(), &model_data_, sizeof(model_data_), 0);
    rhi->BindUniformBuffer(uniform_manager_->GetModelUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetCameraUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetShadowMapUniform());
    rhi->DrawMesh(mesh_);
}

} // namespace zero::render