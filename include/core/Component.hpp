#pragma once

#include <vector>
#include <entt.hpp>
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

namespace zero {

    /**
     * @brief The state of any component
     */
    enum class ComponentState {
        COMPONENT_CREATED,              ///< The component was created
        COMPONENT_UPDATED,              ///< The component was updated
        COMPONENT_MARKED_FOR_DELETE,    ///< The component has been marked for deletion
        COMPONENT_IDLE,                 ///< The component is idle
    }; // enum class ComponentState

    /**
     * @brief The Component base in the Entity-Component-System
     */
    struct Component {
        using Entity = entt::registry<>::entity_type;

        /**
         * @brief The entity that owns this component
         */
        Entity owner_;

        /**
         * @brief The state of the component
         */
        ComponentState state_ = ComponentState::COMPONENT_CREATED;
    }; // struct Component

    /**
     * @brief A general hierarchical component
     */
    struct ParentComponent : public Component {
        /**
         * @brief The parent entity
         */
        Entity parent_;

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
         * @brief The scale in 3D space
         */
        math::Vec3f scale_;

        /**
         * @brief The position in 3D space
         */
        math::Vec3f position_;

        /**
         * @brief The orientation in 3D space
         */
        math::Quaternion orientation_;
    }; // struct Transform

} // namespace zero