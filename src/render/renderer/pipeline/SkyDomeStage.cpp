#include "render/renderer/pipeline/SkyDomeStage.hpp"

namespace zero::render
{

SkyDomeStage::SkyDomeStage(IRenderingManager* rendering_manager)
: rendering_manager_(rendering_manager)
, sphere_mesh_(nullptr)
{
}

void SkyDomeStage::Initialize()
{
    IModelManager* model_manager = rendering_manager_->GetModelManager();
    PrimitiveInstance sphere_primitive_instance{Sphere{}};
    sphere_mesh_ = model_manager->GetPrimitive(sphere_primitive_instance);
}

void SkyDomeStage::Execute(IRenderView* render_view)
{
    const SkyDome& sky_dome = render_view->GetSkyDome();
    if (!sky_dome.is_active_)
    {
        // Skip render stage if the sky dome is not active
        return;
    }

    const Camera& camera = render_view->GetCamera();
    IRenderingContext* rendering_context = rendering_manager_->GetRenderingContext();
    IShaderManager* shader_manager = rendering_manager_->GetShaderManager();
    IUniformManager* uniform_manager = rendering_manager_->GetUniformManager();

    // Begin rendering frame with default render target
    rendering_context->BeginFrame(nullptr);
    rendering_context->SetViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    rendering_context->Clear();

    // Shader program start
    std::shared_ptr<IProgram> shader_program = shader_manager->GenerateProgram(sky_dome);
    shader_program->Use();
    rendering_context->BindShaderProgram(shader_program.get());

    uniform_manager->SetSkyDomeUniforms(shader_program.get(), camera, sky_dome);

    rendering_context->SetFillMode(IRenderingContext::FillMode::SOLID);
    rendering_context->SetCullMode(IRenderingContext::CullMode::CULL_NONE);

    // Shader program flush uniforms and draw
    shader_program->FlushUniforms();
    rendering_context->Draw(sphere_mesh_.get());

    // Shader program finish
    shader_program->Finish();
    rendering_context->EndFrame();
}

} // namespace zero::render

