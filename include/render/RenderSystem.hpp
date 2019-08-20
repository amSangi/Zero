#pragma once

#include <memory>
#include "core/System.hpp"
#include "IAnimator.hpp"
#include "IRenderer.hpp"
#include "Propagator.hpp"
#include "RenderSystemConfig.hpp"
#include "Window.hpp"

namespace zero::render {

    /**
     * @brief The RenderingSystem uses transform, mesh, material, scene, and camera components
     * to render a scene. It is responsible for all windowing/rendering tasks.
     */
    class RenderSystem : public System {
    public:

        /**
         * @brief Create a RenderSystem
         * @param engine the engine this system belongs to
         * @param config the RenderSystem configuration
         */
        RenderSystem(std::shared_ptr<Engine> engine, const RenderSystemConfig& config);

        ~RenderSystem() override = default;

        /**
         * @brief Initialize the window and renderer
         */
        void Initialize() override;

        /**
         * @brief Update the transforms and bounding volumes of all entities
         */
        void PreUpdate() override;

        /**
         * @brief Cull and render all entities with a material component in the active scene using the given cameras
         * @param dt time between the current and last frame
         */
        void Update(float dt) override;

        /**
         * @brief Cleanup rendering tasks
         */
        void PostUpdate() override;

        /**
         * @brief Cleanup the renderer and close the window
         */
        void ShutDown() override;

    private:

        /**
         * @brief The RenderSystem configuration
         */
        RenderSystemConfig config_;

        /**
         * @brief The window to manage the actual window and graphics api context
         */
        std::unique_ptr<Window> window_;

        /**
         * @brief The animator used to animate all dynamic models
         */
        std::unique_ptr<IAnimator> animator_;

        /**
         * @brief The renderer used to call to the graphics API
         */
        std::unique_ptr<IRenderer> renderer_;

        /**
         * @brief The transform/volume parent-child propagator
         */
        std::unique_ptr<Propagator> propagator_;

    }; // class RenderSystem

} // namespace zero::render