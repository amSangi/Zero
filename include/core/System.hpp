#pragma once

#include <memory>
#include <entt/entt.hpp>

namespace zero {

    /* ********** Forward Declarations ********** */
    namespace event { class EventBus; }
    class Engine;

    /**
     * @brief Systems work on entity components and run the game
     * @note Access to components is done through the registry
     * @see https://skypjack.github.io/entt/classentt_1_1Registry.html
     */
    class System {
    public:

        /**
         * @brief System Constructor
         * @param engine the engine this system belongs to
         */
        explicit System(std::shared_ptr<Engine> engine);

        /**
         * @brief Default Virtual Destructor
         */
        virtual ~System() = 0;

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
         * @param dt time between current and last frame.
         */
        virtual void Update(float dt) = 0;


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
        entt::registry& GetRegistry();

        /**
         * @brief Get the game engine event bus
         * @return the event bus
         */
        event::EventBus& GetEventBus();

    protected:

        /**
         * @brief The Game Engine this system belongs to
         */
        std::shared_ptr<Engine> engine_{nullptr};

    }; // abstract class System

} // namespace zero