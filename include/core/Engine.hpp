#pragma once

#include <memory>
#include <vector>
#include "entt.hpp"
#include "System.hpp"
#include "core/ZBase.hpp"
#include "event/EventBus.hpp"
#include "input/InputSystem.hpp"

namespace zero {

    /* ********** Forward Declarations ********** */
    namespace ui { class Window; }
    class EngineConfig;

    /* ********** Engine ********** */

    /**
     * @brief The Game Engine
     */
    class Engine {
    public:

        /**
         * @brief Create an Engine using the given configuration
         * @param config the Engine configuration
         * @return an Engine
         */
        static std::shared_ptr<Engine> Create(const EngineConfig& config);

        /**
         * @brief Engine destructor
         */
        ~Engine() = default;

        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;

        /**
         * @brief Initialize the engine
         */
        void Initialize();

        /**
         * @brief Run the engine
         * @return the error code
         */
        uint32 Run();

        /**
         * @brief Shutdown the engine
         */
        void ShutDown();


        /* ********** Accessors ********** */

        /**
         * @brief Get the event bus
         * @return the event bus
         */
        inline event::EventBus& GetEventBus()  { return event_bus_;    }

        /**
         * @brief Get the game registry
         * @return the registry
         */
        inline entt::registry<>& GetRegistry() { return registry_;     }

        /**
         * @brief Get the game window
         * @return the window
         */
        inline ui::Window* GetWindow()         { return window_.get(); }

    protected:

        /**
         * @brief The event bus
         */
        event::EventBus event_bus_;

        /**
         * @brief The registry containing the entities and their components
         * @see https://github.com/skypjack/entt
         */
        entt::registry<> registry_;

        /**
         * @brief The game window
         */
        std::unique_ptr<ui::Window> window_;

        /**
         * @brief The input system
         */
        input::InputSystem input_system_;

        /**
         * @brief The game systems
         */
        std::vector<std::unique_ptr<System>> game_systems_;

        /**
         * @brief Construct using Engine::Create
         */
        Engine() = default;

    }; // class Engine

} // namespace zero