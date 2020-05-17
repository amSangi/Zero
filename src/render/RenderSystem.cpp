#include "render/RenderSystem.hpp"
#include "render/opengl/GLRenderer.hpp"
#include "render/PrimitiveInstance.hpp"
#include "render/VolumePropagator.hpp"
#include "render/Instantiator.hpp"

namespace zero::render
{

RenderSystem::RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config)
: zero::System(engine_core)
, config_(config)
, window_(std::make_unique<Window>(config.window_config_))
, renderer_(std::make_unique<GLRenderer>())
{
}

void RenderSystem::Initialize()
{
    ShutDown();
    window_->Initialize();
    renderer_->Initialize(config_);
}

void RenderSystem::PreUpdate() {}

void RenderSystem::Update(const TimeDelta& time_delta)
{
    renderer_->Render(GetRegistry());
    window_->SwapBuffers();
}

void RenderSystem::PostUpdate()
{
    auto& registry = GetRegistry();
    renderer_->PostRender(registry);
    // Apply transforms to bounding volumes and expand parent volumes
    VolumePropagator::PropagateVolume(registry);
}

void RenderSystem::ShutDown()
{
    renderer_->ShutDown();
    window_->Cleanup();
}

Entity RenderSystem::CreateModelInstance(const std::string& model_filename)
{
    auto model = renderer_->GetModel(model_filename);
    if (model.expired())
    {
        return NullEntity;
    }
    return Instantiator::InstantiateModel(GetRegistry(), model.lock());
}

Entity RenderSystem::CreatePrimitiveInstance(const PrimitiveInstance& primitive)
{
    return Instantiator::InstantiatePrimitive(GetRegistry(), primitive);
}

Entity RenderSystem::CreateLightInstance(const Light& light, Entity entity)
{
    return Instantiator::InstantiateLight(GetRegistry(), light, entity);
}

} // namespace zero::render