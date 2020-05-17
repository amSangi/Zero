#pragma once

#include <memory>
#include "core/Component.hpp"
#include "core/RenderSystemConfig.hpp"
#include "core/System.hpp"
#include "render/IRenderer.hpp"
#include "render/Window.hpp"

namespace zero::render
{

    // Forward declarations
    class Light;
    class PrimitiveInstance;

    /**
     * @brief The RenderingSystem uses transform, mesh, material, scene, and camera components
     * to render a scene. It is responsible for all windowing/rendering tasks.
     */
    class RenderSystem : public System
    {
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
         * @param model_filename the fully qualified 3D filename
         * @return the root entity associated with the 3D model. NullEntity if an error occurred.
         */
        [[nodiscard]] Entity CreateModelInstance(const std::string& model_filename);

        /**
         * @brief Create a new entity based on a primitive shape.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param primitive the primitive instance data.
         * @return the primitive entity instance.
         */
        [[nodiscard]] Entity CreatePrimitiveInstance(const PrimitiveInstance& primitive);

        /**
         * @brief Create a new light entity or attach a light component to an existing entity
         *
         * If entity is null, constructs an entity with Transform and a light components.
         * The light component can be either a DirectionalLight, PointLight, or SpotLight.
         *
         * @param light the light component container
         * @param entity the entity to attach the light to. Null Entity if a new entity should be created.
         * @return the light entity instance
         */
        Entity CreateLightInstance(const Light& light, Entity entity);

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

    }; // class RenderSystem

} // namespace zero::render