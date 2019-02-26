#pragma once

#include <deque>
#include <memory>
#include <vector>
#include "Event.hpp"
#include "EventHandler.hpp"

namespace zero {
namespace event {

    class EventBus {
        using EventPtr = std::shared_ptr<Event>;
        using EventHandlerPtr = std::shared_ptr<EventHandler>;
    public:
        EventBus() = default;
        ~EventBus() = default;

        EventBus(const EventBus&) = delete;
        EventBus& operator=(const EventBus&) = delete;
        EventBus(EventBus&&) = delete;
        EventBus& operator=(EventBus&&) = delete;

        void AddEventHandler(EventHandlerPtr handler);
        void RemoveEventHandler(EventHandlerPtr handler);

        void Post(EventPtr event);
        void Dispatch(EventPtr event);
        void DispatchEvents();


    private:
        std::vector<EventHandlerPtr> handlers_;
        std::deque<EventPtr> event_queue_;

    }; // class EventBus

} // namespace event
} // namespace zero