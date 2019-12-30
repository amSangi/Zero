#pragma once

#include <memory>
#include "core/System.hpp"
#include "core/RenderSystemConfig.hpp"
#include "Components.hpp"
#include "IAnimator.hpp"
#include "IRenderer.hpp"
#include "Propagator.hpp"
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
         * @param core_engine the engine core containing shared system objects
         * @param config the RenderSystem configuration
         */
        RenderSystem(CoreEngine* core_engine, const RenderSystemConfig& config);

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
         * @param time_delta updated timing information since the last engine tick
         */
        void Update(const TimeDelta& time_delta) override;

        /**
         * @brief Cleanup rendering tasks
         */
        void PostUpdate() override;

        /**
         * @brief Cleanup the renderer and close the window
         */
        void ShutDown() override;

        /**
         * @brief Poll all the events on the rendering window and post them onto the EventBus for processing
         */
        void PollWindowEvents();

        /**
         * @brief Create a new entity based on a 3D model.
         *
         * Constructs an entity with a Transform, Volume, Material, and ModelInstance components.
         *
         * @param model the fully qualified 3D filename
         * @return the root entity associated with the 3D model. NullEntity if an error occurred.
         */
        Component::Entity CreateModelInstance(const std::string& model);

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
         * @brief The renderer used to draw and cull entities onto the window
         */
        std::unique_ptr<IRenderer> renderer_;

        /**
         * @brief The transform/volume parent-child propagator
         */
        std::unique_ptr<Propagator> propagator_;

    }; // class RenderSystem

} // namespace zero::render