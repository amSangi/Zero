#pragma once

#include <vector>
#include <memory>
#include <entt/entt.hpp>
#include "component/Camera.hpp"

namespace zero::render
{
    // Forward declarations
    class IRenderPass;

    /**
     * @brief Manages a list of render passes
     *
     * Render passes are executed in the order they were added
     */
    class RenderPipeline
    {
    public:
        RenderPipeline() = default;
        ~RenderPipeline() = default;

        /**
         * @brief Add a new render pass to the render pipeline
         * @param render_pass the render pass
         */
        void AddRenderPass(std::unique_ptr<IRenderPass> render_pass);

        /**
         * @brief Execute the render passes sequentially
         * @param camera the camera to render to
         * @param registry the registry containing all the game entities and their components
         */
        void Execute(const Camera& camera, entt::registry& registry);

    private:
        /**
         * @brief The list of render passes
         */
        std::vector<std::unique_ptr<IRenderPass>> render_passes_;
    }; // class RenderPipeline

} // namespace zero::render