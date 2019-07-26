#pragma once

#include <vector>
#include <entt.hpp>
#include "math/Matrix4x4.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

namespace zero {

    /**
     * @brief The Component base in the Entity-Component-System
     */
    struct Component {
        using Entity = entt::entity;
    }; // struct Component

    /**
     * @brief A general hierarchical component
     */
    struct ParentComponent : public Component {

        /**
         * @brief Default constructor
         */
        ParentComponent()
        : Component()
        , parent_(entt::null)
        , children_()
        {}

        /**
         * @brief Construct with a given parent entity
         * @param parent the parent of this component
         */
        explicit ParentComponent(Entity parent)
        : Component()
        , parent_(parent)
        , children_()
        {}

        /**
         * @brief The parent entity
         */
        Entity parent_;

        /**
         * @brief The list of child entities
         */
        std::vector<Entity> children_;
    }; // struct ParentComponent

} // namespace zero