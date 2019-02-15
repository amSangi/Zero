#include <vector>
#include "EventBus.hpp"

using namespace zero::event;

void EventBus::AddEventHandler(EventBus::EventHandlerPtr handler) {
	handlers_.push_back(handler);
}

void EventBus::RemoveEventHandler(EventBus::EventHandlerPtr handler) {
	handlers_.erase(std::remove(handlers_.begin(),
	                            handlers_.end(),
	                            handler),
	                handlers_.end());
}

void EventBus::Enqueue(EventBus::EventPtr event) {
	event_queue_.push_back(std::move(event));
}

void EventBus::Dispatch(EventBus::EventPtr event) {
	for (const auto& handler : handlers_) {
		switch (event->type) {
			/* ********** System ********** */
			case NONE:
			case TIMER:
			case ENGINE_START:
			case ENGINE_STOP:
			case ENGINE_RESUME:
			case ENGINE_PAUSE:
				handler->OnSystemEvent(std::dynamic_pointer_cast<SystemEvent>(event));
				break;
			/* ********** Mouse ********** */
			case MOUSE_BUTTON_LEFT_PRESS:
			case MOUSE_BUTTON_LEFT_RELEASE:
			case MOUSE_BUTTON_LEFT_CLICK:
			case MOUSE_BUTTON_RIGHT_PRESS:
			case MOUSE_BUTTON_RIGHT_RELEASE:
			case MOUSE_BUTTON_RIGHT_CLICK:
			case MOUSE_SCROLL:
			case MOUSE_MOVE:
			case MOUSE_DRAG:
			case MOUSE_ENTER:
			case MOUSE_EXIT:
			case MOUSE_CONNECT:
			case MOUSE_DISCONNECT:
				handler->OnMouseEvent(std::dynamic_pointer_cast<MouseEvent>(event));
				break;
			/* ********** Keyboard ********** */
			case KEY_PRESSED:
			case KEY_RELEASED:
			case KEYBOARD_CONNECT:
			case KEYBOARD_DISCONNECT:
				handler->OnKeyboardEvent(std::dynamic_pointer_cast<KeyboardEvent>(event));
				break;
			/* ********** UI ********** */
			case WIDGET_PRESS:
			case WIDGET_RELEASE:
			case WIDGET_CLICK:
			case WIDGET_SCROLL:
			case WIDGET_DRAG:
			case WIDGET_FOCUS_ENTER:
			case WIDGET_FOCUS_EXIT:
				handler->OnUIEvent(std::dynamic_pointer_cast<UIEvent>(event));
				break;
			/* ********** Window ********** */
			case WINDOW_RESIZE:
			case LAYOUT_CHANGE:
			case RESOLUTION_CHANGE:
				handler->OnWindowEvent(std::dynamic_pointer_cast<WindowEvent>(event));
				break;
		}
	}
}

void EventBus::DispatchEvents() {
	while (!event_queue_.empty()) {
		EventPtr event = event_queue_.front();
		event_queue_.pop_front();
		Dispatch(std::move(event));
	}
}