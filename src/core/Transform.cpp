#include "core/Transform.hpp"

using namespace zero;

Transform::Transform()
: HierarchyComponent()
, position_(0.0F)
, local_position_(0.0F)
, scale_(1.0F)
, local_scale_(1.0F)
, orientation_()
, local_orientation_()
{}

Transform::Transform(const math::Vec3f& position,
                     const math::Vec3f& scale,
                     const math::Quaternion& orientation)
: HierarchyComponent()
, position_(position)
, local_position_(0.0F)
, scale_(scale)
, local_scale_(1.0F)
, orientation_(orientation.UnitCopy())
, local_orientation_()
{}

Transform::Transform(Entity parent,
                     const Transform& parent_transform,
                     const math::Vec3f& local_position,
                     const math::Vec3f& local_scale,
                     const math::Quaternion& local_orientation)
: HierarchyComponent(parent)
, position_(parent_transform.position_ + local_position)
, local_position_(local_position)
, scale_(parent_transform.scale_ * local_scale)
, local_scale_(local_scale)
, orientation_((parent_transform.orientation_ * local_orientation).Unit())
, local_orientation_(local_orientation.UnitCopy())
{}

Transform Transform::FromMatrix4x4(const math::Matrix4x4& transformation) {
    Transform transform;
    return transform.Scale(transformation.GetScale())
                    .Rotate(transformation.GetRotation())
                    .Translate(transformation.GetTranslation());
}

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