#pragma once

#include "Component.hpp"

namespace zero {

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
         * @brief Matrix that transforms world coordinates to local coordinates
         * @return the transformation matrix
         */
        math::Matrix4x4 GetWorldToLocalMatrix() const;

        /**
         * @brief Matrix that transforms local coordinates to world coordinates
         * @return the transformation matrix
         */
        math::Matrix4x4 GetLocalToWorldMatrix() const;

        /**
         * @brief Matrix that represents local coordinates relative to the parent matrix
         * @return the transformation matrix
         */
        math::Matrix4x4 GetLocalToParentMatrix() const;

        /**
         * @brief Translate the position relative to the world
         * @param translation the translation vector
         * @return the caller
         */
        Transform& Translate(const math::Vec3f& translation);

        /**
         * @brief Translate the position relative to the parent entity
         * @param parent the parent transform
         * @param translation the translation vector
         * @return the caller
         */
        Transform& LocalTranslate(const Transform& parent, const math::Vec3f& translation);

        /**
         * @brief Rotate the transform using the quaternion relative to the world
         * @param rotation the rotation quaternion
         * @return the caller
         */
        Transform& Rotate(const math::Quaternion& rotation);

        /**
         * @brief Rotate the transform using the quaternion relative to the parent entity
         * @param parent the parent transform
         * @param rotation the rotation quaternion
         * @return the caller
         */
        Transform& LocalRotate(const Transform& parent, const math::Quaternion& rotation);

        /**
         * @brief Scale the entity
         * @param scale the scale
         * @return the caller
         */
        Transform& Scale(const math::Vec3f& scale);

        /**
         * @brief Scale the transform using the scale relative to the parent entity
         * @param parent the parent transform
         * @param scale the scale
         * @return the caller
         */
        Transform& LocalScale(const Transform& parent, const math::Vec3f& scale);

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