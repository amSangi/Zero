#include "render/pipeline/SkyDomePass.hpp"

namespace zero::render
{

SkyDomePass::SkyDomePass(IRenderingManager* rendering_manager)
: rendering_manager_(rendering_manager)
, sphere_mesh_(nullptr)
{
    IModelManager* model_manager = rendering_manager_->GetModelManager();
    PrimitiveInstance sphere_primitive_instance{Sphere{}};
    sphere_mesh_ = model_manager->GetPrimitive(sphere_primitive_instance);
}

void SkyDomePass::Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta)
{
    // Render the first active sky dome
    auto sky_dome_view = registry.view<const SkyDome>();
    for (Entity sky_dome_entity : sky_dome_view)
    {
        const SkyDome& sky_dome = sky_dome_view.get<const SkyDome>(sky_dome_entity);
        if (sky_dome.is_active_)
        {
            RenderSkyDome(camera, sky_dome);
            break;
        }
    }
}

void SkyDomePass::RenderSkyDome(const Camera& camera, const SkyDome& sky_dome) const
{
    IRenderingContext* rendering_context = rendering_manager_->GetRenderingContext();
    IShaderManager* shader_manager = rendering_manager_->GetShaderManager();
    IUniformManager* uniform_manager = rendering_manager_->GetUniformManager();

    IProgram* sky_dome_program = shader_manager->GenerateProgram(sky_dome);
    uniform_manager->SetSkyDomeUniforms(sky_dome_program, camera, sky_dome);

    // Begin rendering frame with default render target
    rendering_context->BeginFrame(nullptr);
    rendering_context->SetViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    rendering_context->Clear();

    rendering_context->SetFillMode(IRenderingContext::FillMode::SOLID);
    rendering_context->SetCullMode(IRenderingContext::CullMode::CULL_NONE);

    rendering_context->BindShaderProgram(sky_dome_program);
    rendering_context->Draw(sphere_mesh_.get());
    rendering_context->EndFrame();
}

} // namespace zero::render

