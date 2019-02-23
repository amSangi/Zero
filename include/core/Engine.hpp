#pragma once

#include <memory>
#include <vector>
#include "core/ZBase.hpp"
#include "event/EventBus.hpp"
#include "System.hpp"

namespace zero {

	class Engine {
	public:
		Engine();
		~Engine();

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

		/**
		 * @brief Add a system from the Entity-Component-System
		 * @param system the system
		 */
		void AddSystem(std::unique_ptr<System> system);

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

	}; // class Engine

} // namespace zero