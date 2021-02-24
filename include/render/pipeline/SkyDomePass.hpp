#pragma once

#include "render/IRenderPass.hpp"
#include "render/IRenderingManager.hpp"

namespace zero::render
{
    class SkyDomePass : public IRenderPass
    {
    public:
        explicit SkyDomePass(IRenderingManager* rendering_manager);
        ~SkyDomePass() override = default;

        void Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta) override;

    private:
        void RenderSkyDome(const Camera& camera, const SkyDome& sky_dome) const;

        IRenderingManager* rendering_manager_;
        std::shared_ptr<IMesh> sphere_mesh_;

    }; // class SkyDomePass

} // namespace zero::render