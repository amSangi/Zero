#pragma once

#include "render/renderer/IDrawCall.hpp"
#include "component/Camera.hpp"
#include "component/SkyDome.hpp"

namespace zero::render
{
    class SkyDomeDrawCall : public IDrawCall
    {
    public:
        SkyDomeDrawCall(std::shared_ptr<IMesh> mesh, std::shared_ptr<IProgram> program);
        ~SkyDomeDrawCall() override = default;
        const DrawKey& GetDrawKey() override;
        void Draw(IRenderHardware* rhi) override;
    private:
        DrawKey draw_key_;
        std::shared_ptr<IMesh> sky_dome_mesh_;
        std::shared_ptr<IProgram> program_;

    }; // class SkyDomeDrawCall

} // namespace zero::render