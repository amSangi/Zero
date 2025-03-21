#include "component/Transform.hpp"

namespace zero
{

Transform::Transform()
: Component()
, position_(0.0F)
, local_position_(0.0F)
, scale_(1.0F)
, local_scale_(1.0F)
, orientation_(math::Quaternion::Identity())
, local_orientation_(math::Quaternion::Identity())
, parent_(zero::NullEntity)
, children_()
{
}

Transform::Transform(const math::Vec3f& position,
                     const math::Vec3f& scale,
                     const math::Quaternion& orientation)
: Component()
, position_(position)
, local_position_(0.0F)
, scale_(scale)
, local_scale_(1.0F)
, orientation_(orientation.UnitCopy())
, local_orientation_(math::Quaternion::Identity())
, parent_(zero::NullEntity)
, children_()
{
}

Transform::Transform(const Entity parent,
                     const Transform& parent_transform,
                     const math::Vec3f& local_position,
                     const math::Vec3f& local_scale,
                     const math::Quaternion& local_orientation)
: Component()
, position_(parent_transform.position_ + local_position)
, local_position_(local_position)
, scale_(parent_transform.scale_ * local_scale)
, local_scale_(local_scale)
, orientation_((parent_transform.orientation_ * local_orientation).Unit())
, local_orientation_(local_orientation.UnitCopy())
, parent_(parent)
, children_()
{
}

Transform Transform::FromMatrix4x4(const math::Matrix4x4& transformation)
{
	return Transform{transformation.GetTranslation(),
					 transformation.GetScale(),
					 transformation.GetRotation()};
}

bool Transform::operator==(const Transform& other) const
{
    return (position_ == other.position_)
        && (local_position_ == other.local_position_)
        && (scale_ == other.scale_)
        && (local_scale_ == other.local_scale_)
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

math::Matrix4x4 Transform::GetLocalToParentMatrix() const
{
    return math::Matrix4x4::Identity()
            .Scale(local_scale_)
            .Rotate(local_orientation_)
            .Translate(local_position_);
}

const math::Vec3f& Transform::GetPosition() const
{
    return position_;
}

const math::Vec3f& Transform::GetScale() const
{
    return scale_;
}

const math::Quaternion& Transform::GetOrientation() const
{
    return orientation_;
}

const Entity& Transform::GetParent() const
{
    return parent_;
}

const std::vector<Entity>& Transform::GetChildren() const
{
    return children_;
}

} // namespace zero