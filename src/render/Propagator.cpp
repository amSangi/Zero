#include "render/Propagator.hpp"
#include <algorithm>
#include <set>
#include <stack>
#include <queue>
#include "render/Components.hpp"

using namespace zero::math;
using namespace zero::render;

void Propagator::RemoveChild(entt::registry& registry, Component::Entity parent, Component::Entity child) const {
    if (registry.valid(parent)) {
        // Remove child reference if the parent is valid
        auto& parent_transform = registry.get<Transform>(parent);
        auto& children = parent_transform.children_;
        children.erase(std::remove(children.begin(),
                                   children.end(),
                                   child),
                       children.end());
    }
    if (registry.valid(child)) {
        // Detach from parent if the child is valid
        auto& child_transform = registry.get<Transform>(child);
        child_transform.parent_ = Component::NullEntity;
    }
}

void Propagator::DetachChildrenTransform(entt::registry& registry, Component::Entity parent) const {
    if (!registry.valid(parent)) return;
    auto& transform = registry.get<Transform>(parent);
    for (auto child_entity : transform.children_) {
        if (!registry.valid(child_entity)) continue;
        auto& child_transform = registry.get<Transform>(child_entity);
        child_transform.parent_ = Component::NullEntity;
    }
    transform.children_.clear();
}

void Propagator::DetachFromParentTransform(entt::registry& registry, Component::Entity entity) const {
    if (!registry.valid(entity)) return;
    auto& transform = registry.get<Transform>(entity);
    RemoveChild(registry, transform.parent_, entity);
}

void Propagator::PropagateMarkForDestruction(entt::registry& registry) const {
    auto view = registry.view<Transform>();
    std::stack<Component::Entity> update_stack;

    // Predicate for whether an entity should have its destruction propagated
    auto ShouldPropagateDestruction = [](Transform& transform) {
        return transform.state_ == Transform::State::MARKED_FOR_DELETE
               && !transform.keep_children_alive_
               && !transform.children_.empty();
    };

    // Retrieve root parents
    for (auto entity : view) {
        auto& transform = view.get(entity);
        if (transform.parent_ == Component::NullEntity
            && ShouldPropagateDestruction(transform)) {
            update_stack.push(entity);
        }
    }

    // Begin downward mark propagation
    while (!update_stack.empty()) {
        auto& transform = view.get(update_stack.top());
        update_stack.pop();

        for (auto child_entity : transform.children_) {
            if (!registry.valid(child_entity)) continue;

            auto& child_transform = view.get(child_entity);
            child_transform.state_ = Transform::State::MARKED_FOR_DELETE;

            if (!child_transform.keep_children_alive_ && !transform.children_.empty()) {
                update_stack.push(child_entity);
            }
        }
    }
}

void Propagator::PropagateTransform(entt::registry& registry) const {
    auto view = registry.view<Transform, Volume>();
    std::stack<Component::Entity> update_stack;

    // Retrieve root parents with children
    for (auto entity : view) {
        auto& transform = view.get<Transform>(entity);
        if (transform.parent_ == Component::NullEntity && !transform.children_.empty()) {
            update_stack.push(entity);
        }
    }

    // Begin downward propagation
    while (!update_stack.empty()) {
        auto& transform = view.get<Transform>(update_stack.top());
        update_stack.pop();

        // Apply transformation to child entities
        for (auto child_entity : transform.children_) {
            if (!registry.valid(child_entity)) continue;

            // Add child to update_stack to propagate to its children
            update_stack.push(child_entity);

            auto& child_transform = view.get<Transform>(child_entity);
            auto& child_volume = view.get<Volume>(child_entity);

            // Compute new world transform for the child entity
            const auto parent_matrix = transform.GetLocalToWorldMatrix();
            const auto child_matrix = child_transform.GetLocalToParentMatrix();
            Matrix4x4 result = child_matrix * parent_matrix;

            // Set the world transform by decomposing the resulting matrix
            child_transform.position_ = result.GetTranslation();
            child_transform.scale_ = result.GetScale();
            child_transform.orientation_ = result.GetRotation();

            // Transform bounding volume using new world transform
            child_volume.Transform(result);
        }
    }
}

void Propagator::PropagateVolume(entt::registry& registry) const {
    auto view = registry.view<Volume, Transform>();
    std::queue<Component::Entity> update_queue;
    // Use to prevent duplicate operations
    std::set<Component::Entity> parents_to_visit;

    // Retrieve leaf entities - entities that have a parent but 0 children
    for (auto entity : view) {
        auto& volume = view.get<Volume>(entity);
        auto& transform = view.get<Transform>(entity);
        if (transform.children_.empty() && transform.parent_ != Component::NullEntity) {
            update_queue.push(entity);
        }
    }

    // Begin upward propagation
    while (!update_queue.empty()) {
        auto entity = update_queue.front();
        auto& volume = view.get<Volume>(entity);
        auto& transform = view.get<Transform>(entity);
        update_queue.pop();

        if (transform.parent_ == Component::NullEntity || !registry.valid(transform.parent_)) continue;

        // Enlarge parent volume
        auto& parent_volume = view.get<Volume>(transform.parent_);
        parent_volume.Engulf(volume);

        // Add parent to queue if it hasn't already been added
        if (parents_to_visit.find(transform.parent_) == parents_to_visit.end()) {
            parents_to_visit.insert(transform.parent_);
            update_queue.push(transform.parent_);
        }
    }
}