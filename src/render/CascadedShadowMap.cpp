#include "render/CascadedShadowMap.hpp"

namespace zero::render
{

CascadedShadowMap::CascadedShadowMap(uint32 cascade_count)
: cascade_count_(cascade_count)
, light_view_matrices_(cascade_count_, math::Matrix4x4::Identity())
, projection_matrices_(cascade_count_, math::Matrix4x4::Identity())
, texture_matrices_(cascade_count_, math::Matrix4x4::Identity())
, world_bounding_boxes_(cascade_count_, math::Box::Unit())
, cascade_splits_(cascade_count_, 0.0F)
, view_far_bounds_(cascade_count_, 0.0F)
, maximum_far_clip_(std::numeric_limits<float>::max())
{
}

void CascadedShadowMap::SetMaxShadowFarClip(float maximum_far_clip)
{
    maximum_far_clip_ = maximum_far_clip;
}

void CascadedShadowMap::Update(const Camera& camera, const DirectionalLight& directional_light)
{
    if (cached_camera_.position_ == camera.position_
    && cached_camera_.orientation_ == camera.orientation_
    && math::Equal(cached_camera_.near_clip_, camera.near_clip_)
    && math::Equal(cached_camera_.far_clip_, camera.far_clip_))
    {
        return;
    }
    cached_camera_ = camera;
    UpdateCascadeSplits(camera);
    UpdateMatrices(camera, directional_light);
    UpdateTextureMatrices();
}

void CascadedShadowMap::UpdateCascadeSplits(const Camera& camera)
{
    // Logarithmic contribution to final distance
    constexpr float kLambda = 0.95F;

    // Cap the maximum range of the shadows
    float far_clip = math::Min(camera.far_clip_, maximum_far_clip_);

    // Practical Split Scheme:
    // https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-10-parallel-split-shadow-maps-programmable-gpus
    float z_range = far_clip - camera.near_clip_;
    float ratio = far_clip / camera.near_clip_;
    for (uint32 i = 0; i < cascade_count_; ++i)
    {
        float cascade_factor = static_cast<float>(i + 1) / static_cast<float>(cascade_count_);
        float logarithmic_distance = camera.near_clip_ * math::Pow(ratio, cascade_factor);
        float uniform_distance = camera.near_clip_ + (z_range * cascade_factor);
        float distance = kLambda * (logarithmic_distance - uniform_distance) + uniform_distance;

        // Compute distance in the range [0, 1] where 1 is the furthest point a shadow can be
        cascade_splits_[i] = (distance - camera.near_clip_) / z_range;

        // Compute distance in light view coordinates (Looking down negative z axis)
        // The more negative the further away from the light
        view_far_bounds_[i] = distance * -1.0F;
    }
}

void CascadedShadowMap::UpdateMatrices(const Camera& camera, const DirectionalLight& directional_light)
{
    // Create cascade split list including initial (0.0F) split
    std::vector<float> boundaries{};
    boundaries.reserve(cascade_count_ + 1);
    boundaries.push_back(0.0F);
    boundaries.insert(boundaries.end(), cascade_splits_.begin(), cascade_splits_.end());

    // Reset world bounding boxes
    float max = std::numeric_limits<float>::max();
    float min = -max;
    for (math::Box& bounding_box : world_bounding_boxes_)
    {
        bounding_box.min_ = math::Vec3f(max);
        bounding_box.max_ = math::Vec3f(min);
    }

    math::Vec3f light_direction = math::Vec3f::Normalize(directional_light.direction_);
    math::Matrix4x4 inverse_view_projection_matrix = (camera.GetProjectionMatrix() * camera.GetViewMatrix()).Inverse();
    for (uint32 cascade_index = 0; cascade_index < cascade_count_; ++cascade_index)
    {
        float near_split = boundaries[cascade_index];
        float far_split = boundaries[cascade_index + 1];

        math::Box& world_bounding_box = world_bounding_boxes_[cascade_index];

        // Frustrum corners in normalized device coordinates
        std::array frustrum_corners =
        {
            math::Vec3f(-1.0f,  1.0f, -1.0f),
            math::Vec3f( 1.0f,  1.0f, -1.0f),
            math::Vec3f( 1.0f, -1.0f, -1.0f),
            math::Vec3f(-1.0f, -1.0f, -1.0f),

            math::Vec3f(-1.0f,  1.0f, 1.0f),
            math::Vec3f( 1.0f,  1.0f, 1.0f),
            math::Vec3f( 1.0f, -1.0f, 1.0f),
            math::Vec3f(-1.0f, -1.0f, 1.0f),
        };

        // Convert frustrum corners to world space
        for (uint32 i = 0; i < 8; ++i)
        {
            math::Vec4f inverse_corner = (inverse_view_projection_matrix * math::Vec4f(frustrum_corners[i].x_,
                                                                                       frustrum_corners[i].y_,
                                                                                       frustrum_corners[i].z_,
                                                                                       1.0F));
            frustrum_corners[i] = inverse_corner.XYZ() / inverse_corner.w_;

            world_bounding_box.min_ = math::Vec3f::GetMinimumCoordinates(world_bounding_box.min_, frustrum_corners[i]);
            world_bounding_box.max_ = math::Vec3f::GetMaximumCoordinates(world_bounding_box.max_, frustrum_corners[i]);
        }

        // Convert frustrum corners to match sub frustrum corners
        for (uint32 i = 0; i < 4; ++i)
        {
            math::Vec3f distance = frustrum_corners[i + 4] - frustrum_corners[i];
            frustrum_corners[i + 4] = frustrum_corners[i] + (distance * far_split);
            frustrum_corners[i] = frustrum_corners[i] + (distance * near_split);
        }

        // Find frustrum center
        math::Vec3f frustrum_center{0.0F};
        for (const math::Vec3f& corner : frustrum_corners)
        {
            frustrum_center += corner;
        }
        frustrum_center /= frustrum_corners.size();

        // Find longest radius for bounding box
        float radius = 0.0F;
        for (const math::Vec3f& corner : frustrum_corners)
        {
            float distance = (corner - frustrum_center).Magnitude();
            radius = math::Max(radius, distance);
        }
        radius = math::Ceil(radius);

        math::Vec3f max_extents{radius};
        math::Vec3f min_extents{-radius};

        math::Matrix4x4 light_view_matrix = math::Matrix4x4::LookAt(frustrum_center - (light_direction * max_extents.z_),
                                                                    frustrum_center,
                                                                    math::Vec3f::Up());

        math::Matrix4x4 orthographic_matrix = math::Matrix4x4::Orthographic(min_extents.x_,
                                                                            max_extents.x_,
                                                                            min_extents.y_,
                                                                            max_extents.y_,
                                                                            0.0F,
                                                                            max_extents.z_ - min_extents.z_);

        // Compute the crop matrix
        // This improves shadow map resolution by fitting the projection matrix to the bounds of the cascade
        math::Matrix4x4 shadow_mvp = orthographic_matrix * light_view_matrix;
        math::Vec3f minimum_coordinate{max};
        math::Vec3f maximum_coordinate{min};
        for (const math::Vec3f& corner : frustrum_corners)
        {
            math::Vec4f shadow_corner = shadow_mvp * math::Vec4f(corner.x_, corner.y_, corner.z_, 1.0F);
            math::Vec3f shadow_corner_div_w = shadow_corner.XYZ() / shadow_corner.w_;

            minimum_coordinate = math::Vec3f::GetMinimumCoordinates(minimum_coordinate, shadow_corner_div_w);
            maximum_coordinate = math::Vec3f::GetMaximumCoordinates(maximum_coordinate, shadow_corner_div_w);
        }

        math::Vec3f scale{2.0F / (maximum_coordinate.x_ - minimum_coordinate.x_),
                          2.0F / (maximum_coordinate.y_ - minimum_coordinate.y_),
                          1.0F / (maximum_coordinate.z_ - minimum_coordinate.z_)};
        math::Vec3f offset{-0.5F * (maximum_coordinate.x_ + minimum_coordinate.x_) * scale.x_,
                           -0.5F * (maximum_coordinate.y_ + minimum_coordinate.y_) * scale.y_,
                           -minimum_coordinate.z_ * scale.z_};
        math::Matrix4x4 crop_matrix = math::Matrix4x4::Identity().Scale(scale).Translate(offset);

        light_view_matrices_[cascade_index] = light_view_matrix;
        projection_matrices_[cascade_index] = crop_matrix * orthographic_matrix;
    }
}

void CascadedShadowMap::UpdateTextureMatrices()
{
    // Transform points from normalized device coordinates (NDC), [-1, 1], to texture coordinates, [0, 1]
    const math::Matrix4x4 texture_correction_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(0.5F))
            .Translate(math::Vec3f(0.5F));
    for (uint32 i = 0; i < cascade_count_; ++i)
    {
        texture_matrices_[i] = texture_correction_matrix * projection_matrices_[i] * light_view_matrices_[i];
    }
}

uint32 CascadedShadowMap::GetCascadeCount() const
{
    return cascade_count_;
}

std::vector<math::Matrix4x4> CascadedShadowMap::GetLightViewMatrices() const
{
    return light_view_matrices_;
}

std::vector<math::Matrix4x4> CascadedShadowMap::GetProjectionMatrices() const
{
    return projection_matrices_;
}

std::vector<math::Matrix4x4> CascadedShadowMap::GetTextureMatrices() const
{
    return texture_matrices_;
}

std::vector<math::Box> CascadedShadowMap::GetWorldBoundingBoxes() const
{
    return world_bounding_boxes_;
}

std::vector<float> CascadedShadowMap::GetViewFarBounds() const
{
    return view_far_bounds_;
}

} // namespace zero::render