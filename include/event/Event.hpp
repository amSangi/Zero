#pragma once

#include "core/ZeroBase.hpp"
#include "input/Key.hpp"
#include "input/MouseButton.hpp"
#include "EventType.hpp"

namespace zero::event
{

    /**
     * @brief Window resize events
     */
    struct SizeEvent
    {
        /**
         * @brief New width, in pixels
         */
        uint32 width_;
        /**
         * @brief New height, in pixels
         */
        uint32 height_;
    }; // struct SizeEvent


    /**
     * @brief Keyboard events
     */
    struct KeyEvent
    {
        /**
         * @brief Code of the key that triggered this event
         */
        input::Key code_;
        /**
         * @brief Is the ALT key pressed?
         */
        bool       alt_;
        /**
         * @brief Is the CONTROL key pressed?
         */
        bool       control_;
        /**
         * @brief Is the SHIFT key pressed?
         */
        bool       shift_;
        /**
         * @brief Is the SYSTEM key pressed?
         */
        bool       system_;
    }; // struct KeyEvent


    /**
     * @brief Text input events
     */
    struct TextEvent
    {
        /**
         * @brief UTF-32 Unicode value of the character
         */
        uint32 unicode_;
    }; // struct TextEvent


    /**
     * @brief Mouse move events
     */
    struct MouseMoveEvent
    {
        /**
         * @brief X position of the mouse pointer, relative to the left of the owner window
         */
        int x_;
        /**
         * @brief Y position of the mouse pointer, relative to the top of the owner window
         */
        int y_;
    }; // struct MouseMoveEvent


    /**
     * @brief Mouse button events
     */
    struct MouseButtonEvent
    {
        /**
         * @brief Mouse button that triggered this event
         */
        input::MouseButton button_;
        /**
         * @brief X position of the mouse pointer, relative to the left of the owner window
         */
        int                x_;
        /**
         * @brief Y position of the mouse pointer, relative to the top of the owner window
         */
        int                y_;
    }; // struct MouseButtonEvent


    /**
     * @brief Mouse wheel scroll events
     */
    struct MouseWheelScrollEvent
    {
        /**
         * @brief Wheel offset (positive is up/left, negative is down/right). High-precision mice may use floats.
         */
        float delta_;
        /**
         * @brief X position of the mouse pointer, relative to the left of the owner window
         */
        int   x_;
        /**
         * @brief Y position of the mouse pointer, relative to the top of the owner window
         */
        int   y_;
    }; // struct MouseWheelScrollEvent


    /**
     * @brief System events passed to the different systems
     * @note Ensure the type is checked before accessing the correct event
     */
    struct Event
    {
        /**
         * @brief The event type
         */
        EventType type_;

        /**
         * @brief The event body
         */
        union
        {
            SizeEvent size_;
            KeyEvent key_;
            TextEvent text_;
            MouseMoveEvent mouse_move_;
            MouseButtonEvent mouse_button_;
            MouseWheelScrollEvent mouse_wheel_scroll_;
        };
    }; // struct Event

} // namespace zero::event