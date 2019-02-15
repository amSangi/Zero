#pragma once

#include <memory>
#include "Event.hpp"

namespace zero {
namespace event {

	class EventBus;

	class EventHandler {
	public:
		explicit EventHandler(std::shared_ptr<EventBus> event_bus);
		virtual ~EventHandler();

		virtual void OnSystemEvent(std::shared_ptr<SystemEvent> event) {}
		virtual void OnMouseEvent(std::shared_ptr<MouseEvent> event) {}
		virtual void OnKeyboardEvent(std::shared_ptr<KeyboardEvent> event) {}
		virtual void OnUIEvent(std::shared_ptr<UIEvent> event) {}
		virtual void OnWindowEvent(std::shared_ptr<WindowEvent> event) {}

	private:
		std::shared_ptr<EventBus> event_bus_;

	}; // abstract class EventHandler


} // namespace event
} // namespace zero