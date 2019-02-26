#pragma once

class Event;

namespace zero {
namespace event {

    class EventHandler {
    public:
        virtual ~EventHandler() = 0;
        virtual void OnEvent(Event* event) = 0;
    }; // interface EventHandler


} // namespace event
} // namespace zero