#include "render/Components.hpp"
#include "render/opengl/GLRenderer.hpp"
#include "render/RenderSystem.hpp"

using namespace zero::render;

RenderSystem::RenderSystem(std::shared_ptr<Engine> engine, RenderSystemConfig config)
: zero::System(std::move(engine))
, config_(std::move(config))
, window_(nullptr)
, renderer_(nullptr)
, propagator_(nullptr)
{
}

void RenderSystem::Initialize() {
    window_ = std::make_unique<Window>(config_.window_config_);
    window_->Initialize();
    switch (config_.window_config_.api_)
    {
        case GraphicsAPI::GRAPHICS_OPENGL:
        default:
            renderer_ = std::make_unique<GLRenderer>();
            break;
    }
    renderer_->Initialize();
    propagator_ = std::make_unique<Propagator>();
}

void RenderSystem::PreUpdate() {
    // Propagate transform and bounding volume data
    auto& registry = GetRegistry();
    propagator_->PropagateTransform(registry);
    propagator_->PropagateVolume(registry);
}

void RenderSystem::Update(float dt) {
    // Render entities
    renderer_->Render(GetRegistry(), dt);
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
