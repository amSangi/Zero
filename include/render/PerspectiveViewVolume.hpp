#pragma once

#include "IViewVolume.hpp"
#include <array>

namespace zero::render {

    /**
     * @brief Perspective view volume represented by 6 planes
     */
    class PerspectiveViewVolume : public IViewVolume {
    public:

        PerspectiveViewVolume();

        ~PerspectiveViewVolume();

        bool Contains(const math::Sphere& sphere) const override;
        bool Contains(const math::Box& box) const override;
        bool Contains(const math::Plane& plane) const override;
        bool Contains(const math::Vec3f& point) const override;

        bool Intersects(const math::Sphere& sphere) const override;
        bool Intersects(const math::Box& box) const override;
        bool Intersects(const math::Plane& plane) const override;

    private:
        math::Plane[6] planes_;

    }; // class PerspectiveViewVolume

} // namespace zero::render