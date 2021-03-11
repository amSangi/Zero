#pragma once

#include "render/renderer/IRenderStage.hpp"
#include "render/renderer/IRenderingManager.hpp"

namespace zero::render
{
    class EntityRenderStage : public RenderStage
    {
    public:
        explicit EntityRenderStage(IRenderingManager* rendering_manager);
        ~EntityRenderStage() override = default;

        void Execute(IRenderView* render_view) override;

    private:
        IRenderingManager* rendering_manager_;
        std::shared_ptr<ISampler> diffuse_map_sampler_;
        std::shared_ptr<ISampler> shadow_map_sampler_;

    }; // class EntityRenderStage

} // namespace zero::render