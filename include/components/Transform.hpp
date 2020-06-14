#pragma once

#include "components/Component.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

namespace zero
{

    /**
     * @brief The transform component containing position, orientation, and scale information
     */
    struct Transform : public HierarchyComponent
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
         * @brief Check if the transform is equal to another transform
         * @param other the other transform
         * @return True if the global and local components are equal. Otherwise false.
         */
        bool operator==(const Transform& other) const;

        /**
         * @brief Check if the transform is not equal to another transform
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
         * @brief Get the cached transformation from local coordinates to world coordinates
         * @return the cached transformation matrix
         */
        [[nodiscard]] math::Matrix4x4 GetCachedLocalToWorldMatrix() const;

        /**
         * @brief Matrix that represents local coordinates relative to the parent matrix
         * @return the transformation matrix
         */
        [[nodiscard]] math::Matrix4x4 GetLocalToParentMatrix() const;

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
         * @brief Clear the cached transformation
         */
        void ClearCachedTransformation();

        /**
         * @brief Has the transform been modified?
         * @return True if the transform was modified. Otherwise false.
         */
        [[nodiscard]] bool IsModified() const;

        /**
         * @brief Get the position in the world
         * @return the position
         */
        [[nodiscard]] const math::Vec3f& GetPosition() const;

        /**
         * @brief Get the local position relative to the parent
         * @return the local position
         */
        [[nodiscard]] const math::Vec3f& GetLocalPosition() const;

        /**
         * @brief Get the scale in the world
         * @return the scale
         */
        [[nodiscard]] const math::Vec3f& GetScale() const;

        /**
         * @brief Get the local scale relative to the parent
         * @return the local scale
         */
        [[nodiscard]] const math::Vec3f& GetLocalScale() const;

        /**
         * @brief Get the orientation in the world
         * @return the orientation
         */
        [[nodiscard]] const math::Quaternion& GetOrientation() const;

        /**
         * @brief Get the local orientation relative to the parent
         * @return the local orientation
         */
        [[nodiscard]] const math::Quaternion& GetLocalOrientation() const;

    private:

        /**
         * @brief The position in the world
         */
        math::Vec3f position_;

        /**
         * @brief The position relative to the parent transform
         */
        math::Vec3f local_position_;

        /**
         * @brief The cached translation transformation
         */
        math::Vec3f cached_translation_;

        /**
         * @brief The scale relative to the world
         */
        math::Vec3f scale_;

        /**
         * @brief The scale relative to the parent transform
         */
        math::Vec3f local_scale_;

        /**
         * @brief The cached scale transformation
         */
        math::Vec3f cached_scale_;

        /**
         * @brief The orientation in the world
         */
        math::Quaternion orientation_;

        /**
         * @brief The orientation relative to the parent transform
         */
        math::Quaternion local_orientation_;

        /**
         * @brief The cached rotation transformation
         */
        math::Quaternion cached_rotation_;

        /**
         * @brief Has the transform been modified?
         */
        bool is_modified_;
    }; // struct Transform

} // namespace zero