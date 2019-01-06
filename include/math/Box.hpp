#pragma once

#include "Vector3.hpp"

namespace Zero {

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
        void Merge(const Box& other);

        /* ********** Box Operations ********** */
        Vector3 Size() const;

        Vector3 Center() const;

        /* ********** Static Methods ********** */
        static Box Merge(const Box& lhs, const Box& rhs);

        static Box Unit();

    public:
        Vector3 min;
        Vector3 max;

    }; // class Box

} // namespace Zero