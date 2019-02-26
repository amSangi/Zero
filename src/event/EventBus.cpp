#include "EventBus.hpp"
#include "input/KeyEvent.hpp"
#include "input/MouseEvent.hpp"
#include "SystemEvent.hpp"
#include "UIEvent.hpp"
#include "WindowEvent.hpp"

using namespace zero::event;

void EventBus::AddEventHandler(EventBus::EventHandlerPtr handler) {
	handlers_.push_back(handler);
}

void EventBus::RemoveEventHandler(EventBus::EventHandlerPtr handler) {
	handlers_.erase(std::remove(handlers_.begin(),
	                            handlers_.end(),
	                            handler),
	                handlers_.end());
}

void EventBus::Post(EventBus::EventPtr event) {
	event_queue_.push_back(std::move(event));
}

void EventBus::Dispatch(EventBus::EventPtr event) {
	for (const auto& handler : handlers_) {
		if (event->IsAccepted()) {
			handler->OnEvent(event.get());
		}
	}
}

void EventBus::DispatchEvents() {
	while (!event_queue_.empty()) {
		EventPtr event = event_queue_.front();
		event_queue_.pop_front();
		Dispatch(std::move(event));
	}
}