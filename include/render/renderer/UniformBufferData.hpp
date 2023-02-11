#pragma once

#include "math/Matrix4x4.hpp"
#include "math/Vector4.hpp"
#include "component/Light.hpp"
#include "component/Material.hpp"
#include "component/Transform.hpp"
#include "render/Constants.hpp"

namespace zero::render
{

	struct alignas(16) CameraData
	{
		CameraData(const math::Matrix4x4& projection_matrix,
		           const math::Matrix4x4& view_matrix,
		           const math::Vec3f& camera_position)
				: projection_matrix_(projection_matrix)
				, view_matrix_(view_matrix)
				, camera_position_(camera_position.x_,
				                   camera_position.y_,
				                   camera_position.z_,
				                   1.0F)
		{
		}

		math::Matrix4x4 projection_matrix_;
		math::Matrix4x4 view_matrix_;
		math::Vec4f camera_position_;
	};

	struct alignas(16) ModelData
	{
		ModelData(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix)
		: model_matrix_(model_matrix)
		, normal_matrix_(normal_matrix)
		{
		}

		math::Matrix4x4 model_matrix_;
		math::Matrix4x4 normal_matrix_;
	};

	struct alignas(16) MaterialData
	{
		explicit MaterialData(const Material& material)
		: diffuse_color_(material.diffuse_color_.x_,
		                 material.diffuse_color_.y_,
		                 material.diffuse_color_.z_,
		                 1.0F)
		, specular_intensity_(material.specular_intensity_)
		, specular_exponent_(material.specular_exponent_)
		{
		}

		math::Vec4f diffuse_color_;
		float specular_intensity_;
		float specular_exponent_;
	};

	struct alignas(16) LightInformationData
	{
		LightInformationData(uint32 directional_light_count, uint32 point_light_count, uint32 spot_light_count)
		: directional_light_count_(directional_light_count)
		, point_light_count_(point_light_count)
		, spot_light_count_(spot_light_count)
		{
		}

		uint32 directional_light_count_;
		uint32 point_light_count_;
		uint32 spot_light_count_;
	};

	struct alignas(16) DirectionalLightData
	{
		explicit DirectionalLightData(const DirectionalLight& light)
		: color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
		, direction_(light.direction_.x_, light.direction_.y_, light.direction_.z_, 0.0F)
		, ambient_intensity_(light.ambient_intensity_)
		, diffuse_intensity_(light.diffuse_intensity_)
		, padding_()
		{
		}

		math::Vec4f color_;
		math::Vec4f direction_;
		float ambient_intensity_;
		float diffuse_intensity_;
		float padding_[2];
	};

	struct alignas(16) PointLightData
	{
		PointLightData(const Transform& transform, const PointLight& light)
		: position_(transform.position_.x_,
		            transform.position_.y_,
		            transform.position_.z_,
		            0.0F)
		, color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
		, ambient_intensity_(light.ambient_intensity_)
		, diffuse_intensity_(light.diffuse_intensity_)
		, attenuation_constant_(light.attenuation_.constant_)
		, attenuation_linear_(light.attenuation_.linear_)
		, attenuation_quadratic_(light.attenuation_.quadratic_)
		, padding_()
		{
		}

		math::Vec4f position_;
		math::Vec4f color_;
		float ambient_intensity_;
		float diffuse_intensity_;
		float attenuation_constant_;
		float attenuation_linear_;
		float attenuation_quadratic_;
		float padding_[3];
	};

	struct alignas(16) SpotLightData
	{
		SpotLightData(const Transform& transform, const SpotLight& light)
		: position_(transform.position_.x_,
		            transform.position_.y_,
		            transform.position_.z_,
		            0.0F)
		, color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
		, direction_(light.direction_.x_, light.direction_.y_, light.direction_.z_, 0.0F)
		, ambient_intensity_(light.ambient_intensity_)
		, diffuse_intensity_(light.diffuse_intensity_)
		, inner_cosine_(math::Cos(light.inner_cone_angle_.ToRadian().rad_))
		, outer_cosine_(math::Cos(light.outer_cone_angle_.ToRadian().rad_))
		, attenuation_constant_(light.attenuation_.constant_)
		, attenuation_linear_(light.attenuation_.linear_)
		, attenuation_quadratic_(light.attenuation_.quadratic_)
		, padding_()
		{
		}

		math::Vec4f position_;
		math::Vec4f color_;
		math::Vec4f direction_;
		float ambient_intensity_;
		float diffuse_intensity_;
		float inner_cosine_;
		float outer_cosine_;
		float attenuation_constant_;
		float attenuation_linear_;
		float attenuation_quadratic_;
		float padding_[1];
	};


	struct alignas(16) ShadowMapInformation
	{
		ShadowMapInformation(const std::vector<math::Matrix4x4>& shadow_map_matrices,
		                     const std::vector<float>& cascade_far_bounds)
		: cascade_far_bounds_()
		, shadow_map_matrices_()
		{
			for (uint32 i = 0; i < Constants::kShadowCascadeCount; ++i)
			{
				shadow_map_matrices_[i] = shadow_map_matrices[i].Transpose();
				cascade_far_bounds_[i].z_ = cascade_far_bounds[i];
			}
		}

		math::Matrix4x4 shadow_map_matrices_[Constants::kShadowCascadeCount];
		math::Vec4f cascade_far_bounds_[Constants::kShadowCascadeCount];
	};


} // namespace zero::render