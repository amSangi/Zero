#include "render/Components.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Vector3.hpp"

using namespace zero::render;

constexpr float kDefaultNearClip = 20.0F;
constexpr float kDefaultFarClip = 500.0F;
constexpr float kDefaultHorizontalFOVDegrees = 65.0F;
constexpr zero::uint32 kDefaultViewportWidth = 800;
constexpr zero::uint32 kDefaultViewportHeight = 600;

////////////////////////////////////////////////////////////
///// Volume
////////////////////////////////////////////////////////////

void Volume::Engulf(const Volume& other) {
    bounding_volume_.Merge(other.bounding_volume_);
}

void Volume::Transform(const zero::math::Matrix4x4& transformation) {
    math::Vec3f center = bounding_volume_.center_;

    // Apply transformation
    math::Vec4f transformed_center = transformation * math::Vec4f(center.x_, center.y_, center.z_, 1.0F);

    // Get the largest scale component
    const float sx = math::Vec3f(transformation[0][0], transformation[1][0], transformation[2][0]).SquareMagnitude();
    const float sy = math::Vec3f(transformation[0][1], transformation[1][1], transformation[2][1]).SquareMagnitude();
    const float sz = math::Vec3f(transformation[0][2], transformation[1][2], transformation[2][2]).SquareMagnitude();
    const float scale = zero::math::Sqrt(std::max({sx, sy, sz}));

    // Update volume
    bounding_volume_.center_ = transformed_center.XYZ();
    bounding_volume_.radius_ *= scale;
}

////////////////////////////////////////////////////////////
///// Camera
////////////////////////////////////////////////////////////

Camera::Viewport::Viewport()
: x_(0)
, y_(0)
, width_(kDefaultViewportWidth)
, height_(kDefaultViewportHeight)
{}

float Camera::Viewport::GetAspectRatio() const {
    return (static_cast<float>(width_) / static_cast<float>(height_));
}

Camera::Camera()
: projection_(Camera::ProjectionType::PERSPECTIVE)
, near_clip_(kDefaultNearClip)
, far_clip_(kDefaultFarClip)
, horizontal_field_of_view_(kDefaultHorizontalFOVDegrees)
, up_(math::Vec3f::Up())
, position_(math::Vec3f::Zero())
, target_(math::Vec3f::Back())
, orientation_()
, viewport_()
{
}

Camera::Camera(Camera::ProjectionType projection_type)
: projection_(projection_type)
, near_clip_(kDefaultNearClip)
, far_clip_(kDefaultFarClip)
, horizontal_field_of_view_(kDefaultHorizontalFOVDegrees)
, up_(math::Vec3f::Up())
, position_(math::Vec3f::Zero())
, target_(math::Vec3f::Back())
, orientation_()
, viewport_()
{
}

Camera::ProjectionType Camera::GetProjectionType() const {
    return projection_;
}

void Camera::LookAt(const math::Vec3f& target) {
    math::Vec3f direction = target - position_;
    math::Quaternion rotation = math::Quaternion::LookRotation(direction, up_);
    orientation_ = rotation * orientation_;
    orientation_.Unit();
    target_ = target;
}

void Camera::GetNearClipCoordinates(math::Vec3f& bottom_left,
                                    math::Vec3f& top_right) const {
    // half_near_height = tan(vertical_fov / 2) * near_clip_
    float half_near_height = math::Tan(GetVerticalFieldOfView().rad_ / 2.0F) * near_clip_;
    float half_near_width = half_near_height * viewport_.GetAspectRatio();

    math::Vec3f view_direction = math::Vec3f::Normalize(target_ - position_);
    math::Vec3f center = position_ + (view_direction * near_clip_);
    math::Vec3f right_vector = math::Vec3f::Normalize(math::Vec3f::Cross(view_direction, up_));

    math::Vec3f vertical = (up_ * half_near_height);
    math::Vec3f horizontal = (right_vector * half_near_width);
    bottom_left = center - vertical - horizontal;
    top_right = center + vertical + horizontal;
}

