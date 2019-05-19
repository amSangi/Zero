#pragma once

#include "Event.hpp"

namespace zero::event {

    class EventHandler {
    public:
        virtual ~EventHandler() = 0;
        virtual void OnEvent(const Event& event) = 0;
    }; // interface EventHandler


} // namespace zero