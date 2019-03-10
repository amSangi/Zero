#include "System.hpp"
#include "Engine.hpp"

using namespace zero;

event::EventBus& System::GetEventBus() {
	return engine_->GetEventBus();
}

entt::registry<>& System::GetRegistry() {
	return engine_->GetRegistry();
}

const sf::Window& System::GetWindow() const {
	return engine_->GetWindow();
}