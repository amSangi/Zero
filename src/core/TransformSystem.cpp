#include "core/TransformSystem.hpp"
#include "component/Volume.hpp"
#include <queue>

namespace zero
{

bool TransformSystem::AddChild(entt::registry& registry, Entity parent, Entity child)
{
	Transform& parent_transform = registry.get<Transform>(parent);
	Transform& child_transform = registry.get<Transform>(child);

	// Duplicate children not allowed
	if (std::find(parent_transform.children_.cbegin(), parent_transform.children_.cend(), child) != parent_transform.children_.cend())
	{
		return false;
	}
	// Child already has a parent
	if (child_transform.parent_ != NullEntity)
	{
		return false;
	}

	// Parent cannot be a descendent of child
	bool is_descendent = false;
	auto callback = [&is_descendent, entity_to_find = parent](entt::registry&, const Entity entity)
	{
		if (entity == entity_to_find)
		{
			is_descendent = true;
		}
	};
	TraverseTransformHierarchy(registry, child, callback);
	if (is_descendent)
	{
		return false;
	}

	parent_transform.children_.push_back(child);
	child_transform.parent_ = parent;
	return true;
}

void TransformSystem::RemoveChild(entt::registry& registry, Entity parent, Entity child)
{
	Transform& parent_transform = registry.get<Transform>(parent);
	Transform& child_transform = registry.get<Transform>(child);
	parent_transform.children_.erase(
		std::find(parent_transform.children_.begin(), parent_transform.children_.end(), child),
		parent_transform.children_.end()
	);
	child_transform.parent_ = NullEntity;
}

void TransformSystem::DestroyEntity(entt::registry& registry, const Entity entity)
{
	const Transform& transform = registry.get<Transform>(entity);
	if (transform.parent_ != NullEntity)
	{
		RemoveChild(registry, transform.parent_, entity);
	}

	// Retrieve all entities in the hierarchy
	std::vector<Entity> entities_to_delete{};
	auto callback = [&entities_to_delete](entt::registry&, const Entity visited_entity)
	{
		entities_to_delete.push_back(visited_entity);
	};
	TraverseTransformHierarchy(registry, entity, callback);
	registry.destroy(entities_to_delete.cbegin(), entities_to_delete.cend());
}

void TransformSystem::UpdateTransform(entt::registry& registry, const Entity root, const math::Matrix4x4& transformation)
{
	Transform& root_transform = registry.get<Transform>(root);

	// Update the world transformation
	const math::Matrix4x4 world_matrix = root_transform.GetLocalToWorldMatrix();
	const math::Matrix4x4 root_transformed_world_matrix = transformation * world_matrix;
	root_transform.scale_ = root_transformed_world_matrix.GetScale();
	root_transform.orientation_ = root_transformed_world_matrix.GetRotation().Unit();
	root_transform.position_ = root_transformed_world_matrix.GetTranslation();

	// Ensure volume is in sync with new transformation
	Volume& volume = registry.get<Volume>(root);
	volume.Transform(transformation);

	auto callback = [root, transformation](entt::registry& callback_registry, const Entity entity)
	{
		if (root == entity)
		{
			// Top level transformation has been updated
			return;
		}

		Transform& entity_transform = callback_registry.get<Transform>(entity);
		const Transform& parent_transform = callback_registry.get<Transform>(entity_transform.parent_);

		const math::Matrix4x4 parent_world_matrix = parent_transform.GetLocalToWorldMatrix();
		const math::Matrix4x4 local_entity_matrix = entity_transform.GetLocalToParentMatrix();

		// Recompute entity's world matrix because the parent world matrix has changed
		const math::Matrix4x4 transformed_world_matrix = parent_world_matrix * local_entity_matrix;

		entity_transform.scale_ = transformed_world_matrix.GetScale();
		entity_transform.orientation_ = transformed_world_matrix.GetRotation().Unit();
		entity_transform.position_ = transformed_world_matrix.GetTranslation();

		// Ensure volume is in sync with new transformation
		Volume& entity_volume = callback_registry.get<Volume>(entity);
		entity_volume.Transform(transformation);
	};
	TraverseTransformHierarchy(registry, root, callback);
}

void TransformSystem::Translate(entt::registry& registry, Entity root, const math::Vec3f& translation)
{
	Transform& root_transform = registry.get<Transform>(root);
	root_transform.position_ += translation;

	// Ensure volume is in sync with new transformation
	Volume& volume = registry.get<Volume>(root);
	volume.Translate(translation);

	auto callback = [root, translation](entt::registry& callback_registry, const Entity entity)
	{
		if (root == entity)
		{
			// Top level transformation has been updated
			return;
		}

		Transform& entity_transform = callback_registry.get<Transform>(entity);
		const Transform& parent_transform = callback_registry.get<Transform>(entity_transform.parent_);

		const math::Matrix4x4 parent_world_matrix = parent_transform.GetLocalToWorldMatrix();
		const math::Matrix4x4 local_entity_matrix = entity_transform.GetLocalToParentMatrix();

		// Recompute entity's world matrix because the parent world matrix has changed
		const math::Matrix4x4 transformed_world_matrix = parent_world_matrix * local_entity_matrix;
		entity_transform.position_ = transformed_world_matrix.GetTranslation();

		// Ensure volume is in sync with new transformation
		Volume& entity_volume = callback_registry.get<Volume>(entity);
		entity_volume.Translate(translation);
	};
	TraverseTransformHierarchy(registry, root, callback);
}

void TransformSystem::Rotate(entt::registry& registry, Entity root, const math::Quaternion& rotation)
{
	Transform& root_transform = registry.get<Transform>(root);
	root_transform.orientation_ *= rotation;

	// Ensure volume is in sync with new transformation
	Volume& volume = registry.get<Volume>(root);
	volume.Rotate(rotation);

	auto callback = [root, rotation](entt::registry& callback_registry, const Entity entity)
	{
		if (root == entity)
		{
			// Top level transformation has been updated
			return;
		}

		Transform& entity_transform = callback_registry.get<Transform>(entity);
		const Transform& parent_transform = callback_registry.get<Transform>(entity_transform.parent_);

		const math::Matrix4x4 parent_world_matrix = parent_transform.GetLocalToWorldMatrix();
		const math::Matrix4x4 local_entity_matrix = entity_transform.GetLocalToParentMatrix();

		// Recompute entity's world matrix because the parent world matrix has changed
		const math::Matrix4x4 transformed_world_matrix = parent_world_matrix * local_entity_matrix;
		entity_transform.orientation_ = transformed_world_matrix.GetRotation().Unit();

		// Ensure volume is in sync with new transformation
		Volume& entity_volume = callback_registry.get<Volume>(entity);
		entity_volume.Rotate(rotation);
	};
	TraverseTransformHierarchy(registry, root, callback);
}

void TransformSystem::Scale(entt::registry& registry, Entity root, const math::Vec3f& scale)
{
	Transform& root_transform = registry.get<Transform>(root);
	root_transform.scale_ *= scale;

	// Ensure volume is in sync with new transformation
	Volume& volume = registry.get<Volume>(root);
	volume.Scale(scale);

	auto callback = [root, scale](entt::registry& callback_registry, const Entity entity)
	{
		if (root == entity)
		{
			// Top level transformation has been updated
			return;
		}

		Transform& entity_transform = callback_registry.get<Transform>(entity);
		const Transform& parent_transform = callback_registry.get<Transform>(entity_transform.parent_);

		const math::Matrix4x4 parent_world_matrix = parent_transform.GetLocalToWorldMatrix();
		const math::Matrix4x4 local_entity_matrix = entity_transform.GetLocalToParentMatrix();

		// Recompute entity's world matrix because the parent world matrix has changed
		const math::Matrix4x4 transformed_world_matrix = parent_world_matrix * local_entity_matrix;
		entity_transform.orientation_ = transformed_world_matrix.GetRotation().Unit();

		// Ensure volume is in sync with new transformation
		Volume& entity_volume = callback_registry.get<Volume>(entity);
		entity_volume.Scale(scale);
	};
	TraverseTransformHierarchy(registry, root, callback);
}

void TransformSystem::TraverseTransformHierarchy(entt::registry& registry,
												 const Entity entity,
												 const std::function<void(entt::registry&, Entity)>& callback)
{
	const auto transform_view = registry.view<const Transform>();

	std::queue<Entity> entities_to_visit{};
	entities_to_visit.push(entity);

	while (!entities_to_visit.empty())
	{
		const Entity entity_to_visit = entities_to_visit.front();
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