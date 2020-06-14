#pragma once

#include "core/Event.hpp"

namespace zero
{
    /**
     * @brief Handles different events
     */
    class EventHandler
    {
    public:
        virtual ~EventHandler() = default;

        /**
         * @brief Invoked when a window event occurs
         *
         * Check the event type before accessing the (x_, y_) fields to see if they are valid.
         *
         * @param window_event the window event
         */
        [[maybe_unused]] virtual void OnWindowEvent(const WindowEvent& window_event) {}

        /**
         * @brief Invoked when a key is pressed
         * @param key_code the pressed key
         */
        [[maybe_unused]] virtual void OnKeyPressed(KeyCode key_code) {}

        /**
         * @brief Invoked when a key is released
         * @param key_code the released key
         */
        [[maybe_unused]] virtual void OnKeyReleased(KeyCode key_code) {}

        /**
         * @brief Invoked when a mouse button is pressed
         * @param mouse_button_event the mouse button event information
         */
        [[maybe_unused]] virtual void OnMouseButtonPressed(const MouseButtonEvent& mouse_button_event) {}

        /**
         * @brief Invoked when a mouse button is released
         * @param mouse_button_event the mouse button event information
         */
        [[maybe_unused]] virtual void OnMouseButtonReleased(const MouseButtonEvent& mouse_button_event) {}

        /**
         * @brief Invoked when a mouse move event occurs
         * @param mouse_move_event the mouse move event
         */
        [[maybe_unused]] virtual void OnMouseMoved(const MouseMoveEvent& mouse_move_event) {}

        /**
         * @brief Invoked when a mouse wheel scroll event occurs
         * @param mouse_wheel_event the mouse wheel scroll event
         */
        [[maybe_unused]] virtual void OnMouseWheelScroll(const MouseWheelEvent& mouse_wheel_event) {}

    }; // class EventHandler


} // namespace zero