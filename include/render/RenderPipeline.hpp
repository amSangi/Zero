#pragma once

#include <vector>
#include <memory>
#include <entt/entt.hpp>
#include "component/Camera.hpp"

namespace zero::render
{
    // Forward declarations
    class IRenderPass;

    class RenderPipeline
    {
    public:
        RenderPipeline() = default;
        ~RenderPipeline() = default;

        void AddRenderPass(std::unique_ptr<IRenderPass> render_pass);
        void Execute(const Camera& camera, entt::registry& registry);

    private:
        std::vector<std::unique_ptr<IRenderPass>> render_passes_;
    }; // class RenderPipeline

} // namespace zero::render