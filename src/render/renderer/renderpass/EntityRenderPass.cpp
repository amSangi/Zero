#include "render/renderer/renderpass/EntityRenderPass.hpp"
#include "render/renderer/UniformBufferData.hpp"
#include "render/renderer/DrawCallComparator.hpp"

namespace zero::render
{

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
	rhi->BeginFrame(nullptr);

	const Camera& camera = render_view->GetCamera();
	rhi->SetViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);

	CameraData camera_data{camera.GetProjectionMatrix(), camera.GetViewMatrix(), camera.position_};
	rhi->UpdateUniformData(camera_uniform_, &camera_data, sizeof(camera_data), 0);
	rhi->BindUniformBuffer(camera_uniform_);

	for (const std::unique_ptr<IDrawCall>& render_call : draw_calls_)
	{
		render_call->Draw(rhi);
	}

	rhi->EndFrame();
}

void EntityRenderPass::ClearDrawCalls()
{
	draw_calls_.clear();
}

} // namespace zero::render