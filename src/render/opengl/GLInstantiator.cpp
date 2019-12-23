#include <render/opengl/GLInstantiator.hpp>
#include "render/opengl/GLModel.hpp"
#include "render/Components.hpp"
#include "core/Transform.hpp"

using namespace zero::render;

zero::Component::Entity GLInstantiator::InstantiateModel(entt::registry& registry, std::shared_ptr<GLModel> gl_model) {
    auto entity = registry.create();
    registry.assign<render::Volume>(entity, gl_model->GetVolume());
    registry.assign<render::Material>(entity, gl_model->GetMaterial());
    registry.assign<render::MeshInstance>(entity, gl_model->GetMeshInstance());

    Transform transform = Transform::FromMatrix4x4(gl_model->GetTransformation());
    transform.parent_ = Component::NullEntity;
    // TODO: Instantiate children recursively

    registry.assign<Transform>(entity, transform);
    return entity;
}