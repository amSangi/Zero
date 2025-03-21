#pragma once

#include "math/Vector3.hpp"
#include "math/Sphere.hpp"
#include "math/Box.hpp"

namespace zero::render
{

    /**
     * @brief A camera view volume interface used to cull volumes
     */
    class IViewVolume
    {
    public:

        virtual ~IViewVolume() = default;

        /**
         * @brief Set the padding of the volume (margin of error)
         *
         * A point, sphere, or box is considered not culled if it is at most padding units outside
         * of the volume. Padding increases the size of the volume to allow for a customizable margin of error.
         *
         * @param padding the padding
         */
        virtual void SetPadding(float padding) = 0;

        /**
         * @brief Is the point outside the view volume?
         * @param point the point
         * @return true if the point is outside the view volume. Otherwise, false.
         */
        [[nodiscard]] virtual bool IsCulled(const math::Vec3f& point) const = 0;

        /**
         * @brief Is the sphere outside the view volume?
         * @param sphere the sphere
         * @return true if the sphere is outside the view volume. Otherwise, false.
         */
        [[nodiscard]] virtual bool IsCulled(const math::Sphere& sphere) const = 0;

        /**
         * @brief Is the box outside the view volume?
         * @param box the box
         * @return true if the box is outside the view volume. Otherwise, false.
         */
        [[nodiscard]] virtual bool IsCulled(const math::Box& box) const = 0;

    }; // class IViewVolume

} // namespace zero::render