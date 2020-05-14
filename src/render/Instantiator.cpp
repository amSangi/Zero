#include "render/Instantiator.hpp"
#include "render/IModel.hpp"
#include "math/Box.hpp"

using namespace zero::render;

zero::Component::Entity Instantiator::InstantiateModel(entt::registry& registry,
                                                       const std::shared_ptr<IModel>& model,
                                                       zero::Component::Entity parent) {
    auto entity = registry.create();
    registry.assign<Volume>(entity, model->GetVolume());
    registry.assign<Material>(entity, model->GetMaterial());
    registry.assign<ModelInstance>(entity, model->GetModelInstance());

    zero::Transform transform = model->GetTransform();
    transform.parent_ = parent;
    for (const auto& child_gl_model : model->GetChildren()) {
        auto child_entity = InstantiateModel(registry, child_gl_model, entity);
        if (child_entity != zero::Component::NullEntity) {
            transform.children_.push_back(child_entity);
        }
    }

    registry.assign<zero::Transform>(entity, transform);
    return entity;
}

zero::Component::Entity Instantiator::InstantiatePrimitive(entt::registry& registry,
                                                           const PrimitiveInstance& primitive) {
    auto entity = registry.create();
    Transform transform{};
    Volume volume{};
    // Set transform/volume values
    switch (primitive.GetType()) {
        case PrimitiveInstance::Type::BOX:
        {
            auto box = primitive.GetBox();
            math::Box math_box{math::Vec3f::Zero(), math::Vec3f(box.width_, box.height_, box.depth_)};
            volume.bounding_volume_.center_ = math_box.Center();
            volume.bounding_volume_.radius_ = math_box.max_.Magnitude() * 0.5F;
            transform.Scale(math_box.max_);
            break;
        }
        case PrimitiveInstance::Type::CONE:
        {
            auto cone = primitive.GetCone();
            auto half_height = cone.height_ * 0.5F;
            volume.bounding_volume_.radius_ = math::Sqrt(half_height * half_height + cone.radius_ * cone.radius_);
            break;
        }
        case PrimitiveInstance::Type::CYLINDER:
        {
            auto cylinder = primitive.GetCylinder();
            auto half_height = cylinder.height_ * 0.5F;
            auto largest_radius = math::Max(cylinder.bottom_radius_, cylinder.top_radius_);
            volume.bounding_volume_.radius_ = math::Sqrt(half_height * half_height + largest_radius * largest_radius);
            break;
        }
        case PrimitiveInstance::Type::PLANE:
        {
            auto plane = primitive.GetPlane();
            auto half_width = plane.width_ * 0.5F;
            auto half_height = plane.height_ * 0.5F;
            volume.bounding_volume_.center_ = math::Vec3f(half_width,
                                                          0.0F,
                                                          half_height);
            volume.bounding_volume_.radius_ = half_width + half_height;
            break;
        }
        case PrimitiveInstance::Type::SPHERE:
        {
            auto sphere = primitive.GetSphere();
            // Volume slightly larger
            volume.bounding_volume_.radius_ = 1.05F;
            break;
        }
        case PrimitiveInstance::Type::TORUS:
        {
            auto torus = primitive.GetTorus();
            // Volume slightly larger
            volume.bounding_volume_.radius_ = (torus.radius_ + torus.tube_radius_) + 0.05F;
            break;
        }
    }

    registry.assign<render::Volume>(entity, volume);
    // Use default shaders
    registry.assign<render::Material>(entity, Material{});
    registry.assign<render::PrimitiveInstance>(entity, primitive);
    registry.assign<Transform>(entity, transform);
    return entity;
}

zero::Component::Entity Instantiator::InstantiateLight(entt::registry& registry,
                                                       const Light& light,
                                                       Component::Entity entity) {
    Component::Entity entity_to_attach = entity;
    if (!registry.valid(entity_to_attach)) {
        entity_to_attach = registry.create();
    }

    switch (light.GetType())
    {
        case Light::Type::DIRECTIONAL:
        {
            registry.assign<render::DirectionalLight>(entity_to_attach, light.GetDirectionalLight());
            break;
        }
        case Light::Type::POINT:
        {
            registry.assign<render::PointLight>(entity_to_attach, light.GetPointLight());
            break;
        }
        case Light::Type::SPOT:
        {
            registry.assign<render::SpotLight>(entity_to_attach, light.GetSpotLight());
            break;
        }
    }

    // Assign a new transform if one does not exist
    if (!registry.has<Transform>(entity_to_attach)) {
        registry.assign<Transform>(entity_to_attach, Transform{});
    }

    return entity_to_attach;
}