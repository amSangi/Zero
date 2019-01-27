#pragma once

#include <memory>
#include <vector>
#include "ecs/EntityManager.hpp"
#include "ecs/Registry.hpp"
#include "ecs/System.hpp"

namespace zero {

	class Engine {
	public:
		Engine()
		: entity_manager_(std::make_shared<EntityManager>()),
		registry_(std::make_shared<Registry>()) {
			entity_manager_->registry_ = registry_;
		}

		virtual ~Engine() = default;

		/**
		 * @brief Runs the game
		 * @return exit code of the game
		 */
		virtual uint32 Run()           = 0;

		/**
		 * @brief Called once on start up
		 */
		virtual void Initialize()      = 0;

		/**
		 * @brief Called repeatedly during the game loop
		 * @param dt the change in time between the last frame and the current frame
		 */
		virtual void Update(float dt)  = 0;

		/**
		 * @brief Called during the termination of the game engine
		 */
		virtual void ShutDown()        = 0;

		/**
		 * @brief Add a System to the engine
		 * @param system the System
		 */
		void AddSystem(std::unique_ptr<System> system) {
			system->entity_manager_ = entity_manager_;
			system->registry_ = registry_;
			systems_.push_back(std::move(system));
		}

	private:
		std::shared_ptr<EntityManager> entity_manager_;
		std::shared_ptr<Registry> registry_;
		std::vector<std::unique_ptr<System>> systems_;

	}; // abstract class Engine

} // namespace zero