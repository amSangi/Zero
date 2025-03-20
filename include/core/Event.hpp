#pragma once

#include "core/ZeroBase.hpp"
#include "core/KeyCode.hpp"
#include "core/MouseButton.hpp"

namespace zero
{
    /**
     * @brief Container for multiple different window events
     */
    struct WindowEvent
    {
        /**
         * @brief The window event type
         * The 'x_' and 'y_' fields are only used for some of these types
         */
        enum class Type
        {
            WINDOW_SHOWN = 0,                    ///< Window has been shown
            WINDOW_HIDDEN = 1,                   ///< Window has been hidden
            WINDOW_EXPOSED = 2,                  ///< Window has been exposed and should be redrawn
            WINDOW_MOVED = 3,                    ///< Window has moved to x_, y_
            WINDOW_RESIZED = 4,                  ///< Window has resized to x_ * y_; this always is preceded by WINDOW_SIZE_CHANGED
            WINDOW_SIZE_CHANGED = 5,             ///< Window size has changed
            WINDOW_MINIMIZED = 6,                ///< Window has been minimized
            WINDOW_MAXIMIZED = 7,                ///< Window has been maximized
            WINDOW_RESTORED = 8,                 ///< Window has been restored to the normal size and position
            WINDOW_MOUSE_ENTER = 9,              ///< Window has gained the mouse focus
            WINDOW_MOUSE_LEAVE = 10,             ///< Window has lost the mouse focus
            WINDOW_KEYBOARD_FOCUS_GAINED = 11,   ///< Window has gained the keyboard focus
            WINDOW_KEYBOARD_FOCUS_LOST = 12,     ///< Window has lost the keyboard focus
            WINDOW_CLOSE = 13,                   ///< Window has been requested to close
        }; // enum class Type

        /**
         * @brief The specific window event type
         */
        Type type_;
        /**
         * @brief Dependent on the specific window event type
         */
        int32 x_;
        /**
         * @brief Dependent on the specific window event type
         */
        int32 y_;

    }; // struct WindowEvent

    /**
     * @brief Mouse pressed/released event information
     */
    struct MouseButtonEvent
    {
        /**
         * @brief The mouse button that triggered the vent
         */
        MouseButton mouse_button;
        /**
         * @brief The x coordinate of the mouse relative to the window
         */
        float x_;
        /**
         * @brief The y coordinate of the mouse relative to the window
         */
        float y_;
        /**
         * @brief The number of times the button was clicked
         */
        uint8 click_count_;

    }; // struct MouseButtonEvent

    /**
     * @brief Mouse movement event
     */
    struct MouseMoveEvent
    {
        /**
         * @brief The x coordinate of the mouse relative to the window
         */
        float x_;
        /**
         * @brief The y coordinate of the mouse relative to the window
         */
        float y_;
        /**
         * @brief Relative motion in the x direction
         */
        float relative_x_motion_;
        /**
         * @brief Relative motion in the y direction
         */
        float relative_y_motion_;

    }; // struct MouseMoveEvent

    /**
     * @brief Mouse wheel event
     */
    struct MouseWheelEvent
    {
        /**
         * @brief The amount scrolled horizontally.
         * Positive indicates the scroll is to the right
         * Negative indicates the scroll is to the left
         */
        float x_;
        /**
         * @brief The amount scrolled vertically.
         * Positive indicates the scroll is away from the user
         * Negative indicates the scroll is towards the user
         */
        float y_;

    }; // struct MouseWheelEvent


} // namespace zero