#pragma once

#include <vector>
#include <memory>
#include <entt/entt.hpp>
#include "component/Camera.hpp"
#include "core/TimeDelta.hpp"
#include "render/IRenderView.hpp"
#include "render/renderer/IRenderStage.hpp"

namespace zero::render
{
    class RenderPipeline
    {
    public:
        RenderPipeline() = default;
        ~RenderPipeline() = default;

        void AddRenderStage(std::unique_ptr<IRenderStage> render_stage);
        void Render(std::unique_ptr<IRenderView> render_view);
    private:
        std::vector<std::unique_ptr<IRenderStage>> render_stages_;
    }; // class RenderPipeline

} // namespace zero::render