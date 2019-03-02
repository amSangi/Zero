#pragma once

#include <memory>
#include <vector>
#include "entt.hpp"
#include "core/ZBase.hpp"
#include "event/EventBus.hpp"
#include "System.hpp"

namespace zero {

    class EngineConfig;

    class Engine {
    public:

        /**
         * @brief Create an Engine using the given configuration
         * @param config the Engine configuration
         * @return an Engine
         */
        static std::unique_ptr<Engine> Create(const EngineConfig& config);

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

    protected:

        /**
         * @brief The registry containing the entities and their components
         * @see https://github.com/skypjack/entt
         */
        entt::registry<> registry_;

        /**
         * @brief The event bus
         */
        event::EventBus event_bus_;

        /**
         * @brief The systems
         */
        std::vector<std::unique_ptr<System>> systems_;

        /**
         * @brief Construct using Engine::Create
         */
        Engine();

    }; // class Engine

} // namespace zero