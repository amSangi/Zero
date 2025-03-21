#pragma once

#include <entt/entt.hpp>
#include "component/Transform.hpp"

namespace zero
{
	/**
	 * @brief TransformSystem operates on Transform components.
	 *
	 * All operations assume that the provided Entities are valid and have Transform components
	 */
	class TransformSystem
    {
    public:

    	TransformSystem() = delete;

        /**
         * Add a child entity to the given parent entity.
         * This can only be done if the child does not already have a parent.
         *
         * @param registry the registry containing all entities and their components
         * @param parent the parent entity to add a child to
         * @param child the child entity to add to the parent entity's Transform component
         * @return true if the parent-child relationship is set. Otherwise, false.
         */
        static bool AddChild(entt::registry& registry, Entity parent, Entity child);

        /**
         * Remove a child entity from the given parent entity.
         * The child must be a direct child of the parent. Not a nested child.
         *
         * @param registry the registry containing all entities and their components
         * @param parent the parent entity to remove a child from
         * @param child the child entity to remove from the parent
         */
        static void RemoveChild(entt::registry& registry, Entity parent, Entity child);

        /**
         * Destroy the given entity and all child entities.
         *
         * @param registry the registry containing all entities and their components
         * @param entity the entity to destroy
         */
        static void DestroyEntity(entt::registry& registry, Entity entity);

        /**
         * Update the transform of an entity and all of its children
         *
         * @param registry the registry containing all entities and their components
         * @param root the root entity to transform
         * @param transformation the transformation to apply to the entity and its children
         */
        static void UpdateTransform(entt::registry& registry, Entity root, const math::Matrix4x4& transformation);

		/**
		 * Translate the entity and its children
		 *
		 * @param registry the registry containing all entities and their components
		 * @param root the root entity to translate
		 * @param translation the translation to apply to the entity and its children
		 */
		static void Translate(entt::registry& registry, Entity root, const math::Vec3f& translation);
		/**
		 * Rotate the entity and its children
		 *
		 * @param registry the registry containing all entities and their components
		 * @param root the root entity to rotate
		 * @param rotation the rotation to apply to the entity and its children
		 */
		static void Rotate(entt::registry& registry, Entity root, const math::Quaternion& rotation);
		/**
		 * Scale the entity and its children
		 *
		 * @param registry the registry containing all entities and their components
		 * @param root the root entity to scale
		 * @param scale the scale to apply to the entity and its children
		 */
		static void Scale(entt::registry& registry, Entity root, const math::Vec3f& scale);

        /**
         * @brief Traverse every entity in a Transform hierarchy starting with a given entity
         *
         * @note The entity does not have to be a root-level entity. The traversal is performed in a breadth first
         * search manner, visiting each child entity. Also, all entities must be valid and have a Transform component.
         *
         * @warning Do not use this to delete an entity with a Transform component. Use `DestroyEntity` instead.
         *
         * @param registry the registry containing the entities and their components
         * @param entity the root entity to visit
         * @param callback the callback that is invoked on each Entity in the Transform hierarchy
         */
        static void TraverseTransformHierarchy(entt::registry& registry,
                                               Entity entity,
                                               const std::function<void(entt::registry&, Entity)>& callback);

    }; // class TransformSystem

} // namespace zero
