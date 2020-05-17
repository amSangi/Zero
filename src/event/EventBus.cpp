#include <algorithm>
#include "event/EventBus.hpp"

namespace zero::event
{

void EventBus::AddEventHandler(const EventHandlerPtr& handler)
{
	handlers_.push_back(handler);
}

void EventBus::RemoveEventHandler(const EventHandlerPtr& handler)
{
	handlers_.erase(std::remove(handlers_.begin(),
	                            handlers_.end(),
	                            handler),
	                handlers_.end());
}

void EventBus::Post(const Event& event) {
	event_queue_.push_back(event);
}

void EventBus::Dispatch(const Event& event) {
	for (const auto& handler : handlers_)
	{
		handler->OnEvent(event);
	}
}

void EventBus::DispatchEvents()
{
	while (!event_queue_.empty())
	{
		Event event = event_queue_.front();
		event_queue_.pop_front();
		Dispatch(event);
	}
}

} // namespace zero::event