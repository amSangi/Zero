#pragma once

namespace zero {
namespace event {

    class Event;

    class EventHandler {
    public:
        virtual ~EventHandler() = 0;
        virtual void OnEvent(Event* event) = 0;
    }; // interface EventHandler


} // namespace event
} // namespace zero