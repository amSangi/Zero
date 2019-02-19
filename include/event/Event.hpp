#pragma once

namespace zero {
namespace event {

	enum EventType {

		/* ********** System ********** */

		NONE,
		TIMER,
		ENGINE_START,
		ENGINE_STOP,
		ENGINE_RESUME,
		ENGINE_PAUSE,

		/* ********** Mouse ********** */

		MOUSE_BUTTON_LEFT_PRESS,
		MOUSE_BUTTON_LEFT_RELEASE,
		MOUSE_BUTTON_LEFT_CLICK,
		MOUSE_BUTTON_RIGHT_PRESS,
		MOUSE_BUTTON_RIGHT_RELEASE,
		MOUSE_BUTTON_RIGHT_CLICK,
		MOUSE_SCROLL,
		MOUSE_MOVE,
		MOUSE_DRAG,
		MOUSE_ENTER,
		MOUSE_EXIT,
		MOUSE_CONNECT,
		MOUSE_DISCONNECT,


		/* ********** Keyboard ********** */

		KEY_PRESSED,
		KEY_RELEASED,
		KEYBOARD_CONNECT,
		KEYBOARD_DISCONNECT,

		/* ********** UI ********** */

		WIDGET_PRESS,
		WIDGET_RELEASE,
		WIDGET_CLICK,
		WIDGET_SCROLL,
		WIDGET_DRAG,
		WIDGET_FOCUS_ENTER,
		WIDGET_FOCUS_EXIT,

		/* ********** Window ********** */

		WINDOW_RESIZE,
		LAYOUT_CHANGE,
		RESOLUTION_CHANGE,

	}; // enum EventType

	struct Event {
		virtual ~Event() = default;
		EventType type_;
	}; // struct Event

	struct SystemEvent : public Event {
		~SystemEvent() override = default;
	}; // struct SystemEvent

	struct MouseEvent : public Event {
		~MouseEvent() override = default;
	}; // struct MouseEvent

	struct KeyboardEvent : public Event {
		~KeyboardEvent() override = default;
	}; // struct KeyboardEvent

	struct UIEvent : public Event {
		~UIEvent() override = default;
	}; // struct UIEvent

	struct WindowEvent : public Event {
		~WindowEvent() override = default;
	}; // struct WindowEvent


} // namespace event
} // namespace zero