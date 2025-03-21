#include "render/renderer/renderpass/EntityRenderPass.hpp"
#include "render/renderer/UniformBufferData.hpp"
#include "render/renderer/DrawCallComparator.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

const char* EntityRenderPass::kTitle = "EntityRenderPass";

EntityRenderPass::EntityRenderPass()
: camera_uniform_(nullptr)
, draw_calls_()
{
}

void EntityRenderPass::Initialize(IRenderHardware* rhi, std::shared_ptr<IUniformBuffer> camera_uniform)
{
    camera_uniform_ = std::move(camera_uniform);
}

void EntityRenderPass::Submit(std::unique_ptr<IDrawCall> draw_call)
{
    draw_calls_.push_back(std::move(draw_call));
}

void EntityRenderPass::Sort()
{
    std::sort(draw_calls_.begin(), draw_calls_.end(), DrawCallComparator::CompareDrawCalls);
}

void EntityRenderPass::Render(IRenderView* render_view, IRenderHardware* rhi)
{
    if (draw_calls_.empty())
    {
        LOG_DEBUG(kTitle, "No draw calls to render");
        return;
    }

    LOG_DEBUG(kTitle, "Rendering entities");
    rhi->BeginFrame(nullptr);

    const Camera& camera = render_view->GetCamera();
    rhi->SetViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    rhi->Clear();

    const CameraData camera_data{camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.position_};
    rhi->UpdateUniformData(camera_uniform_, &camera_data, sizeof(camera_data), 0);
    for (const std::unique_ptr<IDrawCall>& draw_call : draw_calls_)
    {
        draw_call->Draw(rhi);
    }

    rhi->EndFrame();
}

void EntityRenderPass::ClearDrawCalls()
{
    draw_calls_.clear();
}

} // namespace zero::render