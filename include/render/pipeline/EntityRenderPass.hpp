#pragma once

#include "render/IRenderPass.hpp"
#include "render/IRenderingManager.hpp"

namespace zero::render
{
    class EntityRenderPass : public RenderPass
    {
    public:
        explicit EntityRenderPass(IRenderingManager* rendering_manager);
        ~EntityRenderPass() override = default;

        void Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta) override;

    private:
        IRenderingManager* rendering_manager_;

    }; // class EntityRenderPass

} // namespace zero::render