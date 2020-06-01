#pragma once

#include <entt/entt.hpp>
#include "event/EventBus.hpp"
#include "core/Logger.hpp"

namespace zero
{

    /**
     * @brief Container for shared data and objects between different systems. Owned by the Engine.
     */
    class EngineCore
    {
    public:

        EngineCore()
        : event_bus_()
        , registry_()
        , logger_()
        {
        }

        /**
         * @brief Get the event bus
         * @return the event bus
         */
        inline event::EventBus& GetEventBus()       { return event_bus_; }

        /**
         * @brief Get the game registry
         * @return the registry
         */
        inline entt::registry& GetRegistry()        { return registry_; }

        /**
         * @brief Get the Logger
         * @return the logger
         */
        inline Logger& GetLogger()                  { return logger_;   }

    private:
        /**
         * @brief The event bus that manages all events. Used for registering events and event handlers.
         */
        event::EventBus event_bus_;

        /**
         * @brief The registry containing the game entities and their components.
         * @see https://github.com/skypjack/entt
         */
        entt::registry registry_;

        /**
         * @brief The logger used to log messages throughout the engine
         */
        Logger logger_;

    }; // class EngineCore

} // namespace zero
