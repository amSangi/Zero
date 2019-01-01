#pragma once

#include "Vector3.h"

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
         * @brief Check if a frustrum is inside the box
         * @param frustrum The frustrum
         * @return True if the frsutrum is inside this. Otherwise false.
         */
        bool Contains(const Frustrum& frustrum) const;

        /**
         * @brief Check if a ray is inside the box
         * @param ray The ray
         * @return True if the ray is inside this. Otherwise false.
         */
        bool Contains(const Ray& ray) const;

        /**
         * @brief Check if a sphere is inside the box
         * @param sphere The sphere
         * @return True if the sphere is inside this. Otherwise false.
         */
        bool Contains(const Sphere& sphere) const;

        /**
         * @brief Check if a point is inside the box
         * @param point The point
         * @return True if the point is inside this. Otherwise false.
         */
        bool Contains(const Vector3& point) const;

        /* ********** Merge ********** */
        void Merge(const Box& other);

        /* ********** Static Operations ********** */
        static Box Merge(const Box& lhs, const Box& rhs);


    public:
        Vector3 min;
        Vector3 max;

    }; // class Box

} // namespace Zero