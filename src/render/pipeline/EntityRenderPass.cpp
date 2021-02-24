#include "render/pipeline/EntityRenderPass.hpp"
#include "render/CullingManager.hpp"

namespace zero::render
{

EntityRenderPass::EntityRenderPass(IRenderingManager* rendering_manager)
: rendering_manager_(rendering_manager)
{
}

void EntityRenderPass::Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta)
{
    // Retrieve rendering sub managers
    IModelManager* model_manager = rendering_manager_->GetModelManager();
    IRenderingContext* rendering_context = rendering_manager_->GetRenderingContext();
    IShaderManager* shader_manager = rendering_manager_->GetShaderManager();
    ITextureManager* texture_manager = rendering_manager_->GetTextureManager();
    IUniformManager* uniform_manager = rendering_manager_->GetUniformManager();

    // Retrieve samplers and shadow map textures
    ISampler* diffuse_map_sampler = texture_manager->GetDiffuseMapSampler();
    ISampler* shadow_map_sampler = texture_manager->GetShadowMapSampler();
    std::vector<std::shared_ptr<ITexture>> shadow_map_textures = texture_manager->GetShadowMapTextures();

    // Begin rendering frame with default render target
    rendering_context->BeginFrame(nullptr);
    rendering_context->SetViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    math::Matrix4x4 view_matrix = camera.GetViewMatrix();

    // Retrieve all component views
    auto renderable_view     = registry.view<const Transform, const Material, const Volume>();
    auto model_view          = registry.view<const Transform, const Material, const Volume, const ModelInstance>();
    auto animated_model_view = registry.view<const Transform, const Material, const Volume, const ModelInstance, const Animator>();
    auto primitive_view      = registry.view<const Transform, const Material, const Volume, const PrimitiveInstance>();

    /* Cull entities
     * These entities contain the following components:
     *      - Transform
     *      - Material
     *      - Volume
     *      - ModelInstance or PrimitiveInstance
     * Some ModelInstance entities may also have a Animator component
     */
    std::vector<Entity> viewable_entities = CullingManager::GetRenderableEntities(camera, registry);

    // Begin rendering each renderable entity
    for (Entity viewable_entity : viewable_entities)
    {
        const Transform& transform = renderable_view.get<const Transform>(viewable_entity);
        const Material& material = renderable_view.get<const Material>(viewable_entity);
        const Volume& volume = renderable_view.get<const Volume>(viewable_entity);

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
        rendering_context->BindShaderProgram(shader_program);
        rendering_context->BindUniformBuffer(0, uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::CAMERA_BUFFER));
        rendering_context->BindUniformBuffer(1, uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::LIGHT_BUFFER));
        rendering_context->BindUniformBuffer(2, uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MATERIAL_BUFFER));
        rendering_context->BindUniformBuffer(3, uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MODEL_BUFFER));
        rendering_context->BindUniformBuffer(4, uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::SHADOW_MAP_BUFFER));

        // Bind shadow map textures/samplers
        uint32 texture_index = 0;
        for (; texture_index < shadow_map_textures.size(); ++texture_index)
        {
            rendering_context->BindTexture(texture_index, shadow_map_textures[texture_index].get());
            rendering_context->BindTextureSampler(texture_index, shadow_map_sampler);
            uniform_manager->SetShadowSamplerName(shader_program, texture_index);
        }

        // Bind diffuse map texture/sampler
        rendering_context->BindTexture(texture_index, texture_manager->GetTexture(material.texture_map_.diffuse_map_));
        rendering_context->BindTextureSampler(texture_index, diffuse_map_sampler);
        uniform_manager->SetDiffuseSamplerName(shader_program, texture_index);

        // Render model/primitive
        if (model_view.contains(viewable_entity))
        {
            const ModelInstance& model_instance = model_view.get<const ModelInstance>(viewable_entity);
            std::shared_ptr<Model> model = model_manager->GetModel(model_instance);
            if (animated_model_view.contains(viewable_entity))
            {
                const Animator& animator = animated_model_view.get<const Animator>(viewable_entity);
                RenderAnimatedModel(rendering_manager_, model, shader_program, animator, time_delta);
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
    };

    // Finish frame
    rendering_context->EndFrame();
}

} // namespace zero::render

