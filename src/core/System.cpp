#include "core/Engine.hpp"
#include "core/System.hpp"

using namespace zero;

System::System(CoreEngine* core_engine)
: core_engine_(core_engine) {}

event::EventBus& System::GetEventBus() {
	return core_engine_->GetEventBus();
}

entt::registry& System::GetRegistry() {
	return core_engine_->GetRegistry();
}