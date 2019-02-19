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
		entt::registry<>* registry_ = nullptr;
		event::EventBus* event_bus_ = nullptr;

	}; // abstract class System

} // namespace zero