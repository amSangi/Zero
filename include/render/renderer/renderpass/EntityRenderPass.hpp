#pragma once

#include "component/Camera.hpp"
#include "render/renderer/IRenderPass.hpp"

namespace zero::render
{

	class EntityRenderPass : public IRenderPass
	{
	public:
		explicit EntityRenderPass();
		~EntityRenderPass() override = default;
		void Initialize(IRenderHardware* rhi, std::shared_ptr<IUniformBuffer> camera_uniform) override;
		void Submit(std::unique_ptr<IDrawCall> draw_call) override;
		void Sort() override;
		void Render(IRenderView* render_view, IRenderHardware* rhi) override;
		void ClearDrawCalls() override;
	private:
		std::shared_ptr<IUniformBuffer> camera_uniform_;
		std::vector<std::unique_ptr<IDrawCall>> draw_calls_;
	};

} // namespace zero::render