#include "render/Instantiator.hpp"
#include "render/Model.hpp"
#include "component/Material.hpp"
#include "component/Transform.hpp"
#include "component/Volume.hpp"
#include "math/Box.hpp"

namespace zero::render
{

Entity Instantiator::InstantiateModel(entt::registry& registry,
                                      const std::shared_ptr<Model>& model,
                                      Entity parent)
{
    Entity root_bone_entity = NullEntity;
    std::unordered_map<std::string, Entity> bone_entity_map{};
    return NullEntity;
}

Entity Instantiator::InstantiateNode(entt::registry& registry,
                                     const std::shared_ptr<Node>& node,
                                     Entity parent,
                                     Entity& root_bone_entity,
                                     std::unordered_map<std::string, Entity>& bone_entity_map)
{
    Entity entity = NullEntity;
    return entity;
}

Entity Instantiator::InstantiateEntityPrototype(entt::registry& registry,
                                                Entity parent,
                                                EntityPrototype* entity_prototype,
                                                const math::Matrix4x4& transformation)
{
    Entity entity = registry.create();
    return entity;
}

Entity Instantiator::InstantiatePrimitive(entt::registry& registry, const PrimitiveInstance& primitive)
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
            math::Box math_box{math::Vec3f::Zero(), math::Vec3f(static_cast<float>(box.width_),
																static_cast<float>(box.height_),
																static_cast<float>(box.depth_))};
            volume.bounding_volume_.center_ = math_box.Center();
            volume.bounding_volume_.radius_ = math_box.max_.Magnitude() * 0.5F;
            transform.scale_ = math_box.max_;
            break;
        }
        case PrimitiveInstance::Type::CONE:
        {
            Cone cone = primitive.GetCone();
            float half_height = static_cast<float>(cone.height_) * 0.5F;
            volume.bounding_volume_.radius_ = math::Sqrt(half_height * half_height + cone.radius_ * cone.radius_);
            break;
        }
        case PrimitiveInstance::Type::CYLINDER:
        {
            Cylinder cylinder = primitive.GetCylinder();
            float half_height = static_cast<float>(cylinder.height_) * 0.5F;
            float largest_radius = math::Max(cylinder.bottom_radius_, cylinder.top_radius_);
            volume.bounding_volume_.radius_ = math::Sqrt(half_height * half_height + largest_radius * largest_radius);
            break;
        }
        case PrimitiveInstance::Type::PLANE:
        {
            Plane plane = primitive.GetPlane();
            float half_width = static_cast<float>(plane.width_) * 0.5F;
            float half_height = static_cast<float>(plane.height_) * 0.5F;
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

    registry.emplace<Volume>(entity, volume);
    // Use default shaders
    registry.emplace<Material>(entity, Material{});
    registry.emplace<PrimitiveInstance>(entity, primitive);
    registry.emplace<Transform>(entity, transform);
    return entity;
}

Entity Instantiator::InstantiateLight(entt::registry& registry, const Light& light, Entity entity)
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
            registry.emplace<DirectionalLight>(entity_to_attach, light.GetDirectionalLight());
            break;
        }
        case Light::Type::POINT:
        {
            registry.emplace<PointLight>(entity_to_attach, light.GetPointLight());
            break;
        }
        case Light::Type::SPOT:
        {
            registry.emplace<SpotLight>(entity_to_attach, light.GetSpotLight());
            break;
        }
    }

    // Assign a new transform if one does not exist
    auto transform = registry.try_get<Transform>(entity_to_attach);
    if (transform == nullptr)
    {
        registry.emplace<Transform>(entity_to_attach, Transform{});
    }

    return entity_to_attach;
}

} // namespace zero::render