#include "component/Transform.hpp"

namespace zero
{

Transform::Transform()
: HierarchyComponent()
, position_(0.0F)
, local_position_(0.0F)
, cached_translation_(0.0F)
, scale_(1.0F)
, local_scale_(1.0F)
, cached_scale_(1.0F)
, orientation_()
, local_orientation_()
, cached_rotation_()
, is_modified_(false)
{
}

Transform::Transform(const math::Vec3f& position,
                     const math::Vec3f& scale,
                     const math::Quaternion& orientation)
: HierarchyComponent()
, position_(position)
, local_position_(0.0F)
, cached_translation_(0.0F)
, scale_(scale)
, local_scale_(1.0F)
, cached_scale_(1.0F)
, orientation_(orientation.UnitCopy())
, local_orientation_()
, cached_rotation_()
, is_modified_(false)
{
}

Transform::Transform(Entity parent,
                     const Transform& parent_transform,
                     const math::Vec3f& local_position,
                     const math::Vec3f& local_scale,
                     const math::Quaternion& local_orientation)
: HierarchyComponent(parent)
, position_(parent_transform.position_ + local_position)
, local_position_(local_position)
, cached_translation_(0.0F)
, scale_(parent_transform.scale_ * local_scale)
, local_scale_(local_scale)
, cached_scale_(1.0F)
, orientation_((parent_transform.orientation_ * local_orientation).Unit())
, local_orientation_(local_orientation.UnitCopy())
, cached_rotation_()
, is_modified_(false)
{
}

Transform Transform::FromMatrix4x4(const math::Matrix4x4& transformation)
{
    Transform transform;
    return transform.Scale(transformation.GetScale())
                    .Rotate(transformation.GetRotation())
                    .Translate(transformation.GetTranslation());
}

bool Transform::operator==(const Transform& other) const
{
    return (position_ == other.position_)
        && (local_position_ == other.local_position_)
        && (scale_ == other.scale_)
        && (local_scale_ == other.scale_)
        && (orientation_ == other.orientation_)
        && (local_orientation_ == other.local_orientation_);
}

bool Transform::operator!=(const Transform& other) const
{
    return !(operator==(other));
}

math::Matrix4x4 Transform::GetWorldToLocalMatrix() const
{
    return GetLocalToWorldMatrix().Inverse();
}

math::Matrix4x4 Transform::GetLocalToWorldMatrix() const
{
    return math::Matrix4x4::Identity()
            .Scale(scale_)
            .Rotate(orientation_)
            .Translate(position_);
}

math::Matrix4x4 Transform::GetCachedLocalToWorldMatrix() const
{
    return math::Matrix4x4::Identity()
            .Scale(cached_scale_)
            .Rotate(cached_rotation_)
            .Translate(cached_translation_);
}

math::Matrix4x4 Transform::GetLocalToParentMatrix() const
{
    return math::Matrix4x4::Identity()
            .Scale(local_scale_)
            .Rotate(local_orientation_)
            .Translate(local_position_);
}

Transform& Transform::Translate(const math::Vec3f& translation)
{
    position_ += translation;
    cached_translation_ += translation;
    is_modified_ = true;
    return *this;
}

Transform& Transform::LocalTranslate(const Transform& parent, const math::Vec3f& translation)
{
    local_position_ += translation;

    // Update world position using parent transform
    math::Matrix4x4 parent_matrix = parent.GetLocalToWorldMatrix();
    math::Vec4f homogeneous_local_pos = math::Vec4f(local_position_.x_, local_position_.y_, local_position_.z_, 1.0F);
    position_ = (parent_matrix * homogeneous_local_pos).XYZ();

    return *this;
}

Transform& Transform::Rotate(const math::Quaternion& rotation)
{
    orientation_ *= rotation;
    orientation_.Unit();

    cached_rotation_ *= rotation;
    cached_rotation_.Unit();

    is_modified_ = true;
    return *this;
}

Transform& Transform::LocalRotate(const Transform& parent, const math::Quaternion& rotation)
{
    local_orientation_ *= rotation;
    local_orientation_.Unit();

    // Update world orientation using parent transform
    math::Matrix4x4 parent_matrix = parent.GetLocalToWorldMatrix();
    parent_matrix.Rotate(local_orientation_);
    orientation_ = parent_matrix.GetRotation();

    return *this;
}

Transform& Transform::Scale(const math::Vec3f& scale)
{
    scale_ *= scale;
    cached_scale_ *= scale;

    is_modified_ = true;
    return *this;
}

Transform& Transform::LocalScale(const Transform& parent, const math::Vec3f& scale)
{
    local_scale_ *= scale;

    // Update the world scale using the parent transform
    math::Matrix4x4 parent_matrix = parent.GetLocalToWorldMatrix();
    parent_matrix.Scale(local_scale_);
    scale_ = parent_matrix.GetScale();

    return *this;
}

void Transform::ClearCachedTransformation()
{
    cached_translation_ = math::Vec3f::Zero();
    cached_rotation_ = math::Quaternion::Identity();
    cached_scale_ = math::Vec3f::One();
    is_modified_ = false;
}

bool Transform::IsModified() const
{
    return is_modified_;
}

const math::Vec3f& Transform::GetPosition() const
{
    return position_;
}

const math::Vec3f& Transform::GetLocalPosition() const
{
    return local_position_;
}

const math::Vec3f& Transform::GetScale() const
{
    return scale_;
}

const math::Vec3f& Transform::GetLocalScale() const
{
    return local_scale_;
}

const math::Quaternion& Transform::GetOrientation() const
{
    return orientation_;
}

const math::Quaternion& Transform::GetLocalOrientation() const
{
    return local_orientation_;
}

void Transform::SetLocalScale(const math::Vec3f& scale)
{
    local_scale_ = scale;
}

void Transform::SetLocalOrientation(const math::Quaternion& orientation)
{
    local_orientation_ = orientation;
}

void Transform::SetLocalPosition(const math::Vec3f& position)
{
    local_position_ = position;
}

} // namespace zero