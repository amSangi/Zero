#pragma once

#include <entt/entt.hpp>
#include "component/Animator.hpp"
#include "component/Camera.hpp"
#include "component/Volume.hpp"
#include "core/TimeDelta.hpp"
#include "render/IRenderView.hpp"
#include "render/renderer/IRenderingManager.hpp"

namespace zero::render
{
    class IRenderStage
    {
    public:
        virtual ~IRenderStage() = default;
        virtual void Initialize() = 0;
        virtual void Execute(IRenderView* render_view) = 0;
    }; // interface IRenderStage

    class RenderStage : public IRenderStage
    {
    public:
        RenderStage();
        virtual ~RenderStage() = default;
        void Execute(IRenderView* render_view) override = 0;

    protected:
        void Render(IRenderingContext* rendering_context,
                    IModelManager* model_manager,
                    const std::shared_ptr<IRenderable>& renderable,
                    const TimeDelta& time_delta);

        void RenderVolume(IRenderingManager* rendering_manager,
                          const Camera& camera,
                          const math::Matrix4x4& projection_matrix,
                          const math::Matrix4x4& view_matrix,
                          const Volume& volume);
    private:
        Material volume_material_;
        PrimitiveInstance volume_primitive_instance_;

    }; // abstract class RenderStage

} // namespace zero::render