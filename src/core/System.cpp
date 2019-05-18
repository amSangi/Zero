#include "core/Engine.hpp"
#include "core/System.hpp"

using namespace zero;

System::System(std::shared_ptr<Engine> engine_)
: engine_(std::move(engine_)) {}

event::EventBus& System::GetEventBus() {
	return engine_->GetEventBus();
}

entt::registry<>& System::GetRegistry() {
	return engine_->GetRegistry();
}