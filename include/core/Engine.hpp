#pragma once

#include <memory>
#include <vector>
#include "EngineCore.hpp"
#include "EngineConfig.hpp"
#include "NonCopyable.hpp"
#include "System.hpp"
#include "TimeDelta.hpp"
#include "ZBase.hpp"
#include "render/RenderSystem.hpp"

namespace zero
{

    /**
     * @brief The Game Engine
     */
    class Engine : public NonCopyable
    {
    public:

        /**
         * @brief Create an Engine using the given configuration
         * @param config the Engine configuration
         */
        explicit Engine(EngineConfig engine_config);

        /**
         * @brief Engine destructor
         */
        ~Engine() = default;

        /**
         * @brief Initialize the engine
         */
        void Initialize();

        /**
         * @brief Tick the engine
         */
        void Tick();

        /**
         * @brief Shutdown the engine
         */
        void ShutDown();

        /**
         * @brief Get a raw reference to the engine core
         * @return the EngineCore
         */
        [[nodiscard]] EngineCore* GetEngineCore() const;

        /**
         * @brief Create a new entity based on a 3D model.
         *
         * Constructs an entity with Transform, Volume, Material, and ModelInstance components.
         *
         * @param model the fully qualified 3D filename
         * @return the root entity associated with the 3D model. NullEntity if an error occurred.
         */
        [[nodiscard]] Entity InstantiateModel(const std::string& model_filename);

        /**
         * @brief Create a new entity based on a primitive shape.
         *
         * Constructs an entity with Transform, Volume, Material, and PrimitiveInstance components.
         *
         * @param primitive the primitive instance data.
         * @return the primitive entity instance.
         */
        [[nodiscard]] Entity InstantiatePrimitive(render::PrimitiveInstance primitive);

        /**
         * @brief Create a new light entity or attach a light component to an existing entity
         *
         * If the entity is null, constructs an entity with Transform
         * and either DirectionalLight, PointLight, or SpotLight components.
         *
         * @param light the light component data container
         * @param entity the entity to attach the light to. Null Entity if a new entity should be created.
         * @return the light entity instance
         */
        Entity InstantiateLight(render::Light light, Entity entity = NullEntity);

    protected:

        /**
         * @brief The engine configuration.
         */
        EngineConfig engine_config_;

        /**
         * @brief Container for all time related data (e.g. time since last frame, physics tick time, etc)
         */
        TimeDelta time_delta_;

        /**
         * @brief The engine core. Contains game data and objects that are used by many different Systems.
         */
        std::unique_ptr<EngineCore> engine_core_;

        /**
         * @brief The Rendering system used to create a window/OpenGL context, render game entities, and instnatiate 3D models.
         */
        std::unique_ptr<render::RenderSystem> render_system_;

        /**
         * @brief The systems that use the game data and objects to progress the game.
         */
        std::vector<std::unique_ptr<System>> game_systems_;

    }; // class Engine

} // namespace zero