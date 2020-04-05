#include "render/Shapes.hpp"

using namespace zero::render;

Box::Box()
: width_(1U)
, height_(1U)
, depth_(1U)
{}

Plane::Plane()
: width_(1.0F)
, height_(1.0F)
, u_axis_(math::Vec3f::Right())
, v_axis_(math::Vec3f::Forward())
{}

Cone::Cone()
: radius_(1.0F)
, height_(1)
, radial_segments_(16)
, height_segments_(1)
, is_open_ended_(false)
{}

Cylinder::Cylinder()
: top_radius_(1.0F)
, bottom_radius_(1.0F)
, height_(1)
, radial_segments_(16)
, height_segments_(1)
, is_open_ended_(false)
{}

Sphere::Sphere()
: latitude_count_(16)
, longitude_count_(16)
{}

Torus::Torus()
: radius_(1.0F)
, tube_radius_(0.5F)
, radial_segments_(16)
, tubular_segments_(16)
{}

