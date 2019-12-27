#include "core/Engine.hpp"
#include "render/RenderSystem.hpp"

using namespace zero;

Engine::Engine(EngineConfig  engine_config)
: engine_config_(std::move(engine_config))
, time_delta_()
, core_engine_(std::make_unique<CoreEngine>())
, game_systems_()
{
    // TODO: Pull core systems (e.g. Rendering, Physics, etc) out of this vector
    // Their ticks should be independent of the game system ticks
    // game_systems_ should contain game related systems (e.g. update scoreboard, damage calculations, etc)
    game_systems_.push_back(std::make_unique<render::RenderSystem>(GetCoreEngine(), engine_config_.render_system_config_));
}

void Engine::Initialize() {
    for (const auto& system : game_systems_) {
        system->Initialize();
    }
}

void Engine::Tick() {
    for (const auto& system : game_systems_) {
        system->PreUpdate();
    }
    for (const auto& system : game_systems_) {
        // TODO: Create TimeDelta class to maintain timing data between ticks
        system->Update(time_delta_);
    }
    for (const auto& system : game_systems_) {
        system->PostUpdate();
    }
}

void Engine::ShutDown() {
    for (const auto& system : game_systems_) {
        system->ShutDown();
    }
}

CoreEngine* Engine::GetCoreEngine() const {
    return core_engine_.get();
}