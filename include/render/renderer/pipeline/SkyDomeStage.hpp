#pragma once

#include "render/renderer/IRenderStage.hpp"
#include "render/renderer/IRenderingManager.hpp"

namespace zero::render
{
    class SkyDomeStage : public IRenderStage
    {
    public:
        explicit SkyDomeStage(IRenderingManager* rendering_manager);
        ~SkyDomeStage() override = default;

        void Execute(IRenderView* render_view) override;
    private:
        IRenderingManager* rendering_manager_;
        std::shared_ptr<IMesh> sphere_mesh_;

    }; // class SkyDomeStage

} // namespace zero::render