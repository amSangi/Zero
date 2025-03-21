#pragma once

#include "component/Component.hpp"
#include "math/Sphere.hpp"
#include "math/Vector3.hpp"

namespace zero
{

    /**
     * @brief A volume component encapsulating an entity
     */
    struct Volume : public Component
    {

        /**
         * @brief Default constructor
         */
        Volume();

        /**
         * @brief Construct a volume with a given position and radius
         * @param position the position of the volume
         * @param radius the radius of the volume
         */
        Volume(const math::Vec3f& position, float radius);

        /**
         * @brief Construct a volume from an Axis-Aligned Bounding Box
         * @param min the minimum point of the box
         * @param max the maximum point of the box
         */
        Volume(const math::Vec3f& min, const math::Vec3f& max);

        /**
         * @brief Engulf the other volume so that the other volume is inside
         * @param other the other volume
         */
        void Engulf(const Volume& other);

        /**
         * @brief Apply a matrix transformation to the volume
         * @param transformation the transformation matrix to apply
         */
        void Transform(const math::Matrix4x4& transformation);

        /**
         * Translate the volume
         * @param translation the translation to perform
         */
        void Translate(const math::Vec3f& translation);

        /**
         * Scale the volume
         * @param scale the scale factor to apply to the bounding volume
         */
        void Scale(const math::Vec3f& scale);

        /**
         * Rotate the volume
         * @param rotation the amount to rotate the volume by
         */
        void Rotate(const math::Quaternion& rotation);

        /**
         * @brief The bounding sphere of the entity
         */
        math::Sphere bounding_volume_;

    }; // struct Volume

} // namespace zero
