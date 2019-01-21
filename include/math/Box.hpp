#pragma once

#include "Vector3.hpp"

namespace zero {

    class Box {
    public:
        Box() = default;
        Box(const Vector3& min, const Vector3& max);
        Box(const Box& other) = default;

        ~Box() = default;
        Box& operator=(const Box& other) = default;

        bool operator==(const Box& other) const;
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
        bool Contains(const Vector3& point) const;

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
        Vector3 Size() const;

        /**
         * @return the center position of this box
         */
        Vector3 Center() const;

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

        Vector3 min, max;

    }; // class Box

} // namespace zero