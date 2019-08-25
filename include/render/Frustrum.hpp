#pragma once

#include "math/Plane.hpp"
#include "Components.hpp"

namespace zero::render {

    /**
     * @brief A Camera view frustrum represented by 6 planes
     */
    class Frustrum {
    public:
        /**
         * @brief Constructor
         * @param camera the camera to construct the frustrum from
         */
        explicit Frustrum(Camera camera);

        /**
         * @brief Is the sphere inside the frustrum?
         * @param sphere the sphere
         * @return true if the sphere is inside the frustrum. Otherwise false.
         */
        [[nodiscard]] bool Contains(const math::Sphere& sphere) const;

        /**
         * @brief Is the box inside the frustrum?
         * @param box the box
         * @return true if the box is inside the frustrum. Otherwise false.
         */
        [[nodiscard]] bool Contains(const math::Box& box) const;

        /**
         * @brief Is the plane inside the frustrum?
         * @param plane the plane
         * @return true if the plane is inside the frustrum. Otherwise false.
         */
        [[nodiscard]] bool Contains(const math::Plane& plane) const;

        /**
         * @brief Is the point inside the frustrum?
         * @param point the point
         * @return true if the point is inside the frustrum. Otherwise false. 
         */
        [[nodiscard]] bool Contains(const math::Vec3f& point) const;

    private:
        /**
         * @brief The left (-x) plane
         */
        math::Plane left_;

        /**
         * @brief The right (+x) plane
         */
        math::Plane right_;

        /**
         * @brief The forward (+z) plane
         */
        math::Plane forward_;

        /**
         * @brief The back (-z) plane
         */
        math::Plane back_;

        /**
         * @brief The up (+y) plane
         */
        math::Plane up_;

        /**
         * @brief THe down (-y) plane
         */
        math::Plane down_;

    }; // class Frustrum

} // namespace zero::render