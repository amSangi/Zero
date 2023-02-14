#pragma once

#include "component/Material.hpp"
#include "render/renderer/IDrawCall.hpp"
#include "render/renderer/UniformBufferData.hpp"
#include "render/renderer/UniformManager.hpp"

namespace zero::render
{

    class EntityDrawCall: public IDrawCall
    {
    public:
        EntityDrawCall(uint32 mesh_id,
                       Material material,
                       ModelData model_data,
                       std::shared_ptr<IMesh> mesh,
                       std::shared_ptr<IProgram> program,
                       std::shared_ptr<ISampler> texture_sampler,
                       std::shared_ptr<ISampler> shadow_map_texture_sampler,
                       std::shared_ptr<UniformManager> uniform_manager,
                       std::shared_ptr<ITexture> diffuse_texture);
        ~EntityDrawCall() override = default;
        const DrawKey& GetDrawKey() override;
        void Draw(IRenderHardware* rhi) override;
    private:
        DrawKey draw_key_;
        Material material_;
        ModelData model_data_;
        std::shared_ptr<IMesh> mesh_;
        std::shared_ptr<IProgram> program_;
        std::shared_ptr<ISampler> texture_sampler_;
        std::shared_ptr<ISampler> shadow_map_texture_sampler_;
        std::shared_ptr<UniformManager> uniform_manager_;
        std::shared_ptr<ITexture> diffuse_texture_;

    }; // class EntityDrawCall

} // namespace zero::render