void Camera::GetFarClipCoordinates(math::Vec3f& bottom_left,
                                   math::Vec3f& top_right) const {

    // half_far_height = 2 * tan(vertical_fov / 2) * far_clip_
    float half_far_height = math::Tan(GetVerticalFieldOfView().rad_ / 2.0F) * far_clip_;
    float half_far_width = half_far_height * viewport_.GetAspectRatio();

    math::Vec3f view_direction = math::Vec3f::Normalize(target_ - position_);
    math::Vec3f center = position_ + (view_direction * far_clip_);
    math::Vec3f right_vector = math::Vec3f::Normalize(math::Vec3f::Cross(view_direction, up_));

    math::Vec3f vertical = (up_ * half_far_height);
    math::Vec3f horizontal = (right_vector * half_far_width);
    bottom_left = center - vertical - horizontal;
    top_right = center + vertical + horizontal;
}

zero::math::Radian Camera::GetVerticalFieldOfView() const {
    // V = 2 * arctan( tan (H/2) * h/w )
    // H is the horizontal field of view in radians
    // h is the height of the viewport
    // w is the width of the viewport
    float height_over_width = static_cast<float>(viewport_.height_) / static_cast<float>(viewport_.width_);
    float tan_half_h_fov = math::Tan(horizontal_field_of_view_.ToRadian().rad_ * 0.5F);
    float vertical_fov_in_radians = 2.0F * math::Atan(tan_half_h_fov * height_over_width);
    return math::Radian(vertical_fov_in_radians);
}

zero::math::Matrix4x4 Camera::GetProjectionMatrix() const {
    switch (projection_)
    {
        case ProjectionType::ORTHOGRAPHIC: {
            float top = near_clip_ * math::Tan(GetVerticalFieldOfView().rad_ * 0.5F);
            float bottom = -top;
            float right = top * viewport_.GetAspectRatio();
            float left = -right;
            return Orthographic(left, right, bottom, top, near_clip_, far_clip_);
        }
        default:
            return Perspective(GetVerticalFieldOfView(),
                               viewport_.GetAspectRatio(),
                               near_clip_,
                               far_clip_);
    }
}

zero::math::Matrix4x4 Camera::GetViewMatrix() const {
    return GetCameraToWorldMatrix().Inverse();
}

zero::math::Matrix4x4 Camera::GetCameraToWorldMatrix() const {
    math::Vec3f k = math::Vec3f::Normalize(position_ - target_);
    math::Vec3f i = math::Vec3f::Normalize(math::Vec3f::Cross(up_, k));
    math::Vec3f j = math::Vec3f::Cross(k, i);
    return math::Matrix4x4(i.x_, j.x_, k.x_, position_.x_,
                           i.y_, j.y_, k.y_, position_.y_,
                           i.z_, j.z_, k.z_, position_.z_,
                           0.0F, 0.0F, 0.0F, 1.0F);
}

zero::math::Matrix4x4 Camera::Perspective(math::Radian vertical_fov,
                                          float aspect_ratio,
                                          float near,
                                          float far) {

    math::Matrix4x4 perspective_matrix(0.0F);

    float tan_half_vertical_fov = math::Tan(vertical_fov.rad_ / 2.0F);
    perspective_matrix[0][0] = 1.0F / (aspect_ratio * tan_half_vertical_fov);
    perspective_matrix[1][1] = 1.0F / (tan_half_vertical_fov);
    perspective_matrix[2][2] = -(far + near) / (far - near);
    perspective_matrix[2][3] = -(2.0F * far * near) / (far - near);
    perspective_matrix[3][2] = -1.0F;

    return perspective_matrix;
}

zero::math::Matrix4x4 Camera::Orthographic(float left,
                                           float right,
                                           float bottom,
                                           float top,
                                           float near,
                                           float far) {

    math::Matrix4x4 orthographic_matrix(0.0F);

    orthographic_matrix[0][0] = 2.0F / (right - left);
    orthographic_matrix[0][3] = -(right + left) / (right - left);
    orthographic_matrix[1][1] = 2.0F / (top - bottom);
    orthographic_matrix[1][3] = -(top + bottom) / (top - bottom);
    orthographic_matrix[2][2] = -2.0F / (far - near);
    orthographic_matrix[2][3] = -(far + near) / (far - near);
    orthographic_matrix[3][3] = 1.0F;

    return orthographic_matrix;
}