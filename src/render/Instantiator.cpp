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
    Entity root_bone_entity = NullEntity;
    std::unordered_map<std::string, Entity> bone_entity_map{};
    Entity root_entity = InstantiateNode(registry, model->GetRootNode(), parent, root_bone_entity, bone_entity_map);

    // Update animation specific data
    if (model->GetAnimator())
    {
        // Create Animator component for root entity
        Animator& animator = registry.emplace<Animator>(root_entity, *(model->GetAnimator()));
        animator.SetRootBoneEntity(root_bone_entity);

        // Update bone entity list and set root bone entity for all Animated components
        auto animated_view = registry.view<Animated>();
        auto transform_view = registry.view<const Transform>();
        std::queue<Entity> frontier{};
        frontier.push(root_entity);
        while (!frontier.empty())
        {
            Entity entity = frontier.front();
            frontier.pop();

            if (animated_view.contains(entity))
            {
                Animated& animated_component = animated_view.get<Animated>(entity);
                animated_component.root_bone_entity_ = root_bone_entity;
                animated_component.inverse_root_transform_ = model->GetRootNode()->GetTransformation().Inverse();
                animated_component.bone_entities_.reserve(animated_component.bone_names_.size());
                // Insert bone Entity instance in the same order as the associated bone name
                for (const std::string& bone_name : animated_component.bone_names_)
                {
                    animated_component.bone_entities_.push_back(bone_entity_map[bone_name]);
                }
            }

            const Transform& transform = transform_view.get<const Transform>(entity);
            for (Entity child_entity : transform.children_)
            {
                frontier.push(child_entity);
            }
        }
    }
    return root_entity;
}

Entity Instantiator::InstantiateNode(entt::registry& registry,
                                     std::shared_ptr<Node> node,
                                     Entity parent,
                                     Entity& root_bone_entity,
                                     std::unordered_map<std::string, Entity>& bone_entity_map)
{
    Entity entity = NullEntity;
    if (node->GetEntityPrototypeCount() == 1)
    {
        // Directly instantiate EntityPrototype
        entity = InstantiateEntityPrototype(registry, parent, node->GetEntityPrototype(0), node->GetTransformation());
    }
    else
    {
        // Aggregate multiple EntityPrototypes in one new parent entity
        entity = registry.create();
        registry.emplace<Volume>(entity, node->GetVolume());
        Transform& entity_transform = registry.emplace<Transform>(entity, Transform::FromMatrix4x4(node->GetTransformation()));
        entity_transform.parent_ = parent;

        // Add EntityPrototype instances as children to entity
        std::vector<Entity> child_entities{};
        child_entities.reserve(node->GetEntityPrototypeCount());
        for (uint32 prototype_index = 0; prototype_index < node->GetEntityPrototypeCount(); ++prototype_index)
        {
            EntityPrototype* entity_prototype = node->GetEntityPrototype(prototype_index);
            child_entities.push_back(InstantiateEntityPrototype(registry, entity, entity_prototype, node->GetTransformation()));
        }

        entity_transform = registry.get<Transform>(entity);
        entity_transform.children_ = child_entities;
    }

    if (node->GetBone())
    {
        registry.emplace<Bone>(entity, *node->GetBone());
        if (root_bone_entity == NullEntity)
        {
            root_bone_entity = entity;
        }
        bone_entity_map[node->GetBone()->name_] = entity;
    }

    // Update parent transform
    if (parent != NullEntity)
    {
        Transform& parent_transform = registry.get<Transform>(parent);
        parent_transform.children_.push_back(entity);
    }

    // Add child node instances as children
    InstantiateChildNodes(registry, node, entity, root_bone_entity, bone_entity_map);
    return entity;
}

void Instantiator::InstantiateChildNodes(entt::registry& registry,
                                         std::shared_ptr<Node> parent_node,
                                         Entity parent_entity,
                                         Entity& root_bone_entity,
                                         std::unordered_map<std::string, Entity>& bone_entity_map)
{
    std::vector<Entity> child_entities{};
    child_entities.reserve(parent_node->GetChildren().size());
    for (std::shared_ptr<Node> child_node : parent_node->GetChildren())
    {
        child_entities.push_back(InstantiateNode(registry, child_node, parent_entity, root_bone_entity, bone_entity_map));
    }

    Transform& transform = registry.get<Transform>(parent_entity);
    transform.children_.insert(transform.children_.end(), child_entities.begin(), child_entities.end());
}

Entity Instantiator::InstantiateEntityPrototype(entt::registry& registry,
                                                Entity parent,
                                                EntityPrototype* entity_prototype,
                                                const math::Matrix4x4& transformation)
{
    Entity entity = registry.create();
    Transform& transform = registry.emplace<Transform>(entity, Transform::FromMatrix4x4(transformation));
    transform.parent_ = parent;
    if (entity_prototype->GetMesh()->GetBoneNames().size() > 0)
    {
        Animated& animated_component = registry.emplace<Animated>(entity);
        animated_component.bone_names_ = entity_prototype->GetMesh()->GetBoneNames();
    }
    registry.emplace<Volume>(entity, entity_prototype->GetVolume());
    registry.emplace<Material>(entity, entity_prototype->GetMaterial());
    registry.emplace<ModelInstance>(entity, entity_prototype->GetModelInstance());
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