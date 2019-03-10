#pragma once

#include "core/ZBase.hpp"
#include "EventType.hpp"

namespace zero {
namespace event {

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