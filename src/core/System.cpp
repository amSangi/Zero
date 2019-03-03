#include "System.hpp"
#include "Engine.hpp"

using namespace zero;

event::EventBus& System::GetEventBus() {
	return engine_->GetEventBus();
}

entt::registry<>& System::GetRegistry() {
	return engine_->GetRegistry();
}

ui::Window* System::GetWindow(){
	return engine_->GetWindow();
}