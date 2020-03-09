#pragma once

#include <entt/entt.hpp>
#include "core/Component.hpp"
#include "core/Transform.hpp"
#include "math/Matrix4x4.hpp"

namespace zero::render {

    /**
     * @brief Propagates Transform, Volume, and Hierarchy information to child/parent components
     */
    class Propagator {
    public:

        Propagator() = default;

        /**
         * @brief Remove a child from a parent Transform
         * @param registry the registry containing the entities and their components
         * @param parent the parent entity
         * @param child the child entity
         */
        void RemoveChild(entt::registry& registry, Component::Entity parent, Component::Entity child) const;

        /**
         * @brief Detach all of the children of a parent Transform
         * @param registry the registry containing the entities and their components
         * @param parent the parent entity
         */
        void DetachChildrenTransform(entt::registry& registry, Component::Entity parent) const;

        /**
         * @brief Detach an entity from its parent Transform
         * @param registry the registry containing the entities and their components
         * @param entity the entity
         */
        void DetachFromParentTransform(entt::registry& registry, Component::Entity entity) const;

        /**
         * @brief All entities that are marked for destruction have their children marked for destruction
         * if keep_children_alive_ is set to false.
         * @param registry the registry containing the entities and their components
         */
        void PropagateMarkForDestruction(entt::registry& registry) const;

        /**
         * @brief Propagate transform data and update bounding volume position
         * @param registry the registry containing the entities and their components
         */
        void PropagateTransform(entt::registry& registry) const;

        /**
         * @brief Propagate bounding volume up and expand/contract
         * @param registry the registry containing the entities and their components
         */
        void PropagateVolume(entt::registry& registry) const;

    }; // class Propagator

} // namespace zero::render