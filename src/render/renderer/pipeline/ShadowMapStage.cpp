#include "render/renderer/pipeline/ShadowMapStage.hpp"

namespace zero::render
{

const std::string ShadowMapStage::kShadowMapFragmentShaderName = "shadow_map.fragment.glsl";

ShadowMapStage::ShadowMapStage(IRenderingManager* rendering_manager)
: rendering_manager_(rendering_manager)
, diffuse_map_sampler_(rendering_manager_->GetTextureManager()->GetDiffuseMapSampler())
{
}

void ShadowMapStage::Execute(IRenderView* render_view)
{
    // Only perform shadow maps on one active directional light
    DirectionalLight directional_light{};
    if (!GetShadowCastingDirectionalLight(render_view->GetDirectionalLights(), directional_light))
    {
        return;
    }

    IRenderingContext* rendering_context = rendering_manager_->GetRenderingContext();
    ITextureManager* texture_manager = rendering_manager_->GetTextureManager();
    IUniformManager* uniform_manager = rendering_manager_->GetUniformManager();

    // Retrieve shadow map render targets
    std::vector<std::shared_ptr<IRenderTarget>> shadow_map_render_targets = texture_manager->GetShadowMapRenderTargets();

    const TimeDelta& time_delta = render_view->GetTimeDelta();
    const CascadedShadowMap& cascaded_shadow_map = render_view->GetCascadedShadowMap();

    // Retrieve cascaded shadow map data
    std::vector<math::Matrix4x4> light_view_matrices = cascaded_shadow_map.GetLightViewMatrices();
    std::vector<math::Matrix4x4> light_projection_matrices = cascaded_shadow_map.GetProjectionMatrices();

    // Render depth maps from the point of view of the directional light
    for (uint32 cascade_index = 0; cascade_index < cascaded_shadow_map.GetCascadeCount(); ++cascade_index)
    {
        // Get renderable entities that are viewable at the given cascade
        const std::vector<std::shared_ptr<IRenderable>>& renderables = render_view->GetShadowCastingRenderables(cascade_index);

        // Begin rendering to shadow map render target
        rendering_context->BeginFrame(shadow_map_render_targets[cascade_index].get());
        rendering_context->SetFillMode(IRenderingContext::FillMode::SOLID);

        uniform_manager->UpdateCameraUniforms(light_projection_matrices[cascade_index], light_view_matrices[cascade_index], math::Vec3f::Zero());
        RenderEntities(light_view_matrices[cascade_index], renderables, time_delta);

        // End rendering to shadow map render target
        rendering_context->EndFrame();
    }
}

bool ShadowMapStage::GetShadowCastingDirectionalLight(const std::vector<DirectionalLight>& directional_lights,
                                                      DirectionalLight& out_directional_light)
{
    for (const DirectionalLight& directional_light : directional_lights)
    {
        // Retrieve the first shadow casting directional light
        if (directional_light.casts_shadows_)
        {
            out_directional_light = directional_light;
            // Active directional light found
            return true;
        }
    }
    // No active directional light found
    return false;
}

void ShadowMapStage::RenderEntities(const math::Matrix4x4& light_view_matrix,
                                    const std::vector<std::shared_ptr<IRenderable>>& renderables,
                                    const TimeDelta& time_delta)
{
    // Retrieve rendering sub managers
    IModelManager* model_manager = rendering_manager_->GetModelManager();
    IRenderingContext* rendering_context = rendering_manager_->GetRenderingContext();
    IShaderManager* shader_manager = rendering_manager_->GetShaderManager();
    ITextureManager* texture_manager = rendering_manager_->GetTextureManager();
    IUniformManager* uniform_manager = rendering_manager_->GetUniformManager();

    // Begin rendering each renderable entity
    for (const std::shared_ptr<IRenderable>& renderable : renderables)
    {
        const Transform& transform = renderable->GetTransform();
        const Material& material = renderable->GetMaterial();

        auto shader_program = shader_manager->GenerateProgram(material.shaders_.vertex_shader_, kShadowMapFragmentShaderName);

        // Update model and material uniform buffers
        math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
        uniform_manager->UpdateModelUniforms(model_matrix, (light_view_matrix * model_matrix).Inverse());
        uniform_manager->UpdateMaterialUniforms(material);

        // Bind uniform buffers
        rendering_context->BindShaderProgram(shader_program.get());
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::CAMERA_BUFFER));
        rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MODEL_BUFFER));

        // Bind diffuse map texture/sampler
        rendering_context->BindTexture(0, texture_manager->GetTexture(material.texture_map_.diffuse_map_));
        rendering_context->BindTextureSampler(0, diffuse_map_sampler_.get());
        uniform_manager->SetDiffuseSamplerName(shader_program.get(), 0);

        Render(rendering_context, model_manager, shader_program.get(), renderable, time_delta);
    }
}

} // namespace zero::render

