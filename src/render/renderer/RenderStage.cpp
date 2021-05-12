#include "render/renderer/IRenderStage.hpp"

namespace zero::render
{

RenderStage::RenderStage()
: volume_material_()
, volume_primitive_instance_(Sphere())
{
    volume_material_.diffuse_color_ = math::Vec3f(1.0F, 0.0F, 0.0F);
    volume_material_.specular_exponent_ = 32.0F;
}

void RenderStage::Render(IRenderingContext* rendering_context,
                         IModelManager* model_manager,
                         const std::shared_ptr<IRenderable>& renderable,
                         const TimeDelta& time_delta)
{

    const ModelInstance* model_instance = renderable->GetModelInstance();
    const PrimitiveInstance* primitive_instance = renderable->GetPrimitiveInstance();

    // Render model/primitive
    if (model_instance)
    {
        std::shared_ptr<Model> model = model_manager->GetModel(model_instance->model_name_);
        std::shared_ptr<Node> node = model->GetNode(model_instance->node_name_);
        IMesh* mesh = node->GetEntityPrototype(model_instance->mesh_index_)->GetMesh();
        rendering_context->Draw(mesh);
    }
    else
    {
        std::shared_ptr<IMesh> primitive_mesh = model_manager->GetPrimitive(*primitive_instance);
        rendering_context->Draw(primitive_mesh.get());
    }
}

void RenderStage::RenderVolume(IRenderingManager* rendering_manager,
                               const Camera& camera,
                               const math::Matrix4x4& projection_matrix,
                               const math::Matrix4x4& view_matrix,
                               const Volume& volume)
{
    IModelManager* model_manager = rendering_manager->GetModelManager();
    IRenderingContext* rendering_context = rendering_manager->GetRenderingContext();
    IShaderManager* shader_manager = rendering_manager->GetShaderManager();
    IUniformManager* uniform_manager = rendering_manager->GetUniformManager();

    rendering_context->SetCullMode(IRenderingContext::CullMode::CULL_NONE);
    rendering_context->SetFillMode(IRenderingContext::FillMode::WIREFRAME);

    math::Matrix4x4 model_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(volume.bounding_volume_.radius_))
            .Translate(volume.bounding_volume_.center_);

    uniform_manager->UpdateCameraUniforms(projection_matrix, view_matrix, camera.position_);
    uniform_manager->UpdateModelUniforms(model_matrix, (view_matrix * model_matrix).Inverse());
    uniform_manager->UpdateMaterialUniforms(volume_material_);

    std::shared_ptr<IProgram> shader_program = shader_manager->GenerateProgram(volume_material_);
    rendering_context->BindShaderProgram(shader_program.get());
    rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::CAMERA_BUFFER));
    rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MATERIAL_BUFFER));
    rendering_context->BindUniformBuffer(uniform_manager->GetUniformBuffer(IUniformManager::UniformBufferType::MODEL_BUFFER));

    std::shared_ptr<IMesh> sphere_mesh = model_manager->GetPrimitive(volume_primitive_instance_);
    rendering_context->Draw(sphere_mesh.get());
}

} // namespace zero::render

