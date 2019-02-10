#pragma once

#include <memory>
#include <vector>
#include "core/ZBase.hpp"
#include "System.hpp"

namespace zero {

	class Engine {
	public:
		Engine();

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
			system->registry_ = &registry_;
			systems_.push_back(std::move(system));
		}

	protected:
		entt::registry<> registry_;
		std::vector<std::unique_ptr<System>> systems_;

	}; // abstract class Engine

} // namespace zero