#include "core/Transform.hpp"

using namespace zero;

math::Matrix4x4 Transform::GetWorldToLocalMatrix() const {
    return GetLocalToWorldMatrix().Inverse();
}

math::Matrix4x4 Transform::GetLocalToWorldMatrix() const {
    return math::Matrix4x4::Identity()
            .Scale(scale_)
            .Rotate(orientation_)
            .Translate(position_);
}

void Transform::Translate(const math::Vec3f& translation) {
    position_ += translation;
}

void Transform::Rotate(const math::Vec3f& euler) {
    Rotate(math::Radian(euler.x_), math::Radian(euler.y_), math::Radian(euler.z_));
}

void Transform::Rotate(const math::Quaternion& rotation) {
    orientation_ *= rotation;
}

void Transform::Rotate(math::Radian x, math::Radian y, math::Radian z) {
    Rotate(math::Quaternion::FromEuler(x, y, z));
}

void Transform::Rotate(const math::Vec3f& axis, math::Radian angle) {
    Rotate(math::Quaternion::FromAngleAxis(axis, angle));
}