#pragma once

#include <deque>
#include <memory>
#include <vector>
#include "Event.hpp"
#include "EventHandler.hpp"

namespace zero::event {

    class EventBus {
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

        void Post(const Event& event);
        void Dispatch(const Event& event);
        void DispatchEvents();


    private:
        std::vector<EventHandlerPtr> handlers_;
        std::deque<Event> event_queue_;

    }; // class EventBus

} // namespace zero::event