#include "render/CascadedShadowMap.hpp"

namespace zero::render
{

const float CascadedShadowMap::kMaxCascadeZCoverage = 50.0F;

CascadedShadowMap::CascadedShadowMap(uint32 cascade_count, uint32 shadow_map_width, uint32 shadow_map_height)
: cascade_count_(cascade_count)
, shadow_map_width_(shadow_map_width)
, shadow_map_height_(shadow_map_height)
, light_view_matrices_(cascade_count_, math::Matrix4x4::Identity())
, projection_matrices_(cascade_count_, math::Matrix4x4::Identity())
, texture_matrices_(cascade_count_, math::Matrix4x4::Identity())
, cascade_splits_(cascade_count_, 0.0F)
, view_far_bounds_(cascade_count_, 0.0F)
{
}

void CascadedShadowMap::Update(const Camera& camera, const DirectionalLight& directional_light)
{
    UpdateCascadeSplits(camera);
    UpdateProjectionMatrices(camera, directional_light);
    UpdateTextureMatrices();
}

void CascadedShadowMap::UpdateCascadeSplits(const Camera& camera)
{
    // Logarithmic contribution to final distance
    constexpr float kLambda = 0.95F;

    // Practical Split Scheme
    // Compute the far bound splits in world coordinates
    float z_range = camera.far_clip_ - camera.near_clip_;
    float ratio = camera.far_clip_ / camera.near_clip_;
    for (uint32 i = 0; i < cascade_count_; ++i)
    {
        float cascade_factor = static_cast<float>(i + 1) / static_cast<float>(cascade_count_);
        float logarithmic_distance = camera.near_clip_ * math::Pow(ratio, cascade_factor);
        float uniform_distance = camera.near_clip_ + (z_range * cascade_factor);
        float distance = kLambda * (logarithmic_distance - uniform_distance) + uniform_distance;
        cascade_splits_[i] = (distance - camera.near_clip_) / z_range;
        view_far_bounds_[i] = distance;
    }
}

void CascadedShadowMap::UpdateProjectionMatrices(const Camera& camera, const DirectionalLight& directional_light)
{
    std::vector<float> boundaries{};
    boundaries.reserve(cascade_count_ + 1);
    boundaries.push_back(0.0F);
    boundaries.insert(boundaries.end(), cascade_splits_.begin(), cascade_splits_.end());

    math::Vec3f light_direction = math::Vec3f::Normalize(directional_light.direction_);
    math::Matrix4x4 inverse_view_projection_matrix = (camera.GetProjectionMatrix() * camera.GetViewMatrix()).Inverse();
    for (uint32 cascade_index = 0; cascade_index < cascade_count_; ++cascade_index)
    {
        float z_near = boundaries[cascade_index];
        float z_far = boundaries[cascade_index + 1];

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
        }

        // Convert frustrum corners to match sub frustrum corners
        for (uint32 i = 0; i < 4; ++i)
        {
            math::Vec3f corner = frustrum_corners[i + 4] - frustrum_corners[i];
            math::Vec3f near_corner = corner * z_near;
            math::Vec3f far_corner = corner * z_far;

            frustrum_corners[i + 4] = frustrum_corners[i] + far_corner;
            frustrum_corners[i] = frustrum_corners[i] + near_corner;
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

        math::Matrix4x4 light_view_matrix = math::Matrix4x4::LookAt(frustrum_center - (light_direction * -min_extents.z_),
                                                                    frustrum_center,
                                                                    math::Vec3f::Up());



        math::Matrix4x4 orthographic_matrix = math::Matrix4x4::Orthographic(min_extents.x_,
                                                                            max_extents.x_,
                                                                            min_extents.y_,
                                                                            max_extents.y_,
                                                                            0.0F,
                                                                            max_extents.z_ - min_extents.z_);

        light_view_matrices_[cascade_index] = light_view_matrix;
        projection_matrices_[cascade_index] = orthographic_matrix;
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

std::vector<float> CascadedShadowMap::GetViewFarBounds() const
{
    return view_far_bounds_;
}

} // namespace zero::render