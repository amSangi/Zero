#include "component/Shapes.hpp"

namespace zero
{

/**
 * @brief The epsilon threshold to use when comparing floats
 */
constexpr float kShapesFloatDifferenceEpsilon = 0.1F;

Box::Box()
: width_(1U)
, height_(1U)
, depth_(1U)
{
}

bool Box::operator==(const Box& other) const
{
    return width_ == other.width_
        && height_ == other.height_
        && depth_ == other.depth_;
}

bool Box::operator!=(const Box& other) const
{
    return !(operator==(other));
}

Plane::Plane()
: width_(1U)
, height_(1U)
, u_axis_(math::Vec3f::Right())
, v_axis_(math::Vec3f::Forward())
{
}

bool Plane::operator==(const Plane& other) const
{
    return width_ == other.width_
       && height_ == other.height_
       && u_axis_ == other.u_axis_
       && v_axis_ == other.v_axis_;
}

bool Plane::operator!=(const Plane& other) const
{
    return !(operator==(other));
}

Cone::Cone()
: radius_(1.0F)
, height_(1U)
, radial_segments_(16U)
, height_segments_(1U)
, is_open_ended_(false)
{
}

bool Cone::operator==(const Cone& other) const
{
    return math::Equal(radius_, other.radius_, kShapesFloatDifferenceEpsilon)
        && height_ == other.height_
        && radial_segments_ == other.radial_segments_
        && height_segments_ == other.height_segments_
        && is_open_ended_ == other.is_open_ended_;
}

bool Cone::operator!=(const Cone& other) const
{
    return !(operator==(other));
}

Cylinder::Cylinder()
: top_radius_(1.0F)
, bottom_radius_(1.0F)
, height_(1U)
, radial_segments_(16U)
, height_segments_(1U)
, is_open_ended_(false)
{
}

bool Cylinder::operator==(const Cylinder& other) const
{
    return math::Equal(top_radius_, other.top_radius_, kShapesFloatDifferenceEpsilon)
        && math::Equal(bottom_radius_, other.bottom_radius_, kShapesFloatDifferenceEpsilon)
        && height_  == other.height_
        && radial_segments_ == other.radial_segments_
        && height_segments_ == other.height_segments_
        && is_open_ended_ == other.is_open_ended_;
}

bool Cylinder::operator!=(const Cylinder& other) const
{
    return !(operator==(other));
}

Sphere::Sphere()
: latitude_count_(16U)
, longitude_count_(16U)
{
}

bool Sphere::operator==(const Sphere& other) const
{
    return latitude_count_ == other.latitude_count_
        && longitude_count_ == other.longitude_count_;
}

bool Sphere::operator!=(const Sphere& other) const
{
    return !(operator==(other));
}

Torus::Torus()
: radius_(1.0F)
, tube_radius_(0.5F)
, radial_segments_(16U)
, tubular_segments_(16U)
{
}

bool Torus::operator==(const Torus& other) const
{
    return math::Equal(radius_, other.radius_, kShapesFloatDifferenceEpsilon)
        && math::Equal(tube_radius_, other.tube_radius_, kShapesFloatDifferenceEpsilon)
        && radial_segments_ == other.radial_segments_
        && tubular_segments_ == other.tubular_segments_;
}

bool Torus::operator!=(const Torus& other) const
{
    return !(operator==(other));
}

} // namespace zero