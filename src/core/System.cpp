#include "Engine.hpp"
#include "System.hpp"

using namespace zero;

System::System(std::shared_ptr<Engine> engine_)
: engine_(std::move(engine_)) {}

event::EventBus& System::GetEventBus() {
	return engine_->GetEventBus();
}

entt::registry<>& System::GetRegistry() {
	return engine_->GetRegistry();
}

const sf::Window& System::GetWindow() const {
	return engine_->GetWindow();
}