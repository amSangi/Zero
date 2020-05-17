#include "core/Engine.hpp"
#include "core/TransformPropagator.hpp"

namespace zero
{

Engine::Engine(EngineConfig  engine_config)
: engine_config_(std::move(engine_config))
, time_delta_()
, engine_core_(std::make_unique<EngineCore>())
, render_system_(std::make_unique<render::RenderSystem>(GetEngineCore(), engine_config_.render_system_config_))
, game_systems_()
{
}

void Engine::Initialize() {
    render_system_->Initialize();
    for (const auto& system : game_systems_)
    {
        system->Initialize();
    }
}

void Engine::Tick() {
    render_system_->PreUpdate();
    for (const auto& system : game_systems_)
    {
        system->PreUpdate();
    }

    render_system_->Update(time_delta_);
    for (const auto& system : game_systems_)
    {
        system->Update(time_delta_);
    }

    TransformPropagator::PropagateTransform(engine_core_->GetRegistry());
    render_system_->PostUpdate();
    for (const auto& system : game_systems_)
    {
        system->PostUpdate();
    }
    TransformPropagator::ClearCachedTransformations(engine_core_->GetRegistry());
}

void Engine::ShutDown()
{
    for (const auto& system : game_systems_)
    {
        system->ShutDown();
    }
    render_system_->ShutDown();
}

EngineCore* Engine::GetEngineCore() const
{
    return engine_core_.get();
}

Entity Engine::InstantiateModel(const std::string& model_filename)
{
    return render_system_->CreateModelInstance(model_filename);
}

Entity Engine::InstantiatePrimitive(render::PrimitiveInstance primitive)
{
    return render_system_->CreatePrimitiveInstance(primitive);
}

Entity Engine::InstantiateLight(render::Light light, Entity entity)
{
    return render_system_->CreateLightInstance(light, entity);
}

} // namespace zero