#include "render/renderer/pipeline/EntityRenderStage.hpp"
#include "render/Constants.hpp"

namespace zero::render
{

EntityRenderStage::EntityRenderStage(IRenderingManager* rendering_manager)
: rendering_manager_(rendering_manager)
, diffuse_map_sampler_(nullptr)
, shadow_map_sampler_(nullptr)
{
}

void EntityRenderStage::Initialize()
{
    ITextureManager* texture_manager = rendering_manager_->GetTextureManager();
    diffuse_map_sampler_ = texture_manager->GetDiffuseMapSampler();
    shadow_map_sampler_ = texture_manager->GetShadowMapSampler();
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

    math::Matrix4x4 projection_matrix = camera.GetProjectionMatrix();
    math::Matrix4x4 view_matrix = camera.GetViewMatrix();
    uniform_manager->UpdateCameraUniforms(projection_matrix, view_matrix, camera.position_);

    // Begin rendering each renderable entity
    for (const std::shared_ptr<IRenderable>& renderable : render_view->GetRenderables())
    {
        const Transform& transform = renderable->GetTransform();
        const Material& material = renderable->GetMaterial();

        // Shader program start
        auto shader_program = shader_manager->GenerateProgram(material);
        if (shader_program == nullptr)
        {
            continue;
        }
        shader_program->Use();
        rendering_context->BindShaderProgram(shader_program.get());

        // Set fill/cull settings
        rendering_context->SetCullMode(material.two_sided_ ? IRenderingContext::CullMode::CULL_NONE : IRenderingContext::CullMode::CULL_BACK);
        rendering_context->SetFillMode(material.wireframe_enabled_ ? IRenderingContext::FillMode::WIREFRAME : IRenderingContext::FillMode::SOLID);

        // Update model and material uniform buffers
        math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
        uniform_manager->UpdateModelUniforms(model_matrix, (view_matrix * model_matrix).Inverse());
        uniform_manager->UpdateMaterialUniforms(material);

        // Bind uniform buffers
        if (renderable->GetAnimator())
        {
            // Only update and bind the bone buffer for animated entities
            // Assume the material component has an animated vertex shader (i.e. has the bone uniform)
            uniform_manager->UpdateBoneUniforms(renderable->GetBoneMatrices());
            rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::BONE_BUFFER));
        }
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::CAMERA_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MATERIAL_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MODEL_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::LIGHT_INFO_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::DIRECTIONAL_LIGHT_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::POINT_LIGHT_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::SPOT_LIGHT_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::SHADOW_MAP_BUFFER));

        // Bind shadow map textures/samplers
        for (uint32 texture_index = 0; texture_index < shadow_map_textures.size(); ++texture_index)
        {
            uniform_manager->SetShadowSamplerName(shader_program.get(), texture_index);
            rendering_context->BindTexture(texture_index, shadow_map_textures[texture_index].get());
            rendering_context->BindTextureSampler(texture_index, shadow_map_sampler_.get());
        }

        // Bind diffuse map texture/sampler
        uint32 diffuse_texture_index = Constants::kShadowCascadeCount;
        uniform_manager->SetDiffuseSamplerName(shader_program.get(), diffuse_texture_index);
        rendering_context->BindTexture(diffuse_texture_index, texture_manager->GetTexture(material.texture_map_.diffuse_map_));
        rendering_context->BindTextureSampler(diffuse_texture_index, diffuse_map_sampler_.get());

        // Shader program flush uniforms and draw
        shader_program->FlushUniforms();
        Render(rendering_context, model_manager, renderable, time_delta);

        // Shader program finish
        shader_program->Finish();
    }

    // Finish frame
    rendering_context->EndFrame();
}

} // namespace zero::render

