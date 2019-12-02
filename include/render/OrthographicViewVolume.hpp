#pragma once

#include "IViewVolume.hpp"
#include "math/Box.hpp"

namespace zero::render {

    /**
     * @brief Orthographic view volume represented using a Box
     */
    class OrthographicViewVolume : public IViewVolume {
    public:

        OrthographicViewVolume(const math::Vec3f& min, const math::Vec3f& max);
        ~OrthographicViewVolume() override = default;

        [[nodiscard]] bool IsCulled(const math::Vec3f& point) const override;
        [[nodiscard]] bool IsCulled(const math::Sphere& sphere) const override;
        [[nodiscard]] bool IsCulled(const math::Box& box) const override;

        [[nodiscard]] const math::Box& GetViewBox() const;

    private:
        math::Box view_box_;

    }; // class OrthographicViewVolume

} // namespace zero::render