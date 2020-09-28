#include "render/CascadedShadowMap.hpp"

namespace zero::render
{

CascadedShadowMap::CascadedShadowMap(uint32 cascade_count, uint32 shadow_map_width, uint32 shadow_map_height)
: cascade_count_(cascade_count)
, shadow_map_width_(shadow_map_width)
, shadow_map_height_(shadow_map_height)
, light_view_matrix_(math::Matrix4x4::Identity())
, projection_matrices_(cascade_count_, math::Matrix4x4::Identity())
, texture_matrices_(cascade_count_, math::Matrix4x4::Identity())
, far_bounds_(cascade_count_, 0.0F)
{
}

void CascadedShadowMap::Update(const Camera& camera, const DirectionalLight& directional_light)
{
	const math::Matrix4x4 bias_matrix = math::Matrix4x4::Identity()
			.Scale(math::Vec3f(0.5F))
			.Translate(math::Vec3f(0.5F));
	light_view_matrix_ = math::Matrix4x4::LookAt(math::Vec3f::Zero(),
											     math::Vec3f::Normalize(directional_light.direction_),
											     math::Vec3f(0.0F, 0.0F, 1.0F));
	// Set the far z position for each cascade
	float z_distance = camera.far_clip_ - camera.near_clip_;
    far_bounds_[0] = (math::Min(25.0F, math::Ceil(z_distance * 0.10F)));
    far_bounds_[1] = (math::Min(125.0F, math::Ceil(z_distance * 0.40F)));
    far_bounds_[2] = (math::Ceil(z_distance));

	UpdateProjectionMatrices(camera);
    for (uint32 i = 0; i < cascade_count_; ++i)
    {
    	texture_matrices_[i] = (bias_matrix * projection_matrices_[i] * light_view_matrix_).Transpose();
    }
}

void CascadedShadowMap::UpdateProjectionMatrices(const Camera& camera)
{
	// Compute near and far boundaries for all of the cascades
	std::vector<float> z_boundaries{};
	z_boundaries.reserve(cascade_count_ + 1);
	z_boundaries.push_back(camera.near_clip_);
	for (float end_clip_space : far_bounds_)
	{
		z_boundaries.push_back(end_clip_space);
	}

	float aspect_ratio = camera.viewport_.GetAspectRatio();
	float tan_half_h_fov = math::Tan(camera.horizontal_field_of_view_.ToRadian().rad_ * 0.5F);
	float tan_half_v_fov = math::Tan(camera.horizontal_field_of_view_.ToRadian().rad_ * 0.5F * aspect_ratio);
	math::Matrix4x4 inverse_view_transform = camera.GetCameraToWorldMatrix();

	// Compute projection matrices corners
	constexpr uint32 kFrustrumCornerCount = 8;
	for (uint32 i = 0; i < cascade_count_; ++i)
	{
		float z_near = z_boundaries[i];
		float z_far  = z_boundaries[i + 1];

		float x_near = z_near * tan_half_h_fov;
		float x_far  = z_far * tan_half_h_fov;

		float y_near = z_near * tan_half_v_fov;
		float y_far  = z_far * tan_half_v_fov;

		// Compute frustrum corners for cascade sub-frustrum
		std::array<math::Vec4f, kFrustrumCornerCount> frustrum_corners =
		{
			// Near plane corners
			math::Vec4f( x_near,  y_near, z_near, 1.0F), // Top Right
			math::Vec4f(-x_near,  y_near, z_near, 1.0F), // Top Left
			math::Vec4f( x_near, -y_near, z_near, 1.0F), // Bottom Right
			math::Vec4f(-x_near, -y_near, z_near, 1.0F), // Bottom Left
			// Far plane corners
			math::Vec4f( x_far,  y_far, z_far, 1.0F), // Top Right
			math::Vec4f(-x_far,  y_far, z_far, 1.0F), // Top Left
			math::Vec4f( x_far, -y_far, z_far, 1.0F), // Bottom Right
			math::Vec4f(-x_far, -y_far, z_far, 1.0F), // Bottom Left
		};

		float max = std::numeric_limits<float>::max();
		float min_x = max;
		float min_y = max;
		float min_z = max;

		float min = std::numeric_limits<float>::min();
		float max_x = min;
		float max_y = min;
		float max_z = min;

		// Compute min/max corners of the bounding box in the light view space
		for (const math::Vec4f& frustrum_corner : frustrum_corners)
		{
			// Convert corner into world space coordinates
			math::Vec4f world_space_frustrum_corner = inverse_view_transform * frustrum_corner;
			// Convert corner into light space coordinates
			math::Vec4f light_space_frustrum_corner = light_view_matrix_ * world_space_frustrum_corner;

			min_x = math::Min(min_x, light_space_frustrum_corner.x_);
			min_y = math::Min(min_y, light_space_frustrum_corner.y_);
			min_z = math::Min(min_z, light_space_frustrum_corner.z_);

			max_x = math::Max(max_x, light_space_frustrum_corner.x_);
			max_y = math::Max(max_y, light_space_frustrum_corner.y_);
			max_z = math::Max(max_z, light_space_frustrum_corner.z_);
		}

		projection_matrices_[i] = math::Matrix4x4::Orthographic(min_x, max_x, min_y, max_y, min_z, max_z);
	}
}

uint32 CascadedShadowMap::GetCascadeCount() const
{
	return cascade_count_;
}

math::Matrix4x4 CascadedShadowMap::GetLightViewMatrix() const
{
	return light_view_matrix_;
}

std::vector<math::Matrix4x4> CascadedShadowMap::GetProjectionMatrices() const
{
	return projection_matrices_;
}

std::vector<math::Matrix4x4> CascadedShadowMap::GetTextureMatrices() const
{
	return texture_matrices_;
}

std::vector<float> CascadedShadowMap::GetCascadeFarBounds() const
{
	return far_bounds_;
}

} // namespace zero::render