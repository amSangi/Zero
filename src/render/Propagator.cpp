#include <set>
#include <stack>
#include <queue>
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
            const auto parent_matrix = Matrix4x4::Identity()
                                       .Scale(transform.scale_)
                                       .Rotate(transform.orientation_)
                                       .Translate(transform.position_);
            const auto child_matrix = Matrix4x4::Identity()
                                     .Scale(child_transform.scale_)
                                     .Rotate(child_transform.orientation_)
                                     .Translate(child_transform.position_);
            Matrix4x4 result = parent_matrix * child_matrix;

            // Set the world transform
            child_transform.position_ = result.GetTranslation();
            child_transform.scale_ = result.GetScale();
            child_transform.orientation_ = result.GetRotation();

            // Transform bounding volume using new world transform
            TransformVolume(child_volume, result);
        }
    }
}

void Propagator::PropagateVolume(entt::registry& registry) const {
    auto view = registry.view<Volume>();
    std::queue<Component::Entity> update_queue;
    // Use to prevent duplicate operations
    std::set<Component::Entity> parents_to_visit;

    // Retrieve leaf entities - entities that have a parent but 0 children
    for (auto entity : view) {
        auto& volume = view.get(entity);
        if (volume.children_.empty() && volume.parent_ != entt::null) {
            update_queue.push(entity);
        }
    }

    // Begin upward propagation
    while (!update_queue.empty()) {
        auto& volume = view.get(update_queue.front());
        update_queue.pop();

        if (volume.parent_ == entt::null) continue;

        // Enlarge parent volume
        auto& parent_volume = view.get(volume.parent_);
        parent_volume.bounding_volume_.Merge(volume.bounding_volume_);

        // Add parent to queue if it hasn't already been added
        if (parents_to_visit.find(volume.parent_) == parents_to_visit.end()) {
            parents_to_visit.insert(volume.parent_);
            update_queue.push(volume.parent_);
        }
    }
}

void Propagator::TransformVolume(Volume& volume, const Matrix4x4& transformation) const {
    Vec3f center = volume.bounding_volume_.center_;

    // Apply transformation
    Vec4f transformed_center = transformation * Vec4f(center.x_, center.y_, center.z_, 1.0f);

    // Get the largest scale component
    const float sx = Vec3f(transformation[0][0], transformation[1][0], transformation[2][0]).SquareMagnitude();
    const float sy = Vec3f(transformation[0][1], transformation[1][1], transformation[2][1]).SquareMagnitude();
    const float sz = Vec3f(transformation[0][2], transformation[1][2], transformation[2][2]).SquareMagnitude();
    const float scale = zero::math::Sqrt(std::max({sx, sy, sz}));

    // Update volume
    volume.bounding_volume_.center_.x_ = transformed_center.x_;
    volume.bounding_volume_.center_.y_ = transformed_center.y_;
    volume.bounding_volume_.center_.z_ = transformed_center.z_;
    volume.bounding_volume_.radius_ *= scale;
}