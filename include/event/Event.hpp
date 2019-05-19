#pragma once

#include "core/ZBase.hpp"
#include "input/Key.hpp"
#include "input/MouseButton.hpp"
#include "EventType.hpp"

namespace zero::event {

    /**
     * @brief Window resize events
     */
    struct SizeEvent {
        uint32 width;   // New width, in pixels
        uint32 height;  // New height, in pixels
    }; // struct SizeEvent


    /**
     * @brief Keyboard events
     */
    struct KeyEvent {
        input::Key code;    // Code of the key that triggered this event
        bool       alt;     // Is the Alt key pressed?
        bool       control; // Is the Control key pressed?
        bool       shift;   // Is the Shift key pressed?
        bool       system;  // Is the System key pressed?
    }; // struct KeyEvent


    /**
     * @brief Text input events
     */
    struct TextEvent {
        uint32 unicode; // UTF-32 Unicode value of the character
    }; // struct TextEvent


    /**
     * @brief Mouse move events
     */
    struct MouseMoveEvent {
        int x; // X position of the mouse pointer, relative to the left of the owner window
        int y; // Y position of the mouse pointer, relative to the top of the owner window
    }; // struct MouseMoveEvent


    /**
     * @brief Mouse button events
     */
    struct MouseButtonEvent {
        input::MouseButton button; // Mouse button that triggered this event
        int                x;      // X position of the mouse pointer, relative to the left of the owner window
        int                y;      // Y position of the mouse pointer, relative to the top of the owner window
    }; // struct MouseButtonEvent


    /**
     * @brief Mouse wheel scroll events
     */
    struct MouseWheelScrollEvent {
        float delta; // Wheel offset (positive is up/left, negative is down/right). High-precision mice may use floats.
        int   x;     // X position of the mouse pointer, relative to the left of the owner window
        int   y;     // Y position of the mouse pointer, relative to the top of the owner window
    }; // struct MouseWheelScrollEvent


    /**
     * @brief System events passed to the different systems
     * @note Ensure the type is checked before accessing the correct event
     */
    struct Event {
        EventType type;
        union
        {
            SizeEvent size;
            KeyEvent key;
            TextEvent text;
            MouseMoveEvent mouse_move;
            MouseButtonEvent mouse_button;
            MouseWheelScrollEvent mouse_wheel_scroll;
        };
    }; // struct Event

} // namespace zero