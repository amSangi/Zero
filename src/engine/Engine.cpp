#include "engine/Engine.hpp"
#include "core/TransformPropagator.hpp"

namespace zero
{

const char* Engine::kTitle = "Engine";

Engine::Engine(EngineConfig  engine_config)
: engine_config_(std::move(engine_config))
, time_delta_()
, engine_core_(std::make_unique<EngineCore>())
, render_system_(std::make_unique<render::RenderSystem>(GetEngineCore(), engine_config_.render_system_config_))
, game_systems_()
{
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Engine instance constructed")
}

void Engine::Initialize() {
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Initializing systems")
    render_system_->Initialize();
    for (const auto& system : game_systems_)
    {
        system->Initialize();
    }
}

void Engine::Tick() {
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Tick Begin")

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

    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Propagating entity transforms")
    TransformPropagator::PropagateTransform(engine_core_->GetRegistry());
    render_system_->PostUpdate();
    for (const auto& system : game_systems_)
    {
        system->PostUpdate();
    }
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Clearing cached entity transformations")
    TransformPropagator::ClearCachedTransformations(engine_core_->GetRegistry());

    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Tick End")
}

void Engine::ShutDown()
{
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Shutting down systems")
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
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, std::string("Instantiating 3D Model: ") + model_filename)
    return render_system_->CreateModelInstance(model_filename);
}

Entity Engine::InstantiatePrimitive(render::PrimitiveInstance primitive)
{
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Instantiating Primitive")
    return render_system_->CreatePrimitiveInstance(primitive);
}

Entity Engine::InstantiateLight(render::Light light, Entity entity)
{
    LOG_VERBOSE(GetEngineCore()->GetLogger(), kTitle, "Instantiating Light")
    return render_system_->CreateLightInstance(light, entity);
}

} // namespace zero