#include "core/TransformSystem.hpp"
#include "component/Volume.hpp"
#include <queue>

namespace zero
{

void TransformSystem::DestroyEntity(entt::registry& registry, Entity entity)
{
	// Avoid a reference to a destroyed entity
	TransformSystem::DetachFromParent(registry, entity);

	// Retrieve all entities in the hierarchy
	std::vector<Entity> entities_to_delete{};
	auto callback = [&entities_to_delete](entt::registry&, Entity visited_entity)
	{
		entities_to_delete.push_back(visited_entity);
	};
	TransformSystem::TraverseTransformHierarchy(registry, entity, callback);

	registry.destroy(entities_to_delete.cbegin(), entities_to_delete.cend());
}

void TransformSystem::RemoveChild(entt::registry& registry, Entity parent, Entity child)
{
	Transform& parent_transform = registry.get<Transform>(parent);
	Transform& child_transform = registry.get<Transform>(child);

	// Remove reference in the parent's list of child entities
	std::vector<Entity>& children = parent_transform.children_;
	children.erase(std::remove(children.begin(), children.end(), child), children.end());

	child_transform.parent_ = NullEntity;
}

void TransformSystem::DetachFromParent(entt::registry& registry, Entity entity)
{
	Transform& transform = registry.get<Transform>(entity);
	TransformSystem::RemoveChild(registry, transform.parent_, entity);
}

void TransformSystem::DetachChildren(entt::registry& registry, Entity parent)
{
	auto transform_view = registry.view<Transform>();

	Transform& parent_transform = transform_view.get<Transform>(parent);
	for (Entity child_entity : parent_transform.children_)
	{
		Transform& child_transform = transform_view.get<Transform>(child_entity);
		child_transform.parent_ = NullEntity;
	}

	parent_transform.children_.clear();
}

void TransformSystem::UpdateTransform(entt::registry& registry, Entity top_level_entity, const math::Matrix4x4& transformation)
{
	Transform& top_level_transform = registry.get<Transform>(top_level_entity);

	// Update the world transformation
	math::Matrix4x4 world_matrix = top_level_transform.GetLocalToWorldMatrix();
	math::Matrix4x4 transformed_world_matrix = transformation * world_matrix;
	top_level_transform.scale_ = transformed_world_matrix.GetScale();
	top_level_transform.orientation_ = transformed_world_matrix.GetRotation().Unit();
	top_level_transform.position_ = transformed_world_matrix.GetTranslation();

	// Ensure volume is in sync with new transformation
	Volume& volume = registry.get<Volume>(top_level_entity);
	volume.Transform(transformation);

	auto callback = [top_level_entity, transformation](entt::registry& registry, Entity entity)
	{
		if (top_level_entity == entity)
		{
			// Top level transformation has been updated
			return;
		}

		Transform& entity_transform = registry.get<Transform>(entity);
		const Transform& parent_transform = registry.get<Transform>(entity_transform.parent_);

		const math::Matrix4x4 parent_world_matrix = parent_transform.GetLocalToWorldMatrix();
		const math::Matrix4x4 local_entity_matrix = entity_transform.GetLocalToParentMatrix();

		// Recompute entity's world matrix because the parent world matrix has changed
		math::Matrix4x4 transformed_world_matrix = parent_world_matrix * local_entity_matrix;

		entity_transform.scale_ = transformed_world_matrix.GetScale();
		entity_transform.orientation_ = transformed_world_matrix.GetRotation().Unit();
		entity_transform.position_ = transformed_world_matrix.GetTranslation();

		// Ensure volume is in sync with new transformation
		Volume& entity_volume = registry.get<Volume>(entity);
		entity_volume.Transform(transformation);
	};
	TransformSystem::TraverseTransformHierarchy(registry, top_level_entity, callback);
}

void TransformSystem::TraverseTransformHierarchy(entt::registry& registry,
												 Entity entity,
												 const std::function<void(entt::registry&, Entity)>& callback)
{
	auto transform_view = registry.view<const Transform>();

	std::queue<Entity> entities_to_visit{};
	entities_to_visit.push(entity);

	while (!entities_to_visit.empty())
	{
		Entity entity_to_visit = entities_to_visit.front();
		entities_to_visit.pop();

		const Transform& transform = transform_view.get<const Transform>(entity_to_visit);
		for (Entity child_entity : transform.children_)
		{
			entities_to_visit.push(child_entity);
		}

		callback(registry, entity_to_visit);
	}
}

} // namespace zero