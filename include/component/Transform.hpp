#pragma once

#include "component/Component.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

namespace zero
{

    /**
     * @brief The transform component containing position, orientation, and scale information
     */
    struct Transform : public Component
    {

        /**
         * @brief Default constructor
         */
        Transform();

        /**
         * @brief Constructor for root transform entities
         * @param position the position relative to the world
         * @param scale the scale relative to the world
         * @param orientation the orientation relative to the world
         */
        Transform(const math::Vec3f& position,
                  const math::Vec3f& scale,
                  const math::Quaternion& orientation);

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
                  const math::Quaternion& local_orientation);

        static Transform FromMatrix4x4(const math::Matrix4x4& transformation);

        /**
         * @brief Check if the positional data is equal to another transform's positional data
         *
         * Does not check if parent/child relationships are equal.
         *
         * @param other the other transform
         * @return True if the global and local components are equal. Otherwise false.
         */
        bool operator==(const Transform& other) const;

        /**
         * @brief Check if the positional data is not equal to another transform's positional data
         *
         * Does not check if parent/child relationships are equal.
         *
         * @param other the other transform
         * @return True if the transforms are not equal. Otherwise false.
         */
        bool operator!=(const Transform& other) const;

        /**
         * @brief Matrix that transforms world coordinates to local coordinates
         * @return the transformation matrix
         */
        [[nodiscard]] math::Matrix4x4 GetWorldToLocalMatrix() const;

        /**
         * @brief Matrix that transforms local coordinates to world coordinates
         * @return the transformation matrix
         */
        [[nodiscard]] math::Matrix4x4 GetLocalToWorldMatrix() const;

        /**
         * @brief Matrix that represents local coordinates relative to the parent matrix
         * @return the transformation matrix
         */
        [[nodiscard]] math::Matrix4x4 GetLocalToParentMatrix() const;

        /**
         * @brief The position in the world
         */
        math::Vec3f position_;

        /**
         * @brief The position relative to the parent
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

        /**
         * @brief The parent entity
         *
         * This is set to NullEntity by default.
         */
        Entity parent_;

        /**
         * @brief The child entities
         */
        std::vector<Entity> children_;

    }; // struct Transform

} // namespace zero