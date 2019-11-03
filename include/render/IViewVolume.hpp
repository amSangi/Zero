#pragma once

#include "math/Plane.hpp"
#include "Components.hpp"

namespace zero::render {

    /**
     * @brief A camera view volume interface used to cull volumes
     */
    class IViewVolume {
    public:

        IViewVolume() = default;

        virtual ~IViewVolume() = default;

        /**
         * @brief Is the sphere inside the view volume?
         * @param sphere the sphere
         * @return true if the sphere is inside the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool Contains(const math::Sphere& sphere) const = 0;

        /**
         * @brief Is the box inside the view volume?
         * @param box the box
         * @return true if the box is inside the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool Contains(const math::Box& box) const = 0;

        /**
         * @brief Is the plane inside the view volume?
         * @param plane the plane
         * @return true if the plane is inside the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool Contains(const math::Plane& plane) const = 0;

        /**
         * @brief Is the point inside the view volume?
         * @param point the point
         * @return true if the point is inside the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool Contains(const math::Vec3f& point) const = 0;

        /**
         * @brief Does the sphere intersect with the view volume?
         * @param sphere the sphere
         * @return true if the sphere intersects with the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool Intersects(const math::Sphere& sphere) const = 0;

        /**
         * @brief Does the box intersect with the view volume?
         * @param box the box
         * @return true if the box intersects with the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool Intersects(const math::Box& box) const = 0;

        /**
         * @brief Does the plane intersect with the view volume?
         * @param plane the plane
         * @return true if the plane intersects with the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool Intersects(const math::Plane& plane) const = 0;

    }; // class IViewVolume

} // namespace zero::render