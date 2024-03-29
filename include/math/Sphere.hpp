#pragma once

#include "Vector3.hpp"

namespace zero::math
{

    /**
     * @brief A 3D sphere with a center and a radius
     */
    class Sphere
    {
    public:
        Sphere();
        explicit Sphere(float radius);
        explicit Sphere(const Vec3f& center);
        Sphere(const Vec3f& center, float radius);

        /**
         * @brief Construct a sphere from a Axis-Aligned Bounding Box
         * @param min the minimum point of the box
         * @param max the maximum point of the box
         */
        Sphere(const Vec3f& min, const Vec3f& max);

        ~Sphere() = default;
        Sphere& operator=(const Sphere& other) = default;

        /**
         * @brief Check if this sphere is equal to another sphere
         * @param other The other sphere
         * @return True if the center and radii are equal. False otherwise.
         */
        bool operator==(const Sphere& other) const;

        /**
         * @brief Check if this sphere is not equal to another sphere
         * @param other The other sphere
         * @return True if the centers or radii are not equal. False otherwise.
         */
        bool operator!=(const Sphere& other) const;

        /* ********** Intersection Tests ********** */
        /**
         * @brief Check if a box is inside the sphere
         * @param box The box
         * @return True if box is inside this. Otherwise false.
         */
        [[nodiscard]] bool Contains(const Box& box) const;

        /**
         * @brief Check if another sphere is inside the sphere
         * @param other The other sphere
         * @return True if the other sphere is inside this. Otherwise false.
         */
        [[nodiscard]] bool Contains(const Sphere& other) const;

        /**
         * @brief Check if a point is inside the sphere
         * @param point The point
         * @return True if the point is inside this. Otherwise false.
         */
        [[nodiscard]] bool Contains(const Vec3f& point) const;


        /**
         * @brief Check if another sphere intersects this sphere
         * @param other The other sphere
         * @return True if the other sphere intersects this. Otherwise false.
         */
        [[nodiscard]] bool Intersects(const Sphere& other) const;

        /* ********** Merge ********** */
        /**
         * @brief Merge this sphere with another sphere
         * @param other The other sphere
         */
        void Merge(const Sphere& other);

        /* ********** Static Operations ********** */

        /**
         * @brief Create a new sphere that contains the two given spheres
         * @param lhs The first sphere
         * @param rhs The second sphere
         * @return A new sphere that contains both lhs and rhs
         */
        static Sphere Merge(const Sphere& lhs, const Sphere& rhs);

        /**
         * @brief Create a Zero-point and Zero-radius sphere
         * @return The zero sphere
         */
        static Sphere Zero();

        /**
         * @brief The center point of the sphere
         */
        Vec3f center_;

        /**
         * @brief The radius of the sphere
         */
        float radius_;

    }; // class Sphere

} // namespace zero::math