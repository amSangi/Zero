#pragma once

#include <vector>
#include <entt.hpp>
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

namespace zero {

    /**
     * @brief The Component base in the Entity-Component-System
     */
    struct Component {
        using Entity = entt::registry::entity_type;

        /**
         * @brief The entity that owns this component
         */
        Entity owner_;
    }; // struct Component

    /**
     * @brief A general hierarchical component
     */
    struct ParentComponent : public Component {
        /**
         * @brief The parent entity
         */
        Entity parent_ = entt::null;

        /**
         * @brief The list of child entities
         */
        std::vector<Entity> children_;
    }; // struct ParentComponent

    /**
     * @brief The transform component containing position, orientation, and scale information
     */
    struct Transform : public ParentComponent {
        /**
         * @brief The position in the world
         */
        math::Vec3f position_;

        /**
         * @brief The position relative to the parent transform
         */
        math::Vec3f local_position_;

        /**
         * @brief The scale relative to the world
         */
        math::Vec3f scale_;

        /**
         * @brief The scale relative to the parent transform
         */
        math::Vec3f local_scale_;

        /**
         * @brief The orientation in the world
         */
        math::Quaternion orientation_;

        /**
         * @brief The orientation relative to the parent transform
         */
        math::Quaternion local_orientation_;
    }; // struct Transform

} // namespace zero