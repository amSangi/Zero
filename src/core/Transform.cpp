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

void Transform::LocalTranslate(const Transform& parent, const math::Vec3f& translation) {
    local_position_ += translation;

    // Update world position using parent transform
    auto parent_matrix = parent.GetLocalToWorldMatrix();
    math::Vec4f homogeneous_local_pos = math::Vec4f(local_position_.x_, local_position_.y_, local_position_.z_, 1.0f);
    position_ = (parent_matrix * homogeneous_local_pos).XYZ();
}

void Transform::Rotate(const math::Quaternion& rotation) {
    orientation_ *= rotation;
    orientation_.Unit();
}

void Transform::LocalRotate(const Transform& parent, const math::Quaternion& rotation) {
    local_orientation_ *= rotation;
    local_orientation_.Unit();

    // Update world orientation using parent transform
    auto parent_matrix = parent.GetLocalToWorldMatrix();
    parent_matrix.Rotate(local_orientation_);
    orientation_ = parent_matrix.GetRotation();
}

void Transform::Scale(const math::Vec3f& scale) {
    scale_ *= scale;
}

void Transform::LocalScale(const Transform& parent, const math::Vec3f& scale) {
    local_scale_ *= scale;

    // Update the world scale using the parent transform
    auto parent_matrix = parent.GetLocalToWorldMatrix();
    parent_matrix.Scale(local_scale_);
    scale_ = parent_matrix.GetScale();
}