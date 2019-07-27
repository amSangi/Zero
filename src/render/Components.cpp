#include "render/Components.hpp"

using namespace zero::render;

void Volume::Engulf(const Volume& other) {
    bounding_volume_.Merge(other.bounding_volume_);
}

void Volume::Transform(const zero::math::Matrix4x4& transformation) {
    math::Vec3f center = bounding_volume_.center_;

    // Apply transformation
    math::Vec4f transformed_center = transformation * math::Vec4f(center.x_, center.y_, center.z_, 1.0f);

    // Get the largest scale component
    const float sx = math::Vec3f(transformation[0][0], transformation[1][0], transformation[2][0]).SquareMagnitude();
    const float sy = math::Vec3f(transformation[0][1], transformation[1][1], transformation[2][1]).SquareMagnitude();
    const float sz = math::Vec3f(transformation[0][2], transformation[1][2], transformation[2][2]).SquareMagnitude();
    const float scale = zero::math::Sqrt(std::max({sx, sy, sz}));

    // Update volume
    bounding_volume_.center_ = transformed_center.XYZ();
    bounding_volume_.radius_ *= scale;
}