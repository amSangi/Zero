#include "core/System.hpp"

using namespace zero;

System::System(EngineCore* engine_core)
: engine_core_(engine_core)
{}

event::EventBus& System::GetEventBus() {
	return engine_core_->GetEventBus();
}

entt::registry& System::GetRegistry() {
	return engine_core_->GetRegistry();
}