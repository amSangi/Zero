#pragma once

#include <memory>
#include "entt.hpp"
#include "event/EventBus.hpp"

namespace zero {

    /**
     * @brief Systems work on entity components and run the game
     * @note Access to components is done through the registry
     * @see https://skypjack.github.io/entt/classentt_1_1Registry.html
     */
    class System {
        friend class Engine;
        using Entity = entt::registry<>::entity_type;
        using EventPtr = std::shared_ptr<event::Event>;
        using EventHandlerPtr = std::shared_ptr<event::EventHandler>;
    public:
        System() = default;
        virtual ~System() = default;

        /**
         * @brief Initialize the system. Called after construction.
         */
        virtual void Initialize() = 0;

        /**
         * @brief Update the system. Called during game loop.
         * @param dt Time between current and last frame.
         */
        virtual void Update(float dt) = 0;

        /**
         * @brief Perform all shutdown operations. Called just before destruction.
         */
        virtual void ShutDown() = 0;

    protected:

        /**
         * @brief The registry used by the game
         */
        entt::registry<>* registry_ = nullptr;

        /**
         * @brief Register an event handler from the event bus
         * @param handler The event handler
         */
        inline void RegisterEventHandler(EventHandlerPtr handler) {
            event_bus_->AddEventHandler(std::move(handler));
        }

        /**
         * @brief Deregister an event handler from the event bus
         * @param handler The event handler
         */
        inline void DeregisterEventHandler(EventHandlerPtr handler) {
            event_bus_->RemoveEventHandler(std::move(handler));
        }

        /**
         * @brief Post an event to the event bus
         * @param event The event
         */
        inline void PostEvent(EventPtr event) {
            event_bus_->Post(std::move(event));
        }

    private:

        /**
         * @brief The event bus used by the game
         */
        event::EventBus* event_bus_ = nullptr;


    }; // abstract class System

} // namespace zero