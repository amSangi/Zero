#include <render/opengl/GLInstantiator.hpp>
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
                                                             const PrimitiveInstance& primitive) {
    // TODO: Instantiate a primitive
    return Component::NullEntity;
}