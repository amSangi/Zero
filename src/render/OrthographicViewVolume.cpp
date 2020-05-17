#include "render/OrthographicViewVolume.hpp"
#include "math/Intersection.hpp"

namespace zero::render
{

OrthographicViewVolume::OrthographicViewVolume(const math::Vec3f& min, const math::Vec3f& max)
: view_box_(min, max)
{
}

bool OrthographicViewVolume::IsCulled(const math::Vec3f& point) const
{
    return !view_box_.Contains(point);
}

bool OrthographicViewVolume::IsCulled(const math::Sphere& sphere) const
{
    return !math::Intersection::BoxSphereIntersect(view_box_, sphere);
}

bool OrthographicViewVolume::IsCulled(const math::Box& box) const
{
    return !(view_box_.Intersects(box) || view_box_.Contains(box));
}

const zero::math::Box& OrthographicViewVolume::GetViewBox() const
{
    return view_box_;
}

} // namespace zero::render