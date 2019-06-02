#pragma once

#include <memory>
#include <vector>
#include <entt.hpp>
#include "event/EventBus.hpp"
#include "NonCopyable.hpp"
#include "ZBase.hpp"

namespace zero {

    /* ********** Forward Declarations ********** */
    class EngineConfig;
    class System;

    /* ********** Engine ********** */

    /**
     * @brief The Game Engine
     */
    class Engine : public NonCopyable {
    public:

        /**
         * @brief Create an Engine using the given configuration
         * @param config the Engine configuration
         * @return an Engine
         */
        static std::shared_ptr<Engine> Create(const EngineConfig& config) { return nullptr; }

        /**
         * @brief Engine destructor
         */
        ~Engine() = default;

        /**
         * @brief Initialize the engine
         */
        void Initialize() {}

        /**
         * @brief Run the engine
         * @return the error code
         */
        uint32 Run() { return 0; }

        /**
         * @brief Shutdown the engine
         */
        void ShutDown() {}


        /* ********** Accessors ********** */

        /**
         * @brief Get the event bus
         * @return the event bus
         */
        inline event::EventBus& GetEventBus()         { return event_bus_; }

        /**
         * @brief Get the game registry
         * @return the registry
         */
        inline entt::registry<>& GetRegistry()        { return registry_; }

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
         * @brief The game systems
         */
        std::vector<std::unique_ptr<System>> game_systems_;

        /**
         * @brief Construct using Engine::Create
         */
        Engine() = default;

    }; // class Engine

} // namespace zero