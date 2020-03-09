#include "render/RenderSystem.hpp"
#include "render/opengl/GLRenderer.hpp"
#include "render/Components.hpp"

using namespace zero::render;

RenderSystem::RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config)
: zero::System(engine_core)
, config_(config)
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
    renderer_->Render(GetRegistry());
    window_->SwapBuffers();
}

void RenderSystem::PostUpdate() {
    renderer_->PostRender(GetRegistry());
}

void RenderSystem::ShutDown() {
    renderer_->ShutDown();
    window_->Cleanup();
}

zero::Component::Entity RenderSystem::CreateModelInstance(const std::string& model) {
    return renderer_->InstantiateModel(GetRegistry(), model);
}

zero::Component::Entity RenderSystem::CreatePrimitiveInstance(const PrimitiveInstance& primitive) {
    return renderer_->InstantiatePrimitive(GetRegistry(), primitive);
}
