#pragma once

#include "entt.hpp"

namespace zero {

    /* ********** Forward Declarations ********** */
    namespace event { class EventBus; }
    namespace ui    { class Window; }
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
        explicit System(std::shared_ptr<Engine> engine)
        : engine_(std::move(engine)) {}

        /**
         * @brief Default Virtual Destructor
         */
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

        /**
         * @brief Get the game engine window
         * @return the window
         */
        ui::Window* GetWindow();

        /**
         * @brief Get the game engine entity-component registry
         * @return the registry
         */
        entt::registry<>& GetRegistry();

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