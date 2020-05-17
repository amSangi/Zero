#pragma once

#include <entt/entt.hpp>
#include "EngineCore.hpp"
#include "TimeDelta.hpp"

namespace zero
{

    // Forward declarations
    namespace event { class EventBus; }
    class Engine;

    /**
     * @brief Systems work on entity components and run the game
     * @note Access to components is done through the registry
     * @see https://skypjack.github.io/entt/classentt_1_1Registry.html
     */
    class System
    {
    public:

        /**
         * @brief System Constructor
         * @param engine_core the core objects shared amongst different systems
         */
        explicit System(EngineCore* engine_core);

        /**
         * @brief Default Virtual Destructor
         */
        virtual ~System() = default;

        /**
         * @brief Initialize the system. Called once before any updates have been performed.
         */
        virtual void Initialize() = 0;

        /**
         * @brief Perform pre-update operations. Called before all systems are updated.
         */
        virtual void PreUpdate() = 0;

        /**
         * @brief Update the system. Called during game loop.
         * @param time_delta updated timing information since the last engine tick
         */
        virtual void Update(const TimeDelta& time_delta) = 0;

        /**
         * @brief Perform post update operations. Called after all systems have been updated.
         */
        virtual void PostUpdate() = 0;

        /**
         * @brief Perform all shutdown operations. Called just before destruction.
         */
        virtual void ShutDown() = 0;

        /**
         * @brief Get the game engine entity-component registry
         * @return the registry
         */
        [[nodiscard]] entt::registry& GetRegistry();

        /**
         * @brief Get the game engine event bus
         * @return the event bus
         */
        [[nodiscard]] event::EventBus& GetEventBus();

    protected:

        /**
         * @brief The core objects shared amongst different systems
         */
        EngineCore* engine_core_;

    }; // abstract class System

} // namespace zero