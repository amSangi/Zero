#include "render/IRenderPass.hpp"

namespace zero::render
{

void RenderPass::RenderVolume(IRenderingManager* rendering_manager,
                              const Camera& camera,
                              const math::Matrix4x4& projection_matrix,
                              const math::Matrix4x4& view_matrix,
                              const Volume& volume)
{
    // TODO: Implement volume wireframe rendering
}

void RenderPass::RenderModel(IRenderingManager* rendering_manager, std::shared_ptr<Model> model)
{
    // TODO: Implement model rendering
}

void RenderPass::RenderAnimatedModel(IRenderingManager* rendering_manager,
                                     std::shared_ptr<Model> model,
                                     IProgram* shader_program,
                                     const Animator& animator,
                                     const TimeDelta& time_delta)
{
    // TODO: Implement animated model rendering
}

} // namespace zero::render

