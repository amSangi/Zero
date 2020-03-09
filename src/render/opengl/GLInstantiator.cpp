#include "render/opengl/GLInstantiator.hpp"
#include "render/opengl/GLDefaultShader.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/Components.hpp"
#include "core/Transform.hpp"

using namespace zero::render;

zero::Component::Entity GLInstantiator::InstantiateModel(entt::registry& registry,
                                                         const std::shared_ptr<GLModel>& gl_model,
                                                         Component::Entity parent) {
    auto entity = registry.create();
    registry.assign<render::Volume>(entity, gl_model->GetVolume());
    registry.assign<render::Material>(entity, gl_model->GetMaterial());
    registry.assign<render::ModelInstance>(entity, gl_model->GetModelInstance());

    Transform transform = gl_model->GetTransform();
    transform.parent_ = parent;
    for (const auto& child_gl_model : gl_model->GetChildren()) {
        auto child_entity = InstantiateModel(registry, child_gl_model, entity);
        transform.children_.push_back(child_entity);
    }

    registry.assign<Transform>(entity, transform);
    return entity;
}

zero::Component::Entity GLInstantiator::InstantiatePrimitive(entt::registry& registry,
                                                             PrimitiveInstance primitive) {
    auto entity = registry.create();
    Transform transform{};
    Volume volume{};
    switch (primitive.GetType()) {
        case PrimitiveInstance::Type::BOX:
        {
            const auto& box = primitive.GetBox();
            transform.position_ = box.Center();
            volume.bounding_volume_.center_ = transform.position_;
            volume.Engulf(Volume(box.min_, box.max_));
            break;
        }
        case PrimitiveInstance::Type::CONE:
        {
            const auto& cone = primitive.GetCone();
            // TODO: Update transform/volume
            break;
        }
        case PrimitiveInstance::Type::CYLINDER:
        {
            const auto& cylinder = primitive.GetCylinder();
            // TODO: Update transform/volume
            break;
        }
        case PrimitiveInstance::Type::PLANE:
        {
            const auto& plane = primitive.GetPlane();
            transform.position_ = plane.center_;
            transform.scale_ = math::Vec3f(plane.width_, plane.height_, 1.0F);
            volume.bounding_volume_.center_ = transform.position_;
            auto offset = math::Vec3f(plane.width_, plane.height_, 0.0F);
            auto min = transform.position_ - offset;
            auto max = transform.position_ + offset;
            volume.Engulf(Volume(min, max));
            break;
        }
        case PrimitiveInstance::Type::SPHERE:
        {
            const auto& sphere = primitive.GetSphere();
            transform.position_ = sphere.center_;
            transform.scale_ = math::Vec3f(sphere.radius_);
            volume.bounding_volume_ = math::Sphere(transform.position_, sphere.radius_ + math::kEpsilon);
            break;
        }
        case PrimitiveInstance::Type::TORUS:
        {
            const auto& torus = primitive.GetTorus();
            transform.position_ = torus.center_;
            volume.bounding_volume_ = math::Sphere(transform.position_, torus.outer_radius_ + math::kEpsilon);
            break;
        }
    }

    Material material{};
    material.shaders_.vertex_shader_ = GLDefaultShader::kVertexShader.name_;
    material.shaders_.fragment_shader_ = GLDefaultShader::kFragmentShader.name_;

    registry.assign<render::Volume>(entity, volume);
    registry.assign<render::Material>(entity, material);
    registry.assign<render::PrimitiveInstance>(entity, primitive);
    registry.assign<Transform>(entity, transform);
    return entity;
}