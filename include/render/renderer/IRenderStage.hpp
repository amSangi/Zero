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
        virtual void Execute(IRenderView* render_view) = 0;
    }; // interface IRenderStage

    class RenderStage : public IRenderStage
    {
    public:
        virtual ~RenderStage() = default;
        void Execute(IRenderView* render_view) override = 0;

    protected:
        void RenderVolume(IRenderingManager* rendering_manager,
                          const Camera& camera,
                          const math::Matrix4x4& projection_matrix,
                          const math::Matrix4x4& view_matrix,
                          const Volume& volume);
        void RenderModel(IRenderingManager* rendering_manager, std::shared_ptr<Model> model);
        void RenderAnimatedModel(IRenderingManager* rendering_manager,
                                 std::shared_ptr<Model> model,
                                 IProgram* shader_program,
                                 const Animator& animator,
                                 const TimeDelta& time_delta);
    }; // abstract class RenderStage

} // namespace zero::render