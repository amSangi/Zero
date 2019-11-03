#pragma once

#include "IViewVolume.hpp"
#include "math/Box.hpp"

namespace zero::render {

    /**
     * @brief Orthographic view volume represented using a Box
     */
    class OrthographicViewVolume : public IViewVolume {
    public:

        OrthographicViewVolume();

        ~OrthographicViewVolume();

        bool Contains(const math::Sphere& sphere) const override;
        bool Contains(const math::Box& box) const override;
        bool Contains(const math::Plane& plane) const override;
        bool Contains(const math::Vec3f& point) const override;

        bool Intersects(const math::Sphere& sphere) const override;
        bool Intersects(const math::Box& box) const override;
        bool Intersects(const math::Plane& plane) const override;

    private:
        math::Box view_box_;

    }; // class OrthographicViewVolume

} // namespace zero::render