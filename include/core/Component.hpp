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

    /**
     * @brief The transform component containing position, orientation, and scale information
     */
    struct Transform : public ParentComponent {

        /**
         * @brief Default constructor
         */
        Transform()
        : ParentComponent()
        , position_(0.0f)
        , local_position_(0.0f)
        , scale_(1.0f)
        , local_scale_(1.0f)
        , orientation_()
        , local_orientation_()
        {}

        /**
         * @brief Constructor for root transform entities
         * @param position the position relative to the world
         * @param scale the scale relative to the world
         * @param orientation the orientation relative to the world
         */
         Transform(const math::Vec3f& position,
                   const math::Vec3f& scale,
                   const math::Quaternion& orientation)
         : ParentComponent()
         , position_(position)
         , local_position_(0.0f)
         , scale_(scale)
         , local_scale_(1.0f)
         , orientation_(orientation.UnitCopy())
         , local_orientation_()
        {}

        /**
         * @brief Constructor for child transform entities
         * @param parent the parent transform entity
         * @param parent_transform the parent transform
         * @param local_position the local position relative to the parent transform
         * @param local_scale the local scale relative to the parent transform
         * @param local_orientation the local orientation relative to the parent transform
         */
         Transform(Entity parent,
                   const Transform& parent_transform,
                   const math::Vec3f& local_position,
                   const math::Vec3f& local_scale,
                   const math::Quaternion& local_orientation)
         : ParentComponent(parent)
         , position_(parent_transform.position_ + local_position)
         , local_position_(local_position)
         , scale_(parent_transform.scale_ * local_scale)
         , local_scale_(local_scale)
         , orientation_((parent_transform.orientation_ * local_orientation).Unit())
         , local_orientation_(local_orientation.UnitCopy())
        {}

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