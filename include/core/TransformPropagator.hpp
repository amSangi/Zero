#pragma once

#include <entt/entt.hpp>
#include "component/Transform.hpp"

namespace zero
{
	class TransformPropagator
    {
    public:

    	TransformPropagator() = delete;

        /**
         * @brief Remove a child from a parent Transform
         * @param registry the registry containing the entities and their components
         * @param parent the parent entity
         * @param child the child entity
         */
        static void RemoveChild(entt::registry& registry, Entity parent, Entity child);

        /**
         * @brief Remove all the children from a parent Transform
         * @param registry the registry containing the entities and their components
         * @param parent the parent entity
         */
        static void RemoveChildren(entt::registry& registry, Entity parent);

        /**
         * @brief Detach an entity from its parent Transform
         * @param registry the registry containing the entities and their components
         * @param entity the entity
         */
        static void RemoveParent(entt::registry& registry, Entity entity);

        /**
         * @brief All entities that are marked for destruction have their children marked for destruction
         * if keep_children_alive_ is set to false.
         * @param registry the registry containing the entities and their components
         */
        static void PropagateMarkForDestruction(entt::registry& registry);

        /**
         * @brief Propagate transform data and update bounding volume position
         * @param registry the registry containing the entities and their components
         */
        static void PropagateTransform(entt::registry& registry);

        /**
         * @brief Clear the cached transformations
         *
         * This is cleared after the cached transformations have been applied to all child transforms and
         * all systems that wish to use the cached transform have been updated.
         *
         * @param registry the registry containing the entities and their components
         */
        static void ClearCachedTransformations(entt::registry& registry);


        /**
         * @brief Visit the transform hierarchy
         *
         * The visitor callback is only invoked on valid entities that contain at least a Transform component
         *
         * @param registry the registry containing the entities and their components
         * @param root_entity the root entity to start traversing
         * @param visitor the callback to invoke on each entity in the hierarchy, starting with the root entity
         */
//        static void VisitTransform(entt::registry& registry, Entity root_entity, std::function<void(Entity entity)> visitor);

    }; // class TransformPropagator

} // namespace zero
