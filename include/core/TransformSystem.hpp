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
    	 * Destroy an Entity and all of its children.
    	 *
    	 * To avoid destroying an Entity's children,
    	 * detach its children using `DetachChildren` and delete it from the registry normally.
    	 *
         * @brief Destroy an entity and all of its children from the registry
         * @param registry the registry containing the entities and their components
         * @param entity the entity to destroy
         */
    	static void DestroyEntity(entt::registry& registry, Entity entity);

    	/**
         * @brief Remove a child from a parent
         * @param registry the registry containing the entities and their components
         * @param parent the parent entity
         * @param child the child entity
         */
    	static void RemoveChild(entt::registry& registry, Entity parent, Entity child);

    	/**
         * @brief Detach an entity from its parent
         * @param registry the registry containing the entities and their components
         * @param entity the entity
         */
    	static void DetachFromParent(entt::registry& registry, Entity entity);

        /**
         * @brief Remove all the children from a parent Entity
         * @param registry the registry containing the entities and their components
         * @param parent the parent entity
         */
        static void DetachChildren(entt::registry& registry, Entity parent);

        /**
         * @brief Update the transformation of an Entity and propagate it to the Entity's children
         *
         * @note The transformation is applied to the Volume component too.
         *
         * @param registry the registry containing the entities and their components
         * @param top_level_entity the top level entity to transform
         * @param transformation the transformation to apply to the entity and its children
         */
        static void UpdateTransform(entt::registry& registry, Entity top_level_entity, const math::Matrix4x4& transformation);

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
