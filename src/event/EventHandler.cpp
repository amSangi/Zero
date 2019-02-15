#include <memory>
#include "EventHandler.hpp"
#include "EventBus.hpp"

using namespace zero::event;

EventHandler::EventHandler(std::shared_ptr<EventBus> event_bus)
: event_bus_(std::move(event_bus)) {}

EventHandler::~EventHandler() {
	event_bus_->RemoveEventHandler(std::shared_ptr<EventHandler>(this));
}
