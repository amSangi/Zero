#include "render/RenderPipeline.hpp"
#include "render/IRenderPass.hpp"

namespace zero::render
{

void RenderPipeline::AddRenderPass(std::unique_ptr<IRenderPass> render_pass)
{
    render_passes_.push_back(std::move(render_pass));
}

void RenderPipeline::Execute(const Camera& camera, entt::registry& registry)
{
    for (std::unique_ptr<IRenderPass>& render_pass : render_passes_)
    {
        render_pass->Execute(camera, registry);
    }
}

} // namespace zero::render

