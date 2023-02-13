#include "render/renderer/drawcall/SkyDomeDrawCall.hpp"

namespace zero::render
{

SkyDomeDrawCall::SkyDomeDrawCall(std::shared_ptr<IMesh> mesh,
                                 std::shared_ptr<IProgram> program)
: draw_key_()
, sky_dome_mesh_(std::move(mesh))
, program_(std::move(program))
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
    rhi->BindShaderProgram(program_);
    rhi->SetFillMode(IRenderHardware::FillMode::FILL_MODE_SOLID);
    rhi->SetCullMode(IRenderHardware::CullMode::CULL_MODE_NONE);
    rhi->DrawMesh(sky_dome_mesh_);
}

} // namespace zero::render