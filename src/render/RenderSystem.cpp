#include "render/RenderSystem.hpp"
#include "render/VolumePropagator.hpp"
#include "render/Instantiator.hpp"
#include "render/renderer/opengl/GLRenderingManager.hpp"
#include "render/renderer/pipeline/EntityRenderStage.hpp"
#include "render/renderer/pipeline/ShadowMapStage.hpp"
#include "render/renderer/pipeline/SkyDomeStage.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

const char* RenderSystem::kTitle = "RenderSystem";

RenderSystem::RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config)
: zero::System(engine_core)
, config_(config)
, rendering_manager_(std::make_unique<GLRenderingManager>())
, render_pipeline_(std::make_unique<RenderPipeline>())
, scene_manager_(std::make_unique<SceneManager>())
, window_(std::make_unique<Window>(config.window_config_))
{
    LOG_VERBOSE(kTitle, "RenderSystem instance constructed");
}

void RenderSystem::Initialize()
{
    window_->Initialize();
    render_pipeline_->AddRenderStage(std::make_unique<EntityRenderStage>(rendering_manager_.get()));
    render_pipeline_->AddRenderStage(std::make_unique<ShadowMapStage>(rendering_manager_.get()));
    render_pipeline_->AddRenderStage(std::make_unique<SkyDomeStage>(rendering_manager_.get()));

    // TODO: Initialize rendering manager components

}

void RenderSystem::PreUpdate()
{
}

void RenderSystem::Update(const TimeDelta& time_delta)
{

    /**
        1. TODO: Create RenderView
        2. TODO: Use RenderView in RenderPipeline
        3. TODO: Change RenderPass to RenderStage
        4. TODO: Create SceneManager
        5. TODO: Move culling and scene setup logic from GLRenderer to SceneManager
        6. TODO: Update RenderSystem to own a RenderPipeline and SceneManager

        //////////////////////////////////////////////////
        ////////// RenderPipeline
        /////////////////////////////////////////////////

        RenderPipeline::Render(RenderView render_view)
            for (const RenderStage& render_stage : render_stages_)
                render_stage->Render(render_view)

        RenderSystem::Update(registry)
            scene_manager->UpdateView(registry)
            render_pipeline->QueueRenderView(scene_manager->GetLatestView())
            render_pipeline->Render()
     */

    LOG_VERBOSE(kTitle, "Swapping buffers");
    window_->SwapBuffers();
}

void RenderSystem::PostUpdate()
{
    // Apply transforms to bounding volumes and expand parent volumes
    LOG_VERBOSE(kTitle, "Propagating entity volumes");
    VolumePropagator::PropagateVolume(GetCore()->GetRegistry());
}

void RenderSystem::ShutDown()
{
    LOG_VERBOSE(kTitle, "Cleaning up window and graphics context");
    window_->Cleanup();
}

Entity RenderSystem::CreateModelInstance(const std::string& model_filename)
{
    LOG_VERBOSE(kTitle, "Instantiating a new 3D model instance");
    auto model = rendering_manager_->GetModelManager()->GetModel(model_filename);
    return Instantiator::InstantiateModel(GetCore()->GetRegistry(), model);
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