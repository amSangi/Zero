#include "render/VolumePropagator.hpp"
#include <set>
#include <queue>
#include "core/Transform.hpp"
#include "render/Volume.hpp"

using namespace zero::math;
using namespace zero::render;

void VolumePropagator::PropagateVolume(entt::registry& registry) {
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

        // Apply cached transformations to volume
        if (transform.IsModified()) {
            volume.Transform(transform.GetCachedLocalToWorldMatrix());
        }

        if (transform.parent_ == Component::NullEntity || !registry.valid(transform.parent_)) {
            continue;
        }

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