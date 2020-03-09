#pragma once

#include "math/Vector3.hpp"

namespace zero::render {

    struct Plane {
        math::Vec3f center_;
        uint32 width_;
        uint32 height_;
    }; // struct Plane

    struct Cone {
        // TODO: Add data types that represent a cone
    }; // struct Cone

    struct Cylinder {
        // TODO: Add data types that represent a cylinder
    }; // struct Cylinder

    struct Torus {
        math::Vec3f center_;
        float inner_radius_;
        float outer_radius_;
    }; // struct Torus

} // namespace zero::render
