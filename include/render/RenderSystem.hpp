#pragma once

#include <memory>
#include "component/Component.hpp"
#include "component/Light.hpp"
#include "component/PrimitiveInstance.hpp"
#include "core/System.hpp"
#include "engine/RenderSystemConfig.hpp"
#include "render/ModelLoader.hpp"
#include "render/Window.hpp"
#include "render/renderer/IRenderHardware.hpp"
#include "render/renderer/RenderingPipeline.hpp"
#include "render/scene/SceneManager.hpp"

namespace zero::render
{
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

        /**
         * @brief Default destructor
         */
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
         * @brief Create a new entity based on a model name.
         *
         * Constructs an entity with Transform, Volume, Material, and ModelInstance components.
         *
         * @param model_name the model name
         * @param parent_entity the parent entity of the newly instantiated model
         * @return the root entity associated with the model. NullEntity if an error occurred.
         */
        [[nodiscard]] Entity CreateModelInstance(const std::string& model_name, Entity parent_entity);

        /**
         * @brief Create a new entity based on a primitive shape.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param primitive the primitive instance data.
         * @return the primitive entity instance.
         */
        [[nodiscard]] Entity CreatePrimitiveInstance(const PrimitiveInstance& primitive) const;

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
        Entity CreateLightInstance(const Light& light, Entity entity) const;

    private:
        /**
         * @brief Load all 3D assets
         */
        void LoadModels();

        void GenerateDrawCalls(IRenderView* render_view) const;

        /**
         * @brief Check whether there is an active camera in the scene
         * @return True if there is an active Camera entity in the scene. Otherwise, false.
         */
        [[nodiscard]] bool ContainsCamera() const;

        /**
         * @brief The log title
         */
        static const char* kTitle;

        RenderSystemConfig config_;
        std::unique_ptr<IRenderHardware> rhi_;
        std::unique_ptr<Window> window_;
        std::unique_ptr<RenderingPipeline> rendering_pipeline_;
        std::unique_ptr<SceneManager> scene_manager_;
        std::unordered_map<std::string, std::shared_ptr<Model>> model_cache_;

    }; // class RenderSystem

} // namespace zero::render