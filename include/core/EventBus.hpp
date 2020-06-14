#pragma once

#include <vector>
#include "core/Event.hpp"
#include "core/EventHandler.hpp"

// Forward declarations
union SDL_Event;
struct SDL_WindowEvent;
struct SDL_KeyboardEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseWheelEvent;

namespace zero
{
    // Forward declarations
    class System;

    /**
     * @brief Queues events and dispatches them to subscribed event handlers
     */
    class EventBus
    {
        using EventHandlerContainer = std::vector<EventHandler*>;
    public:
        EventBus() = default;
        ~EventBus() = default;

        /**
         * @brief Dispatch an event from SDL2
         * @param sdl_event the SDL event
         */
        void Dispatch(const SDL_Event& sdl_event);

        /**
         * @brief Register an event handler for the respective event type
         * @param event_handler the event handler
         */
        ///@{
        void RegisterWindowEventListener(EventHandler* event_handler);
        void RegisterKeyPressedListener(EventHandler* event_handler);
        void RegisterKeyReleasedListener(EventHandler* event_handler);
        void RegisterMouseButtonPressedListener(EventHandler* event_handler);
        void RegisterMouseButtonReleasedListener(EventHandler* event_handler);
        void RegisterMouseMoveListener(EventHandler* event_handler);
        void RegisterMouseWheelListener(EventHandler* event_handler);
        ///@}

    private:
        void DispatchWindowEvent(const SDL_WindowEvent* window_event);
        void DispatchKeyEvent(const SDL_KeyboardEvent* keyboard_event);
        void DispatchMouseButtonEvent(const SDL_MouseButtonEvent* mouse_button_event);
        void DispatchMouseMoveEvent(const SDL_MouseMotionEvent* mouse_motion_event);
        void DispatchMouseWheelEvent(const SDL_MouseWheelEvent* mouse_wheel_event);


        EventHandlerContainer window_event_listeners_;
        EventHandlerContainer key_pressed_listeners_;
        EventHandlerContainer key_released_listeners_;
        EventHandlerContainer mouse_button_pressed_listeners_;
        EventHandlerContainer mouse_button_released_listeners_;
        EventHandlerContainer mouse_move_listeners_;
        EventHandlerContainer mouse_wheel_listeners_;

    }; // class EventBus

} // namespace zero