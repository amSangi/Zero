#include "render/opengl/GLInstantiator.hpp"
#include "render/opengl/GLDefaultShader.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/Components.hpp"
#include "core/Transform.hpp"
#include "math/Box.hpp"

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
        if (child_entity != Component::NullEntity) {
            transform.children_.push_back(child_entity);
        }
    }

    registry.assign<Transform>(entity, transform);
    return entity;
}

zero::Component::Entity GLInstantiator::InstantiatePrimitive(entt::registry& registry,
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
            transform.scale_ = math_box.max_;
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

    Material material{};
    material.shaders_.vertex_shader_ = GLDefaultShader::kVertexShader.name_;
    material.shaders_.fragment_shader_ = GLDefaultShader::kFragmentShader.name_;

    registry.assign<render::Volume>(entity, volume);
    registry.assign<render::Material>(entity, material);
    registry.assign<render::PrimitiveInstance>(entity, primitive);
    registry.assign<Transform>(entity, transform);
    return entity;
}