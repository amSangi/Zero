#include "render/renderer/drawcall/ShadowMapDrawCall.hpp"

namespace zero::render
{

ShadowMapDrawCall::ShadowMapDrawCall(uint32 mesh_id,
                                     Material material,
                                     ModelData model_data,
                                     std::shared_ptr<IUniformBuffer> model_uniform,
                                     std::shared_ptr<IMesh> mesh,
                                     std::shared_ptr<IProgram> program,
                                     std::unordered_map<std::string, std::shared_ptr<ITexture>> textures,
                                     std::shared_ptr<ISampler> texture_sampler)
: draw_key_()
, material_(std::move(material))
, model_data_(model_data)
, model_uniform_buffer_(std::move(model_uniform))
, mesh_(std::move(mesh))
, program_(std::move(program))
, texture_sampler_(std::move(texture_sampler))
, textures_(std::move(textures))
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
    rhi->UpdateUniformData(model_uniform_buffer_, &model_data_, sizeof(model_data_), 0);
    rhi->BindUniformBuffer(model_uniform_buffer_);
    for (const auto& [uniform_name, texture] : textures_)
    {
        rhi->BindTexture(texture, texture_sampler_, uniform_name);
    }
    rhi->DrawMesh(mesh_);
}

} // namespace zero::render