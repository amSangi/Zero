#pragma once

#include "core/Component.hpp"
#include "math/Angle.hpp"
#include "math/Vector3.hpp"

namespace zero::render {

    /**
     * @brief Directional light component
     */
    struct DirectionalLight : public Component {
        math::Vec3f color_;
        float intensity_;
        // TODO: Fully implement Directional Light data structure
    }; // struct DirectionalLight

    /**
     * @brief Point light component
     */
    struct PointLight : public Component {
        math::Vec3f color_;
        float intensity_;
        float radius_;
    }; // struct PointLight

    /**
     * @brief Spot light component
     */
    struct SpotLight : public Component {
        math::Vec3f color_;
        float intensity_;
        math::Degree inner_cone_angle_;
        math::Degree outer_cone_angle_;
    }; // struct SpotLight

} // namespace zero::render
