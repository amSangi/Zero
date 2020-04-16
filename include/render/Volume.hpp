#pragma once

#include "core/Component.hpp"
#include "math/Sphere.hpp"
#include "math/Vector3.hpp"

namespace zero::render {

    /**
     * @brief A volume component encapsulating an entity
     */
    struct Volume : public Component {

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
         * @brief Construct a volume from a Axis-Aligned Bounding Box
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
         * @brief The bounding sphere of the entity
         */
        math::Sphere bounding_volume_;
    }; // struct Volume

} // namespace zero::render
