#include "core/Transform.hpp"

using namespace zero;

bool Transform::operator==(const Transform& other) const {
    return (position_ == other.position_)
        && (local_position_ == other.local_position_)
        && (scale_ == other.scale_)
        && (local_scale_ == other.scale_)
        && (orientation_ == other.orientation_)
        && (local_orientation_ == other.local_orientation_);
}

bool Transform::operator!=(const Transform& other) const {
    return !(operator==(other));
}

math::Matrix4x4 Transform::GetWorldToLocalMatrix() const {
    return GetLocalToWorldMatrix().Inverse();
}

math::Matrix4x4 Transform::GetLocalToWorldMatrix() const {
    return math::Matrix4x4::Identity()
            .Scale(scale_)
            .Rotate(orientation_)
            .Translate(position_);
}

math::Matrix4x4 Transform::GetLocalToParentMatrix() const {
    return math::Matrix4x4::Identity()
            .Scale(local_scale_)
            .Rotate(local_orientation_)
            .Translate(local_position_);
}

Transform& Transform::Translate(const math::Vec3f& translation) {
    position_ += translation;

    return *this;
}

Transform& Transform::LocalTranslate(const Transform& parent, const math::Vec3f& translation) {
    local_position_ += translation;

    // Update world position using parent transform
    auto parent_matrix = parent.GetLocalToWorldMatrix();
    math::Vec4f homogeneous_local_pos = math::Vec4f(local_position_.x_, local_position_.y_, local_position_.z_, 1.0F);
    position_ = (parent_matrix * homogeneous_local_pos).XYZ();

    return *this;
}

Transform& Transform::Rotate(const math::Quaternion& rotation) {
    orientation_ *= rotation;
    orientation_.Unit();

    return *this;
}

Transform& Transform::LocalRotate(const Transform& parent, const math::Quaternion& rotation) {
    local_orientation_ *= rotation;
    local_orientation_.Unit();

    // Update world orientation using parent transform
    auto parent_matrix = parent.GetLocalToWorldMatrix();
    parent_matrix.Rotate(local_orientation_);
    orientation_ = parent_matrix.GetRotation();

    return *this;
}

Transform& Transform::Scale(const math::Vec3f& scale) {
    scale_ *= scale;

    return *this;
}

Transform& Transform::LocalScale(const Transform& parent, const math::Vec3f& scale) {
    local_scale_ *= scale;

    // Update the world scale using the parent transform
    auto parent_matrix = parent.GetLocalToWorldMatrix();
    parent_matrix.Scale(local_scale_);
    scale_ = parent_matrix.GetScale();

    return *this;
}