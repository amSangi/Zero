#include "render/VolumePropagator.hpp"
#include <set>
#include <queue>
#include "components/Transform.hpp"
#include "components/Volume.hpp"

namespace zero::render
{

void VolumePropagator::PropagateVolume(entt::registry& registry)
{
    auto view = registry.view<Volume, Transform>();
    std::queue<Entity> update_queue;
    // Use to prevent duplicate operations
    std::set<Entity> parents_to_visit;

    // Retrieve root entities and transform volumes
    for (Entity entity : view)
    {
        auto& volume = view.get<Volume>(entity);
        auto& transform = view.get<Transform>(entity);
        // Retrieve root entity
        if (transform.children_.empty() && transform.parent_ != NullEntity)
        {
            update_queue.push(entity);
        }
        // Apply cached transformations to volume
        if (transform.IsModified())
        {
            volume.Transform(transform.GetCachedLocalToWorldMatrix());
        }
    }

    // Begin upward propagation
    while (!update_queue.empty())
    {
        Entity entity = update_queue.front();
        auto& volume = view.get<Volume>(entity);
        auto& transform = view.get<Transform>(entity);
        update_queue.pop();

        if (transform.parent_ == NullEntity || !registry.valid(transform.parent_))
        {
            continue;
        }

        // Enlarge parent volume
        auto& parent_volume = view.get<Volume>(transform.parent_);
        parent_volume.Engulf(volume);

        // Add parent to queue if it hasn't already been added
        if (parents_to_visit.find(transform.parent_) == parents_to_visit.end())
        {
            parents_to_visit.insert(transform.parent_);
            update_queue.push(transform.parent_);
        }
    }
}

} // namespace zero::render