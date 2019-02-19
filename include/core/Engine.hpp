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

		uint32 Run();
		void Initialize();
		void ShutDown();


		void AddSystem(std::unique_ptr<System> system);

	protected:
		entt::registry<> registry_;
		event::EventBus event_bus_;
		std::vector<std::unique_ptr<System>> systems_;

	}; // class Engine

} // namespace zero