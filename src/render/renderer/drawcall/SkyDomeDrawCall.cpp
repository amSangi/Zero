#include "render/renderer/drawcall/SkyDomeDrawCall.hpp"

namespace zero::render
{

SkyDomeDrawCall::SkyDomeDrawCall(ModelData model_data,
                                 math::Vec3f apex_color,
                                 math::Vec3f center_color,
                                 std::shared_ptr<IMesh> mesh,
                                 std::shared_ptr<IProgram> program,
                                 std::shared_ptr<UniformManager> uniform_manager)
: draw_key_()
, model_data_(model_data)
, apex_color_(apex_color)
, center_color_(center_color)
, sky_dome_mesh_(std::move(mesh))
, program_(std::move(program))
, uniform_manager_(std::move(uniform_manager))
{
    // Ensure the sky dome draw call is called first
    draw_key_.mesh_id_ = 0;
    draw_key_.texture_id_ = 0;
    draw_key_.material_id_ = 0;
}

const DrawKey& SkyDomeDrawCall::GetDrawKey()
{
    return draw_key_;
}

void SkyDomeDrawCall::Draw(IRenderHardware *rhi)
{
    program_->SetUniform(uniform_manager_->GetSkyDomeApexColorUniformName(), apex_color_.XYZ());
    program_->SetUniform(uniform_manager_->GetSkyDomeCenterColorUniformName(), center_color_.XYZ());
    rhi->BindShaderProgram(program_);
    rhi->SetFillMode(IRenderHardware::FillMode::FILL_MODE_SOLID);
    rhi->SetCullMode(IRenderHardware::CullMode::CULL_MODE_NONE);
    rhi->UpdateUniformData(uniform_manager_->GetModelUniform(), &model_data_, sizeof(model_data_), 0);
    rhi->BindUniformBuffer(uniform_manager_->GetModelUniform());
    rhi->BindUniformBuffer(uniform_manager_->GetCameraUniform());
    rhi->DrawMesh(sky_dome_mesh_);
}

} // namespace zero::render