#pragma once

#include "Vector3.hpp"

namespace zero {
namespace math {

    /**
     * @brief A simple 3D box represented by min/max Vec3fs
     */
    class Box {
    public:
        Box() = default;
        Box(const Vec3f& min, const Vec3f& max);
        Box(const Box& other) = default;

        ~Box() = default;
        Box& operator=(const Box& other) = default;

        /**
         * @brief Check if the box is equal to another box
         * @param other The other box
         * @return True if the two boxes are equal. False otherwise.
         */
        bool operator==(const Box& other) const;

        /**
         * @brief Check if the box is not equal to another box
         * @param other The other box
         * @return True if the two boxes are not equal. False otherwise.
         */
        bool operator!=(const Box& other) const;

        /* ********** Intersection Tests ********** */

        /**
         * @brief Check if another box is inside the box
         * @param other The other box
         * @return True if the other box is inside this. Otherwise false.
         */
        bool Contains(const Box& other) const;

        /**
         * @brief Check if a point is inside the box
         * @param point The point
         * @return True if the point is inside this. Otherwise false.
         */
        bool Contains(const Vec3f& point) const;

        /**
         * @brief Check if another box intersects this box
         * @param other The other box
         * @return True if the other box intersects this. Otherwise false.
         */
        bool Intersects(const Box& other) const;

        /* ********** Merge ********** */

        /**
         * @brief Make this box the merger of this and the other box
         * @param other The other box
         */
        void Merge(const Box& other);

        /* ********** Box Operations ********** */

        /**
         * @return the size of this box
         */
        Vec3f Size() const;

        /**
         * @return the center position of this box
         */
        Vec3f Center() const;

        /* ********** Static Methods ********** */

        /**
         * @brief Return a new box that is the merger of lhs and rhs
         * @param lhs the first box
         * @param rhs the second box
         * @return a new box that is the merger of the two given boxes
         */
        static Box Merge(const Box& lhs, const Box& rhs);

        /**
         * @return a Box of unit size
         */
        static Box Unit();

        /**
         * @brief The minimum point of the 3D box
         */
        Vec3f min_;

        /**
         * @brief The maximum point of the 3D box
         */
        Vec3f max_;

    }; // class Box

} // namespace math
} // namespace zero