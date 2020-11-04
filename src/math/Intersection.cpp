#include "math/Intersection.hpp"
#include "math/Box.hpp"
#include "math/Sphere.hpp"

namespace zero::math
{

bool Intersection::BoxSphereIntersect(const Box& box, const Sphere& sphere)
{
    math::Vec3f closest_point{math::Max(box.min_.x_, math::Min(sphere.center_.x_, box.max_.x_)),
                              math::Max(box.min_.y_, math::Min(sphere.center_.y_, box.max_.y_)),
                              math::Max(box.min_.z_, math::Min(sphere.center_.z_, box.max_.z_))};

    float squared_distance = (closest_point.x_ - sphere.center_.x_) * (closest_point.x_ - sphere.center_.x_) +
                             (closest_point.y_ - sphere.center_.y_) * (closest_point.y_ - sphere.center_.y_) +
                             (closest_point.z_ - sphere.center_.z_) * (closest_point.z_ - sphere.center_.z_);

    return squared_distance < (sphere.radius_ * sphere.radius_);
}

} // namespace zero::math