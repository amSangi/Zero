#pragma once

#include "render/renderer/IDrawCall.hpp"
#include "render/renderer/UniformManager.hpp"
#include "render/renderer/UniformBufferData.hpp"

namespace zero::render
{
    class SkyDomeDrawCall : public IDrawCall
    {
    public:
        SkyDomeDrawCall(ModelData model_data,
                        math::Vec3f apex_color,
                        math::Vec3f center_color,
                        std::shared_ptr<IMesh> mesh,
                        std::shared_ptr<IProgram> program,
                        std::shared_ptr<UniformManager> uniform_manager);
        ~SkyDomeDrawCall() override = default;
        const DrawKey& GetDrawKey() override;
        void Draw(IRenderHardware* rhi) override;
    private:
        DrawKey draw_key_;
        ModelData model_data_;
        math::Vec3f apex_color_;
        math::Vec3f center_color_;
        std::shared_ptr<IMesh> sky_dome_mesh_;
        std::shared_ptr<IProgram> program_;
        std::shared_ptr<UniformManager> uniform_manager_;

    }; // class SkyDomeDrawCall

} // namespace zero::render