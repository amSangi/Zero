#include "render/RenderSystem.hpp"
#include "render/opengl/GLRenderer.hpp"
#include "render/VolumePropagator.hpp"
#include "render/Instantiator.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

const char* RenderSystem::kTitle = "RenderSystem";

RenderSystem::RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config)
: zero::System(engine_core)
, config_(config)
, window_(std::make_unique<Window>(config.window_config_))
, renderer_(std::make_unique<GLRenderer>(engine_core))
{
    LOG_VERBOSE(kTitle, "RenderSystem instance constructed");
}

void RenderSystem::Initialize()
{
    LOG_VERBOSE(kTitle, "Initializing window");
    window_->Initialize();
    LOG_VERBOSE(kTitle, "Initializing renderer");
    renderer_->Initialize(config_);
}

void RenderSystem::PreUpdate()
{
}

void RenderSystem::Update(const TimeDelta& time_delta)
{
    LOG_VERBOSE(kTitle, "Rendering entities");
    renderer_->Render(time_delta);
    LOG_VERBOSE(kTitle, "Swapping buffers");
    window_->SwapBuffers();
}

void RenderSystem::PostUpdate()
{
    renderer_->PostRender();
    // Apply transforms to bounding volumes and expand parent volumes
    LOG_VERBOSE(kTitle, "Propagating entity volumes");
    VolumePropagator::PropagateVolume(GetCore()->GetRegistry());
}

void RenderSystem::ShutDown()
{
    LOG_VERBOSE(kTitle, "Shutting down renderer");
    renderer_->ShutDown();
    LOG_VERBOSE(kTitle, "Cleaning up window and graphics context");
    window_->Cleanup();
}

Entity RenderSystem::CreateModelInstance(const std::string& model_filename)
{
    auto model = renderer_->GetModel(model_filename);
    if (model.expired())
    {
        LOG_WARN(kTitle, "Instantiating a model that has expired. Model: " + model_filename);
        return NullEntity;
    }
    LOG_VERBOSE(kTitle, "Instantiating a new 3D model instance");
    return Instantiator::InstantiateModel(GetCore()->GetRegistry(), model.lock());
}

Entity RenderSystem::CreatePrimitiveInstance(const PrimitiveInstance& primitive)
{
    LOG_VERBOSE(kTitle, "Instantiating a new primitive instance");
    return Instantiator::InstantiatePrimitive(GetCore()->GetRegistry(), primitive);
}

Entity RenderSystem::CreateLightInstance(const Light& light, Entity entity)
{
    LOG_VERBOSE(kTitle, "Instantiating a new light instance");
    return Instantiator::InstantiateLight(GetCore()->GetRegistry(), light, entity);
}

} // namespace zero::render