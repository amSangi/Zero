#pragma once

#include <entt/entt.hpp>
#include "component/Animator.hpp"
#include "component/Camera.hpp"
#include "component/Volume.hpp"
#include "core/TimeDelta.hpp"
#include "render/IRenderingManager.hpp"

namespace zero::render
{
    class IRenderPass
    {
    public:
        virtual ~IRenderPass() = default;
        virtual void Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta) = 0;
    }; // interface IRenderPass

    class RenderPass : public IRenderPass
    {
    public:
        virtual ~RenderPass() = default;
        virtual void Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta) override = 0;

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
    }; // abstract class RenderPass

} // namespace zero::render