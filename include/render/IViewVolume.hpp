#pragma once

#include "math/Plane.hpp"
#include "Components.hpp"

namespace zero::render {

    /**
     * @brief A camera view volume interface used to cull volumes
     */
    class IViewVolume {
    public:

        virtual ~IViewVolume() = default;

        /**
         * @brief Is the point outside the view volume?
         * @param point the point
         * @return true if the point is outside the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool IsCulled(const math::Vec3f& point) const = 0;

        /**
         * @brief Is the sphere outside of the view volume?
         * @param sphere the sphere
         * @return true if the sphere is outside the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool IsCulled(const math::Sphere& sphere) const = 0;

        /**
         * @brief Is the box outside the view volume?
         * @param box the box
         * @return true if the box is outside the view volume. Otherwise false.
         */
        [[nodiscard]] virtual bool IsCulled(const math::Box& box) const = 0;

    }; // class IViewVolume

} // namespace zero::render