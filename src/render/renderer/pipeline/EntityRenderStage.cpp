#include "render/renderer/pipeline/EntityRenderStage.hpp"

namespace zero::render
{

EntityRenderStage::EntityRenderStage(IRenderingManager* rendering_manager)
: rendering_manager_(rendering_manager)
, diffuse_map_sampler_(rendering_manager_->GetTextureManager()->GetDiffuseMapSampler())
, shadow_map_sampler_(rendering_manager_->GetTextureManager()->GetShadowMapSampler())
{
}

void EntityRenderStage::Execute(IRenderView* render_view)
{
    // Retrieve rendering sub managers
    IModelManager* model_manager = rendering_manager_->GetModelManager();
    IRenderingContext* rendering_context = rendering_manager_->GetRenderingContext();
    IShaderManager* shader_manager = rendering_manager_->GetShaderManager();
    ITextureManager* texture_manager = rendering_manager_->GetTextureManager();
    IUniformManager* uniform_manager = rendering_manager_->GetUniformManager();

    std::vector<std::shared_ptr<ITexture>> shadow_map_textures = texture_manager->GetShadowMapTextures();

    const Camera& camera = render_view->GetCamera();
    const TimeDelta& time_delta = render_view->GetTimeDelta();

    // Begin rendering frame with default render target
    rendering_context->BeginFrame(nullptr);
    rendering_context->SetViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    math::Matrix4x4 view_matrix = camera.GetViewMatrix();

    // Begin rendering each renderable entity
    for (const std::shared_ptr<IRenderable>& renderable : render_view->GetRenderables())
    {
        const Transform& transform = renderable->GetTransform();
        const Material& material = renderable->GetMaterial();
        const Volume& volume = renderable->GetVolume();

        auto shader_program = shader_manager->GenerateProgram(material);
        if (shader_program == nullptr)
        {
            continue;
        }

        // Set fill/cull settings
        rendering_context->SetCullMode(material.two_sided_ ? IRenderingContext::CullMode::CULL_NONE : IRenderingContext::CullMode::CULL_BACK);
        rendering_context->SetFillMode(material.wireframe_enabled_ ? IRenderingContext::FillMode::WIREFRAME : IRenderingContext::FillMode::SOLID);

        // Update model and material uniform buffers
        math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
        uniform_manager->UpdateModelUniforms(model_matrix, (view_matrix * model_matrix).Inverse());
        uniform_manager->UpdateMaterialUniforms(material);

        // Bind uniform buffers
        rendering_context->BindShaderProgram(shader_program.get());
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::CAMERA_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MATERIAL_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MODEL_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::LIGHT_INFO_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::DIRECTIONAL_LIGHT_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::POINT_LIGHT_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::SPOT_LIGHT_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::SHADOW_MAP_BUFFER));

        // Bind shadow map textures/samplers
        uint32 texture_index = 0;
        for (; texture_index < shadow_map_textures.size(); ++texture_index)
        {
            rendering_context->BindTexture(texture_index, shadow_map_textures[texture_index].get());
            rendering_context->BindTextureSampler(texture_index, shadow_map_sampler_.get());
            uniform_manager->SetShadowSamplerName(shader_program.get(), texture_index);
        }

        // Bind diffuse map texture/sampler
        rendering_context->BindTexture(texture_index, texture_manager->GetTexture(material.texture_map_.diffuse_map_));
        rendering_context->BindTextureSampler(texture_index, diffuse_map_sampler_.get());
        uniform_manager->SetDiffuseSamplerName(shader_program.get(), texture_index);

        Render(rendering_context, model_manager, shader_program.get(), renderable, time_delta);
    }

    // Finish frame
    rendering_context->EndFrame();
}

} // namespace zero::render

