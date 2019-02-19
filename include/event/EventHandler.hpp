#pragma once

#include "Event.hpp"

namespace zero {
namespace event {

	class EventHandler {
	public:
		virtual ~EventHandler() = 0;

		virtual void OnSystemEvent(std::shared_ptr<SystemEvent> event) {}
		virtual void OnMouseEvent(std::shared_ptr<MouseEvent> event) {}
		virtual void OnKeyboardEvent(std::shared_ptr<KeyboardEvent> event) {}
		virtual void OnUIEvent(std::shared_ptr<UIEvent> event) {}
		virtual void OnWindowEvent(std::shared_ptr<WindowEvent> event) {}

	}; // abstract class EventHandler


} // namespace event
} // namespace zero