 #pragma once

#include <vector>
#include <entt/entt.hpp>

namespace zero
{

    using Entity = entt::entity;
    static constexpr auto NullEntity = entt::null;

    /**
     * @brief The Component base in the Entity-Component-System
     */
    struct Component {}; // struct Component

    /**
     * @brief A general hierarchical component
     */
    struct HierarchyComponent : public Component
    {

        /**
         * @brief The state of the entity
         */
        enum class State
        {
            IDLE,                ///< Default entity state
            MARKED_FOR_DELETE,   ///< The entity has been marked for delete
        }; // enum class State

        /**
         * @brief Default constructor
         */
        HierarchyComponent()
        : Component()
        , parent_(NullEntity)
        , children_()
        , state_(State::IDLE)
        , keep_children_alive_(false)
        {}

        /**
         * @brief Construct with a given parent entity
         * @param parent the parent of this component
         */
        explicit HierarchyComponent(Entity parent)
        : Component()
        , parent_(parent)
        , children_()
        , state_(State::IDLE)
        , keep_children_alive_(false)
        {}

        /**
         * @brief The parent entity
         */
        Entity parent_;

        /**
         * @brief The list of child entities
         */
        std::vector<Entity> children_;

        /**
         * @brief The entity state
         */
        State state_;

        /**
         * @brief Specifies if the children should be kept alive if the entity is destroyed
         */
        bool keep_children_alive_;

    }; // struct ParentComponent

} // namespace zero