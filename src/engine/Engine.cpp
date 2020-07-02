#include "engine/Engine.hpp"
#include "core/TransformPropagator.hpp"
#include <SDL_events.h>

namespace zero
{

const char* Engine::kTitle = "Engine";

Engine::Engine(EngineConfig  engine_config)
: engine_config_(std::move(engine_config))
, time_delta_()
, engine_core_(std::make_unique<EngineCore>())
, render_system_(std::make_unique<render::RenderSystem>(GetEngineCore(), engine_config_.render_system_config_))
, game_systems_()
, entity_instantiator_(std::make_unique<EntityInstantiator>(engine_core_->GetRegistry(), render_system_.get()))
, is_done_(false)
{
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Engine instance constructed");
}

void Engine::Initialize()
{
    engine_core_->GetFileManager().Initialize();
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Initializing systems");
    render_system_->Initialize();
    for (const auto& system : game_systems_)
    {
        system->Initialize();
    }
}

void Engine::Tick()
{
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Tick Begin");

    render_system_->PreUpdate();
    for (const auto& system : game_systems_)
    {
        system->PreUpdate();
    }

    TickEvents();

    render_system_->Update(time_delta_);
    for (const auto& system : game_systems_)
    {
        system->Update(time_delta_);
    }

    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Propagating entity transforms");
    TransformPropagator::PropagateTransform(engine_core_->GetRegistry());

    render_system_->PostUpdate();
    for (const auto& system : game_systems_)
    {
        system->PostUpdate();
    }

    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Clearing cached entity transformations");
    TransformPropagator::ClearCachedTransformations(engine_core_->GetRegistry());

    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Tick End");
}

void Engine::ShutDown()
{
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Shutting down systems");
    for (const auto& system : game_systems_)
    {
        system->ShutDown();
    }
    render_system_->ShutDown();
}

bool Engine::IsDone() const
{
    return is_done_;
}

EngineCore* Engine::GetEngineCore() const
{
    return engine_core_.get();
}

void Engine::TickEvents()
{
    EventBus& event_bus = engine_core_->GetEventBus();
    SDL_Event event{};
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            is_done_ = true;
        }
        event_bus.Dispatch(event);
    }
}

} // namespace zero