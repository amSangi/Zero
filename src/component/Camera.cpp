#include "component/Camera.hpp"
#include "math/Matrix4x4.hpp"

namespace zero
{

constexpr float kDefaultNearClip = 0.1F;
constexpr float kDefaultFarClip = 500.0F;
constexpr float kDefaultHorizontalFOVDegrees = 65.0F;
constexpr zero::uint32 kDefaultViewportWidth = 800;
constexpr zero::uint32 kDefaultViewportHeight = 600;

Camera::Viewport::Viewport()
: x_(0)
, y_(0)
, width_(kDefaultViewportWidth)
, height_(kDefaultViewportHeight)
{
}

float Camera::Viewport::GetAspectRatio() const
{
    return (static_cast<float>(width_) / static_cast<float>(height_));
}

Camera::Camera()
: projection_(Camera::ProjectionType::PERSPECTIVE)
, near_clip_(kDefaultNearClip)
, far_clip_(kDefaultFarClip)
, horizontal_field_of_view_(kDefaultHorizontalFOVDegrees)
, position_(math::Vec3f::Zero())
, orientation_(math::Quaternion::Identity())
, viewport_()
, render_bounding_volumes_(false)
{
}

Camera::Camera(Camera::ProjectionType projection_type)
: projection_(projection_type)
, near_clip_(kDefaultNearClip)
, far_clip_(kDefaultFarClip)
, horizontal_field_of_view_(kDefaultHorizontalFOVDegrees)
, position_(math::Vec3f::Zero())
, orientation_(math::Quaternion::Identity())
, viewport_()
, render_bounding_volumes_(false)
{
}

Camera::ProjectionType Camera::GetProjectionType() const
{
    return projection_;
}

void Camera::Translate(const math::Vec3f& translation)
{
    position_ += translation;
}

void Camera::TranslateRelative(const math::Vec3f& translation)
{
    Translate(orientation_ * translation);
}

void Camera::Rotate(const math::Quaternion& rotation)
{
    orientation_ = rotation * orientation_;
    orientation_.Unit();
}

void Camera::PitchRelative(const math::Radian& angle)
{
    auto rotation = math::Quaternion::FromAngleAxis(GetRightVector(), angle);
    Rotate(rotation);
}

void Camera::YawRelative(const math::Radian& angle)
{
    auto rotation = math::Quaternion::FromAngleAxis(GetUpVector(), angle);
    Rotate(rotation);
}

void Camera::RollRelative(const math::Radian& angle)
{
    auto rotation = math::Quaternion::FromAngleAxis(GetViewDirection(), angle);
    Rotate(rotation);
}

void Camera::LookAt(const math::Vec3f& target)
{
    math::Quaternion rotation = math::Quaternion::FromToRotation(GetViewDirection(), target);
    Rotate(rotation);
}

void Camera::GetNearClipCoordinates(math::Vec3f& bottom_left, math::Vec3f& top_right) const
{
    // half_near_height = tan(vertical_fov / 2) * near_clip_
    float half_near_height = math::Tan(GetVerticalFieldOfView().rad_ / 2.0F) * near_clip_;
    float half_near_width = half_near_height * viewport_.GetAspectRatio();

    math::Vec3f up = GetUpVector();
    math::Vec3f right_vector = GetRightVector();
    math::Vec3f view_direction = GetViewDirection();
    math::Vec3f center = position_ + (view_direction * near_clip_);

    math::Vec3f vertical = (up * half_near_height);
    math::Vec3f horizontal = (right_vector * half_near_width);
    bottom_left = center - vertical - horizontal;
    top_right = center + vertical + horizontal;
}

void Camera::GetFarClipCoordinates(math::Vec3f& bottom_left, math::Vec3f& top_right) const
{

    // half_far_height = 2 * tan(vertical_fov / 2) * far_clip_
    float half_far_height = math::Tan(GetVerticalFieldOfView().rad_ / 2.0F) * far_clip_;
    float half_far_width = half_far_height * viewport_.GetAspectRatio();

    math::Vec3f up = GetUpVector();
    math::Vec3f right_vector = GetRightVector();
    math::Vec3f view_direction = GetViewDirection();
    math::Vec3f center = position_ + (view_direction * far_clip_);

    math::Vec3f vertical = (up * half_far_height);
    math::Vec3f horizontal = (right_vector * half_far_width);
    bottom_left = center - vertical - horizontal;
    top_right = center + vertical + horizontal;
}

zero::math::Vec3f Camera::GetUpVector() const
{
    return orientation_ * math::Vec3f::Up();
}

zero::math::Vec3f Camera::GetRightVector() const
{
    return orientation_ * math::Vec3f::Right();
}

zero::math::Vec3f Camera::GetViewDirection() const
{
    return orientation_ * math::Vec3f::Back();
}

zero::math::Radian Camera::GetVerticalFieldOfView() const
{
    // V = 2 * arctan( tan (H/2) * h/w )
    // H is the horizontal field of view in radians
    // h is the height of the viewport
    // w is the width of the viewport
    float height_over_width = static_cast<float>(viewport_.height_) / static_cast<float>(viewport_.width_);
    float tan_half_h_fov = math::Tan(horizontal_field_of_view_.ToRadian().rad_ * 0.5F);
    float vertical_fov_in_radians = 2.0F * math::Atan(tan_half_h_fov * height_over_width);
    return math::Radian(vertical_fov_in_radians);
}

zero::math::Matrix4x4 Camera::GetProjectionMatrix() const
{
    switch (projection_)
    {
        case ProjectionType::ORTHOGRAPHIC:
        {
            float top = near_clip_ * math::Tan(GetVerticalFieldOfView().rad_ * 0.5F);
            float bottom = -top;
            float right = top * viewport_.GetAspectRatio();
            float left = -right;
            return math::Matrix4x4::Orthographic(left, right, bottom, top, near_clip_, far_clip_);
        }
        default:
            return math::Matrix4x4::Perspective(GetVerticalFieldOfView(),
                                                viewport_.GetAspectRatio(),
                                                near_clip_,
                                                far_clip_);
    }
}

zero::math::Matrix4x4 Camera::GetViewMatrix() const
{
    return GetCameraToWorldMatrix().Inverse();
}

zero::math::Matrix4x4 Camera::GetCameraToWorldMatrix() const
{
    return math::Matrix4x4::Identity().Translate(position_).Rotate(orientation_);
}

} // namespace zero