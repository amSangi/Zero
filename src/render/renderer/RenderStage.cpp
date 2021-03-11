#include "render/renderer/IRenderStage.hpp"

namespace zero::render
{

void RenderStage::RenderVolume(IRenderingManager* rendering_manager,
                              const Camera& camera,
                              const math::Matrix4x4& projection_matrix,
                              const math::Matrix4x4& view_matrix,
                              const Volume& volume)
{
    // TODO: Implement volume wireframe rendering
}

void RenderStage::RenderModel(IRenderingManager* rendering_manager, std::shared_ptr<Model> model)
{
    // TODO: Implement model rendering
}

void RenderStage::RenderAnimatedModel(IRenderingManager* rendering_manager,
                                     std::shared_ptr<Model> model,
                                     IProgram* shader_program,
                                     const Animator& animator,
                                     const TimeDelta& time_delta)
{
    // TODO: Implement animated model rendering
}

} // namespace zero::render

