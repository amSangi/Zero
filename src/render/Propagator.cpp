#include <set>
#include <stack>
#include <queue>
#include "core/Transform.hpp"
#include "render/Propagator.hpp"
#include "math/Vector3.hpp"
#include "math/Vector4.hpp"

using namespace zero::math;
using namespace zero::render;

void Propagator::PropagateTransform(entt::registry& registry) const {
    auto view = registry.view<Transform, Volume>();
    std::stack<Component::Entity> update_stack;

    // Retrieve root parents with children
    for (auto entity : view) {
        auto& transform = view.get<Transform>(entity);
        if (transform.parent_ == entt::null && !transform.children_.empty()) {
            update_stack.push(entity);
        }
    }

    // Begin downward propagation
    while (!update_stack.empty()) {
        auto& transform = view.get<Transform>(update_stack.top());
        update_stack.pop();

        // Apply transformation to child entities
        for (auto child_entity : transform.children_) {
            // Add child to update_stack to propagate to its children
            update_stack.push(child_entity);

            auto& child_transform = view.get<Transform>(child_entity);
            auto& child_volume = view.get<Volume>(child_entity);

            // Compute new world transform for the child entity
            const auto parent_matrix = transform.GetLocalToWorldMatrix();
            const auto child_matrix = child_transform.GetLocalToWorldMatrix();
            Matrix4x4 result = parent_matrix * child_matrix;

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
        if (transform.children_.empty() && transform.parent_ != entt::null) {
            update_queue.push(entity);
        }
    }

    // Begin upward propagation
    while (!update_queue.empty()) {
        auto entity = update_queue.front();
        auto& volume = view.get<Volume>(entity);
        auto& transform = view.get<Transform>(entity);
        update_queue.pop();

        if (transform.parent_ == entt::null) continue;

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