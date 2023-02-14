#pragma once

#include "render/renderer/IDrawCall.hpp"
#include "component/Material.hpp"
#include "render/renderer/UniformBufferData.hpp"
#include "render/renderer/UniformManager.hpp"

namespace zero::render
{

    class ShadowMapDrawCall: public IDrawCall
    {
    public:
        ShadowMapDrawCall(uint32 mesh_id,
                          Material material,
                          ModelData model_data,
                          std::shared_ptr<IMesh> mesh,
                          std::shared_ptr<IProgram> program,
                          std::shared_ptr<UniformManager> uniform_manager);
        ~ShadowMapDrawCall() override = default;
        const DrawKey& GetDrawKey() override;
        void Draw(IRenderHardware* rhi) override;
    private:
        DrawKey draw_key_;
        Material material_;
        ModelData model_data_;
        std::shared_ptr<IMesh> mesh_;
        std::shared_ptr<IProgram> program_;
        std::shared_ptr<UniformManager> uniform_manager_;
    }; // class ShadowMapDrawCall

} // namespace zero::render