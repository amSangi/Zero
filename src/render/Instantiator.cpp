#include "render/Instantiator.hpp"
#include "render/Model.hpp"
#include "component/Transform.hpp"
#include "math/Box.hpp"
#include <queue>

namespace zero::render
{

Entity Instantiator::InstantiateModel(entt::registry& registry,
                                      const std::shared_ptr<Model>& model,
                                      Entity parent)
{
    return InstantiateNode(registry, model->GetRootNode(), parent);
}

Entity Instantiator::InstantiateNode(entt::registry& registry, std::shared_ptr<Node> node, Entity parent)
{
    auto InstantiateChildNodes = [](Entity entity, std::shared_ptr<Node> node, entt::registry& registry)
    {
        // Instantiate child nodes
        std::vector<Entity> child_entities{};
        child_entities.reserve(node->GetChildren().size());
        for (const auto child_node : node->GetChildren())
        {
            child_entities.push_back(InstantiateNode(registry, child_node, entity));
        }

        // Set child entities
        Transform& transform = registry.view<Transform>().get<Transform>(entity);
        transform.children_ = child_entities;
    };

    if (node->GetEntityPrototypeCount() == 1)
    {
        // Directly instantiate node
        Entity entity = InstantiateEntityPrototype(registry, parent, node->GetEntityPrototype(0), node->GetTransformation());
        InstantiateChildNodes(entity, node, registry);
        return entity;
    }

    // Create entity with transform/volume components to contain multiple entity prototypes
    Entity entity = registry.create();
    registry.emplace<Transform>(entity, Transform::FromMatrix4x4(node->GetTransformation()));
    registry.emplace<Volume>(entity, node->GetVolume());
    InstantiateChildNodes(entity, node, registry);
    return entity;
}

Entity Instantiator::InstantiateEntityPrototype(entt::registry& registry,
                                                Entity parent,
                                                EntityPrototype* entity_prototype,
                                                const math::Matrix4x4& transformation)
{
    Entity entity = registry.create();
    Transform& transform = registry.emplace<Transform>(entity, Transform::FromMatrix4x4(transformation));
    transform.parent_ = parent;
    registry.emplace<Volume>(entity, entity_prototype->GetVolume());
    registry.emplace<Material>(entity, entity_prototype->GetMaterial());
    registry.emplace<ModelInstance>(entity, entity_prototype->GetModelInstance());
    if (entity_prototype->GetAnimator())
    {
        registry.emplace<Animator>(entity, *entity_prototype->GetAnimator());
    }
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
    if (!registry.has<Transform>(entity_to_attach))
    {
        registry.emplace<Transform>(entity_to_attach, Transform{});
    }

    return entity_to_attach;
}

} // namespace zero::render