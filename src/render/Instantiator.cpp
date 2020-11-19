#include "render/Instantiator.hpp"
#include "render/Model.hpp"
#include "math/Box.hpp"

namespace zero::render
{

Entity Instantiator::InstantiateModel(entt::registry& registry,
                                      const std::shared_ptr<Model>& model,
                                      Entity parent)
{
    Entity entity = registry.create();
    registry.assign<Volume>(entity, model->GetVolume());
    registry.assign<Material>(entity, model->GetMaterial());
    registry.assign<ModelInstance>(entity, model->GetModelInstance());

    zero::Transform transform = model->GetTransform();
    transform.parent_ = parent;
    for (const auto& child_gl_model : model->GetChildren())
    {
        Entity child_entity = InstantiateModel(registry, child_gl_model, entity);
        if (child_entity != NullEntity)
        {
            transform.children_.push_back(child_entity);
        }
    }

    registry.assign<zero::Transform>(entity, transform);
    return entity;
}

Entity Instantiator::InstantiatePrimitive(entt::registry& registry,
                                          const PrimitiveInstance& primitive)
{
    Entity entity = registry.create();
    Transform transform{};
    Volume volume{};
    // Set transform/volume values
    switch (primitive.GetType())
    {
        case PrimitiveInstance::Type::BOX:
        {
            Box box = primitive.GetBox();
            math::Box math_box{math::Vec3f::Zero(), math::Vec3f(box.width_, box.height_, box.depth_)};
            volume.bounding_volume_.center_ = math_box.Center();
            volume.bounding_volume_.radius_ = math_box.max_.Magnitude() * 0.5F;
            transform.Scale(math_box.max_);
            break;
        }
        case PrimitiveInstance::Type::CONE:
        {
            Cone cone = primitive.GetCone();
            float half_height = cone.height_ * 0.5F;
            volume.bounding_volume_.radius_ = math::Sqrt(half_height * half_height + cone.radius_ * cone.radius_);
            break;
        }
        case PrimitiveInstance::Type::CYLINDER:
        {
            Cylinder cylinder = primitive.GetCylinder();
            float half_height = cylinder.height_ * 0.5F;
            float largest_radius = math::Max(cylinder.bottom_radius_, cylinder.top_radius_);
            volume.bounding_volume_.radius_ = math::Sqrt(half_height * half_height + largest_radius * largest_radius);
            break;
        }
        case PrimitiveInstance::Type::PLANE:
        {
            Plane plane = primitive.GetPlane();
            float half_width = plane.width_ * 0.5F;
            float half_height = plane.height_ * 0.5F;
            volume.bounding_volume_.center_ = math::Vec3f(half_width,
                                                          0.0F,
                                                          half_height);
            volume.bounding_volume_.radius_ = half_width + half_height;
            break;
        }
        case PrimitiveInstance::Type::SPHERE:
        {
            // Volume slightly larger
            volume.bounding_volume_.radius_ = 1.05F;
            break;
        }
        case PrimitiveInstance::Type::TORUS:
        {
            Torus torus = primitive.GetTorus();
            // Volume slightly larger
            volume.bounding_volume_.radius_ = (torus.radius_ + torus.tube_radius_) + 0.05F;
            break;
        }
    }

    registry.assign<Volume>(entity, volume);
    // Use default shaders
    registry.assign<Material>(entity, Material{});
    registry.assign<PrimitiveInstance>(entity, primitive);
    registry.assign<Transform>(entity, transform);
    return entity;
}

Entity Instantiator::InstantiateLight(entt::registry& registry,
                                      const Light& light,
                                      Entity entity)
{
    Entity entity_to_attach = entity;
    if (!registry.valid(entity_to_attach))
    {
        entity_to_attach = registry.create();
    }

    switch (light.GetType())
    {
        case Light::Type::DIRECTIONAL:
        {
            registry.assign<DirectionalLight>(entity_to_attach, light.GetDirectionalLight());
            break;
        }
        case Light::Type::POINT:
        {
            registry.assign<PointLight>(entity_to_attach, light.GetPointLight());
            break;
        }
        case Light::Type::SPOT:
        {
            registry.assign<SpotLight>(entity_to_attach, light.GetSpotLight());
            break;
        }
    }

    // Assign a new transform if one does not exist
    if (!registry.has<Transform>(entity_to_attach))
    {
        registry.assign<Transform>(entity_to_attach, Transform{});
    }

    return entity_to_attach;
}

} // namespace zero::render