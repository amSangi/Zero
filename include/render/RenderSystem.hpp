#pragma once

#include <memory>
#include "core/Component.hpp"
#include "core/RenderSystemConfig.hpp"
#include "core/System.hpp"
#include "render/IRenderer.hpp"
#include "render/Propagator.hpp"
#include "render/Window.hpp"

namespace zero::render {

    // Forward declarations
    class PrimitiveInstance;

    /**
     * @brief The RenderingSystem uses transform, mesh, material, scene, and camera components
     * to render a scene. It is responsible for all windowing/rendering tasks.
     */
    class RenderSystem : public System {
    public:

        /**
         * @brief Create a RenderSystem
         * @param engine_core the engine core containing shared system objects
         * @param config the RenderSystem configuration
         */
        RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config);

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
         * @brief Create a new entity based on a 3D model.
         *
         * Constructs an entity with Transform, Volume, Material, and ModelInstance components.
         *
         * @param model the fully qualified 3D filename
         * @return the root entity associated with the 3D model. NullEntity if an error occurred.
         */
        [[nodiscard]] Component::Entity CreateModelInstance(const std::string& model);

        /**
         * @brief Create a new entity based on a primitive shape.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param primitive the primitive instance data.
         * @return the primitive entity instance.
         */
        [[nodiscard]] Component::Entity CreatePrimitiveInstance(const PrimitiveInstance& primitive);

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
         * @brief The renderer used to draw and cull entities onto the window
         */
        std::unique_ptr<IRenderer> renderer_;

        /**
         * @brief The transform/volume parent-child propagator
         */
        std::unique_ptr<Propagator> propagator_;

    }; // class RenderSystem

} // namespace zero::render