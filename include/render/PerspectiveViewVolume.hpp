#pragma once

#include "IViewVolume.hpp"
#include "math/Plane.hpp"

namespace zero::render {

    /**
     * @brief Perspective view volume represented by 6 planes
     */
    class PerspectiveViewVolume : public IViewVolume {
    public:

        PerspectiveViewVolume(const math::Plane& left,
                              const math::Plane& right,
                              const math::Plane& bottom,
                              const math::Plane& top,
                              const math::Plane& near,
                              const math::Plane& far);
        ~PerspectiveViewVolume() override = default;

        [[nodiscard]] bool IsCulled(const math::Vec3f& point) const override;
        [[nodiscard]] bool IsCulled(const math::Sphere& sphere) const override;
        [[nodiscard]] bool IsCulled(const math::Box& box) const override;

        [[nodiscard]] const math::Plane& GetLeftPlane() const;
        [[nodiscard]] const math::Plane& GetRightPlane() const;
        [[nodiscard]] const math::Plane& GetBottomPlane() const;
        [[nodiscard]] const math::Plane& GetTopPlane() const;
        [[nodiscard]] const math::Plane& GetNearPlane() const;
        [[nodiscard]] const math::Plane& GetFarPlane() const;

    private:
        math::Plane planes_[6];

    }; // class PerspectiveViewVolume

} // namespace zero::render