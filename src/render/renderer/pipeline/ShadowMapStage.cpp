#include "render/renderer/pipeline/ShadowMapStage.hpp"

namespace zero::render
{

const std::string ShadowMapStage::kShadowMapFragmentShaderName = "shadow_map.fragment.glsl";

ShadowMapStage::ShadowMapStage(IRenderingManager* rendering_manager)
: rendering_manager_(rendering_manager)
, cascaded_shadow_map_(Constants::kShadowCascadeCount)
, diffuse_map_sampler_(rendering_manager_->GetTextureManager()->GetDiffuseMapSampler())
{
}

void ShadowMapStage::Execute(IRenderView* render_view)
{
    // Only perform shadow maps on one active directional light
    DirectionalLight directional_light{};
    if (!GetShadowCastingDirectionalLight(registry, directional_light))
    {
        return;
    }

    IRenderingContext* rendering_context = rendering_manager_->GetRenderingContext();
    ITextureManager* texture_manager = rendering_manager_->GetTextureManager();
    IUniformManager* uniform_manager = rendering_manager_->GetUniformManager();

    // Retrieve shadow map render targets
    std::vector<std::shared_ptr<IRenderTarget>> shadow_map_render_targets = texture_manager->GetShadowMapRenderTargets();

    // Update the cascaded shadow map data
    cascaded_shadow_map_.Update(camera, directional_light);
    std::vector<math::Matrix4x4> light_view_matrices = cascaded_shadow_map_.GetLightViewMatrices();
    std::vector<math::Matrix4x4> light_projection_matrices = cascaded_shadow_map_.GetProjectionMatrices();
    std::vector<math::Box> world_bounding_boxes = cascaded_shadow_map_.GetWorldBoundingBoxes();

    // Render depth maps from the point of view of the directional light
    for (uint32 cascade_index = 0; cascade_index < Constants::kShadowCascadeCount; ++cascade_index)
    {
        // Get renderable entities that are viewable at the given cascade
        std::vector<Entity> viewable_entities = CullingManager::GetShadowCastingEntities(world_bounding_boxes[cascade_index], registry);

        // Begin rendering to shadow map render target
        rendering_context->BeginFrame(shadow_map_render_targets[cascade_index].get());
        rendering_context->SetFillMode(IRenderingContext::FillMode::SOLID);

        uniform_manager->UpdateCameraUniforms(light_projection_matrices[cascade_index], light_view_matrices[cascade_index], math::Vec3f::Zero());
        RenderEntities(light_view_matrices[cascade_index], viewable_entities, registry, time_delta);

        // End rendering to shadow map render target
        rendering_context->EndFrame();
    }
}

bool ShadowMapStage::GetShadowCastingDirectionalLight(IRenderView* render_view, DirectionalLight& out_directional_light)
{
    for (const DirectionalLight& directional_light : render_view->GetDirectionalLights())
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
                                    const std::vector<Entity>& viewable_entities,
                                    const entt::registry& registry,
                                    const TimeDelta& time_delta)
{
    // Retrieve rendering sub managers
    IModelManager* model_manager = rendering_manager_->GetModelManager();
    IRenderingContext* rendering_context = rendering_manager_->GetRenderingContext();
    IShaderManager* shader_manager = rendering_manager_->GetShaderManager();
    ITextureManager* texture_manager = rendering_manager_->GetTextureManager();
    IUniformManager* uniform_manager = rendering_manager_->GetUniformManager();

    // Retrieve samplers and shadow map textures
    auto renderable_view     = registry.view<const Transform, const Material, const Volume>();
    auto model_view          = registry.view<const Transform, const Material, const Volume, const ModelInstance>();
    auto animated_model_view = registry.view<const Transform, const Material, const Volume, const ModelInstance, const Animator>();
    auto primitive_view      = registry.view<const Transform, const Material, const Volume, const PrimitiveInstance>();

    // Begin rendering each renderable entity
    for (Entity viewable_entity : viewable_entities)
    {
        const Transform& transform = renderable_view.get<const Transform>(viewable_entity);
        const Material& material = renderable_view.get<const Material>(viewable_entity);

        auto shader_program = shader_manager->GenerateProgram(material.shaders_.vertex_shader_, kShadowMapFragmentShaderName);

        // Update model and material uniform buffers
        math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
        uniform_manager->UpdateModelUniforms(model_matrix, (light_view_matrix * model_matrix).Inverse());
        uniform_manager->UpdateMaterialUniforms(material);

        // Bind uniform buffers
        rendering_context->BindShaderProgram(shader_program.get());
        rendering_context->BindUniformBuffer(0, uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::CAMERA_BUFFER));
        rendering_context->BindUniformBuffer(1, uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MODEL_BUFFER));

        // Bind diffuse map texture/sampler
        rendering_context->BindTexture(0, texture_manager->GetTexture(material.texture_map_.diffuse_map_));
        rendering_context->BindTextureSampler(0, diffuse_map_sampler_.get());
        uniform_manager->SetDiffuseSamplerName(shader_program.get(), 0);

        // Render model/primitive
        if (model_view.contains(viewable_entity))
        {
            const ModelInstance& model_instance = model_view.get<const ModelInstance>(viewable_entity);
            std::shared_ptr<Model> model = model_manager->GetModel(model_instance);
            if (animated_model_view.contains(viewable_entity))
            {
                const Animator& animator = animated_model_view.get<const Animator>(viewable_entity);
                RenderAnimatedModel(rendering_manager_, model, shader_program.get(), animator, time_delta);
            }
            else
            {
                RenderModel(rendering_manager_, model);
            }
        }
        else
        {
            const PrimitiveInstance& primitive_instance = primitive_view.get<const PrimitiveInstance>(viewable_entity);
            std::shared_ptr<IMesh> primitive_mesh = model_manager->GetPrimitive(primitive_instance);
            rendering_context->Draw(primitive_mesh.get());
        }
    }
}

} // namespace zero::render

