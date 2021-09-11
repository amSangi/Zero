#include "core/TransformPropagator.hpp"
#include <algorithm>
#include <queue>
#include <stack>

namespace zero
{

void TransformPropagator::RemoveChild(entt::registry& registry, Entity parent, Entity child)
{
    if (registry.valid(parent))
    {
        // Remove child reference if the parent is valid
        auto& parent_transform = registry.get<Transform>(parent);
        auto& children = parent_transform.children_;
        children.erase(std::remove(children.begin(),
                                   children.end(),
                                   child),
                       children.end());
    }
    if (registry.valid(child))
    {
        // Detach from parent if the child is valid
        auto& child_transform = registry.get<Transform>(child);
        child_transform.parent_ = NullEntity;
    }
}

void TransformPropagator::RemoveChildren(entt::registry& registry, Entity parent)
{
    auto& transform = registry.get<Transform>(parent);
    for (Entity child_entity : transform.children_)
    {
        if (!registry.valid(child_entity))
        {
            continue;
        }
        auto& child_transform = registry.get<Transform>(child_entity);
        child_transform.parent_ = NullEntity;
    }
    transform.children_.clear();
}

void TransformPropagator::RemoveParent(entt::registry& registry, Entity entity)
{
    auto& transform = registry.get<Transform>(entity);
    if (transform.parent_ != NullEntity)
    {
        RemoveChild(registry, transform.parent_, entity);
    }
}

void TransformPropagator::PropagateMarkForDestruction(entt::registry& registry)
{
    auto view = registry.view<Transform>();
    std::stack<Entity> update_stack;

    // Predicate for whether an entity should have its destruction propagated
    auto ShouldPropagateDestruction = [](const Transform& transform)
    {
        return transform.state_ == Transform::State::MARKED_FOR_DELETE
               && !transform.keep_children_alive_
               && !transform.children_.empty();
    };

    // Retrieve root parents
    for (Entity entity : view)
    {
        auto& transform = view.get<Transform>(entity);
        if (transform.parent_ == NullEntity
            && ShouldPropagateDestruction(transform)) {
            update_stack.push(entity);
        }
    }

    // Begin downward mark propagation
    while (!update_stack.empty())
    {
        auto& transform = view.get<Transform>(update_stack.top());
        update_stack.pop();

        for (Entity child_entity : transform.children_)
        {
            if (!registry.valid(child_entity))
            {
                continue;
            }

            auto& child_transform = view.get<Transform>(child_entity);
            child_transform.state_ = Transform::State::MARKED_FOR_DELETE;

            if (!child_transform.keep_children_alive_ && !transform.children_.empty())
            {
                update_stack.push(child_entity);
            }
        }
    }
}

void TransformPropagator::PropagateTransform(entt::registry& registry)
{
	auto transform_view = registry.view<Transform>();
	std::vector<Entity> root_parent_entities;

	// Retrieve root parents with children
	for (Entity entity : transform_view)
	{
		auto& transform = transform_view.get<Transform>(entity);
		if (transform.parent_ == NullEntity && !transform.children_.empty())
		{
			root_parent_entities.push_back(entity);
		}
	}

	// Begin downward propagation
	for (Entity root_parent_entity : root_parent_entities)
	{
		Transform& root_transform = transform_view.get<Transform>(root_parent_entity);

		// Skip unmodified entities
		if (!root_transform.IsModified())
		{
			continue;
		}

		// Apply transformation to child entities
		std::queue<Entity> entities{};
		entities.push(root_parent_entity);
		while (!entities.empty())
		{
			Entity entity = entities.front();
			entities.pop();

			Transform& parent_transform = transform_view.get<Transform>(entity);
			for (Entity child_entity : parent_transform.children_)
			{
				if (!registry.valid(child_entity))
				{
					continue;
				}

				Transform& child_transform = transform_view.get<Transform>(child_entity);
				child_transform.Scale(parent_transform.GetScale());
				child_transform.Rotate(parent_transform.GetOrientation());
				child_transform.Translate(parent_transform.GetPosition());
				entities.push(child_entity);
			}
		}
	}
}

void TransformPropagator::ClearCachedTransformations(entt::registry& registry)
{
    auto view = registry.view<Transform>();
    for (Entity entity : view)
    {
        auto& transform = view.get<Transform>(entity);
        transform.ClearCachedTransformation();
    }
}
} // namespace zero