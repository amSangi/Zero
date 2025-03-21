#include "render/renderer/renderpass/CascadedShadowMapRenderPass.hpp"
#include "render/renderer/UniformBufferData.hpp"
#include "render/renderer/DrawCallComparator.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

const char* CascadedShadowMapRenderPass::kTitle = "CascadedShadowMapRenderPass";

CascadedShadowMapRenderPass::CascadedShadowMapRenderPass(uint32 cascade_index)
: cascade_index_(cascade_index)
, camera_uniform_(nullptr)
, draw_calls_()
{
}

void CascadedShadowMapRenderPass::Initialize(IRenderHardware* rhi, std::shared_ptr<IUniformBuffer> camera_uniform)
{
    camera_uniform_ = std::move(camera_uniform);
}

void CascadedShadowMapRenderPass::Submit(std::unique_ptr<IDrawCall> draw_call)
{
    draw_calls_.push_back(std::move(draw_call));
}

void CascadedShadowMapRenderPass::Sort()
{
    std::sort(draw_calls_.begin(), draw_calls_.end(), DrawCallComparator::CompareDrawCalls);
}

void CascadedShadowMapRenderPass::Render(IRenderView* render_view, IRenderHardware* rhi)
{
    if (draw_calls_.empty())
    {
        LOG_DEBUG(kTitle, "No draw calls to render for cascade index: " + std::to_string(cascade_index_));
        return;
    }

    LOG_DEBUG(kTitle, "Rendering cascade index: " + std::to_string(cascade_index_));

    const std::vector<std::shared_ptr<IFrameBuffer>> shadow_map_frame_buffers = rhi->GetShadowMapFrameBuffers();
    const CascadedShadowMap cascaded_shadow_map = render_view->GetCascadedShadowMap();
    assert(shadow_map_frame_buffers.size() == cascaded_shadow_map.GetCascadeCount());

    const std::vector<math::Matrix4x4> light_view_matrices = cascaded_shadow_map.GetLightViewMatrices();
    const std::vector<math::Matrix4x4> light_projection_matrices = cascaded_shadow_map.GetProjectionMatrices();

    rhi->BeginFrame(shadow_map_frame_buffers[cascade_index_]);
    rhi->SetViewport(0, 0, Constants::kShadowMapWidth, Constants::kShadowMapHeight);
    rhi->Clear();
    rhi->SetCullMode(IRenderHardware::CullMode::CULL_MODE_BACK);
    rhi->SetFillMode(IRenderHardware::FillMode::FILL_MODE_SOLID);

    const CameraData camera_data{light_projection_matrices[cascade_index_], light_view_matrices[cascade_index_], math::Vec3f::Zero()};
    rhi->UpdateUniformData(camera_uniform_, &camera_data, sizeof(camera_data), 0);
    for (const std::unique_ptr<IDrawCall>& draw_call : draw_calls_)
    {
        draw_call->Draw(rhi);
    }

    rhi->EndFrame();
}

void CascadedShadowMapRenderPass::ClearDrawCalls()
{
    draw_calls_.clear();
}

} // namespace zero::render