#pragma once

#include "math/Vector3.hpp"

namespace zero::render {

    // TODO: Add default constructors with default values
    struct Box {
        Box();
        uint32 width_;
        uint32 height_;
        uint32 depth_;
    };

    struct Plane {
        Plane();
        uint32 width_;
        uint32 height_;
        math::Vec3f u_axis_;
        math::Vec3f v_axis_;
    }; // struct Plane

    struct Cone {
        // TODO: Add data types that represent a cone
    }; // struct Cone

    struct Cylinder {
        // TODO: Add data types that represent a cylinder
    }; // struct Cylinder

    struct Sphere {
        Sphere();
        uint32 latitude_count_;
        uint32 longitude_count_;
    }; // struct Sphere

    struct Torus {
        Torus();
        float radius_;
        float tube_radius_;
        uint32 radial_segments_;
        uint32 tubular_segments_;
    }; // struct Torus

} // namespace zero::render
