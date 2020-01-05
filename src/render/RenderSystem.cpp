#include "render/RenderSystem.hpp"
#include "render/Components.hpp"
#include "render/opengl/GLRenderer.hpp"

using namespace zero::render;

RenderSystem::RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config)
: zero::System(engine_core)
, config_(config)
, animator_(nullptr)
, window_(std::make_unique<Window>(config.window_config_))
, renderer_(std::make_unique<GLRenderer>())
, propagator_(std::make_unique<Propagator>())
{}

void RenderSystem::Initialize() {
    ShutDown();
    window_->Initialize();
    renderer_->Initialize(config_);
}

void RenderSystem::PreUpdate() {
    // Propagate transform and bounding volume data
    auto& registry = GetRegistry();
    propagator_->PropagateTransform(registry);
    propagator_->PropagateVolume(registry);
}

void RenderSystem::Update(const TimeDelta& time_delta) {
    renderer_->Render(GetRegistry(), time_delta);
    window_->SwapBuffers();
}

void RenderSystem::PostUpdate() {
    // Cleanup the renderer after rendering
    renderer_->PostRender(GetRegistry());
}

void RenderSystem::ShutDown() {
    renderer_->ShutDown();
    window_->Cleanup();
}

void RenderSystem::PollWindowEvents() {
    window_->PollEvents(GetEventBus());
}

zero::Component::Entity RenderSystem::CreateModelInstance(const std::string& model) {
    return renderer_->InstantiateModel(GetRegistry(), model);
}