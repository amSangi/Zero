#pragma once

#include "render/renderer/IDrawCall.hpp"
#include "component/Material.hpp"
#include "render/renderer/UniformBufferData.hpp"

namespace zero::render
{

    class ShadowMapDrawCall: public IDrawCall
    {
    public:
        ShadowMapDrawCall(uint32 mesh_id,
                          Material material,
                          ModelData model_data,
                          std::shared_ptr<IUniformBuffer> model_uniform,
                          std::shared_ptr<IMesh> mesh,
                          std::shared_ptr<IProgram> program,
                          std::unordered_map<std::string, std::shared_ptr<ITexture>>textures,
                          std::shared_ptr<ISampler> texture_sampler);
        ~ShadowMapDrawCall() override = default;
        const DrawKey& GetDrawKey() override;
        void Draw(IRenderHardware* rhi) override;
    private:
        DrawKey draw_key_;
        Material material_;
        ModelData model_data_;
        std::shared_ptr<IUniformBuffer> model_uniform_buffer_;
        std::shared_ptr<IMesh> mesh_;
        std::shared_ptr<IProgram> program_;
        std::shared_ptr<ISampler> texture_sampler_;
        std::unordered_map<std::string, std::shared_ptr<ITexture>> textures_;
    }; // class ShadowMapDrawCall

} // namespace zero::render