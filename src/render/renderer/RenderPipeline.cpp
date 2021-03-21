#include "render/renderer/RenderPipeline.hpp"

namespace zero::render
{

void RenderPipeline::AddRenderStage(std::unique_ptr<IRenderStage> render_stage)
{
    render_stages_.push_back(std::move(render_stage));
}

void RenderPipeline::Initialize()
{
    for (std::unique_ptr<IRenderStage>& render_stage : render_stages_)
    {
        render_stage->Initialize();
    }
}

void RenderPipeline::Render(std::unique_ptr<IRenderView> render_view)
{
    for (std::unique_ptr<IRenderStage>& render_stage : render_stages_)
    {
        render_stage->Execute(render_view.get());
    }
}

} // namespace zero::render

