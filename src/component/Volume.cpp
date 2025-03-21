#include "component/Volume.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Quaternion.hpp"

namespace zero
{

Volume::Volume()
: Component()
, bounding_volume_()
{
}

Volume::Volume(const math::Vec3f& position, float radius)
: Component()
, bounding_volume_(position, radius)
{
}

Volume::Volume(const math::Vec3f& min, const math::Vec3f& max)
: Component()
, bounding_volume_(min, max)
{
}

void Volume::Engulf(const Volume& other)
{
    bounding_volume_.Merge(other.bounding_volume_);
}

void Volume::Transform(const math::Matrix4x4& transformation)
{
    const math::Vec3f center = bounding_volume_.center_;

    // Apply transformation
    const math::Vec4f transformed_center = transformation * math::Vec4f(center.x_, center.y_, center.z_, 1.0F);

    // Get the largest scale component
    const float sx = math::Vec3f(transformation[0][0], transformation[1][0], transformation[2][0]).SquareMagnitude();
    const float sy = math::Vec3f(transformation[0][1], transformation[1][1], transformation[2][1]).SquareMagnitude();
    const float sz = math::Vec3f(transformation[0][2], transformation[1][2], transformation[2][2]).SquareMagnitude();
    const float scale = math::Sqrt(std::max({sx, sy, sz}));

    // Update volume
    bounding_volume_.center_ = transformed_center.XYZ();
    bounding_volume_.radius_ *= scale;
}

void Volume::Translate(const math::Vec3f& translation)
{
    bounding_volume_.center_ += translation;
}

void Volume::Scale(const math::Vec3f& scale)
{
    // Get the largest scale component
    const float largest_scale_factor = math::Max(scale.x_, math::Max(scale.y_, scale.z_));
    bounding_volume_.radius_ *= largest_scale_factor;
}

void Volume::Rotate(const math::Quaternion& rotation)
{
    bounding_volume_.center_ = rotation * bounding_volume_.center_;
}

} // namespace zero