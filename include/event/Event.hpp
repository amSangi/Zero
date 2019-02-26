#pragma once

#include "core/ZBase.hpp"

namespace zero {
namespace event {

    /**
     * @brief Enum encapsulating all game engine event types
     */
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

    /**
     * @brief The base Event class for all game engine events
     */
    class Event {
    public:
        explicit Event(EventType type)
        : type_(type)
        , accept_(0)
        , reserved_(0) {}

        Event(const Event& other) = default;
        virtual ~Event() = default;

        /**
         * @brief Get the Event Type
         * @return the event type
         */
        inline EventType GetType() const   { return type_;  }

        /**
         * @brief The receiver of the event wants this event
         * @return True if the accept flag has been set. False otherwise.
         */
        inline bool IsAccepted() const     { return accept_;   }

        /**
         * @brief Sets the accept flag so that the event is accepted by the receiver
         */
        inline void Accept()               { accept_ = 1;      }

        /**
         * @brief Clears the accept flag so that the event is ignored by the receiver
         */
        inline void Ignore()               { accept_ = 0;      }

    protected:
        /**
         * @brief The Event Type
         */
        EventType type_;

        /**
         * @brief The accept flag
         */
        uint8 accept_   : 1;

        /**
         * @brief Reserved flags
         */
        uint8 reserved_ : 7;

    }; // class Event

} // namespace event
} // namespace zero