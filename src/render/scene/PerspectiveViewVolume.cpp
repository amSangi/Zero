#include "math/Box.hpp"
#include "render/scene/PerspectiveViewVolume.hpp"

namespace zero::render
{

PerspectiveViewVolume::PerspectiveViewVolume(const math::Plane& left,
                                             const math::Plane& right,
                                             const math::Plane& bottom,
                                             const math::Plane& top,
                                             const math::Plane& near,
                                             const math::Plane& far)
: planes_{left, right, bottom, top, near, far}
, padding_(math::kEpsilon)
{
}

void PerspectiveViewVolume::SetPadding(float padding)
{
    padding_ = padding;
}

bool PerspectiveViewVolume::IsCulled(const math::Vec3f& point) const
{
    // Is the point on the wrong side of a plane?
    for (const auto& plane : planes_)
    {
        auto dist = plane.Distance(point);
        if (dist < -padding_)
        {
            return true;
        }
    }
    return false;
}

bool PerspectiveViewVolume::IsCulled(const math::Sphere& sphere) const
{
    // If the sphere is on the wrong side of a plane, is the distance greater than the radius?
    for (const auto& plane : planes_)
    {
        auto distance = plane.Distance(sphere.center_);
        if (distance < -(sphere.radius_ + padding_))
        {
            return true;
        }
    }
    return false;
}

bool PerspectiveViewVolume::IsCulled(const math::Box& box) const
{
    // Are the min/max points on the wrong side of a plane?
    for (const auto& plane : planes_)
    {
        if (plane.Distance(box.min_) < -padding_
         && plane.Distance(box.max_) < -padding_)
        {
            return true;
        }
    }
    return false;
}

const zero::math::Plane& PerspectiveViewVolume::GetLeftPlane() const
{
    return planes_[0];
}

const zero::math::Plane& PerspectiveViewVolume::GetRightPlane() const
{
    return planes_[1];
}

const zero::math::Plane& PerspectiveViewVolume::GetBottomPlane() const
{
    return planes_[2];
}

const zero::math::Plane& PerspectiveViewVolume::GetTopPlane() const
{
    return planes_[3];
}

const zero::math::Plane& PerspectiveViewVolume::GetNearPlane() const
{
    return planes_[4];
}

const zero::math::Plane& PerspectiveViewVolume::GetFarPlane() const
{
    return planes_[5];
}

} // namespace zero::render