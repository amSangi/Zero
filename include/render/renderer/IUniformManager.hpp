#pragma once

#include <memory>
#include "component/Camera.hpp"
#include "component/Light.hpp"
#include "component/Material.hpp"
#include "component/Transform.hpp"
#include "component/SkyDome.hpp"
#include "core/NonCopyable.hpp"
#include "render/renderer/IProgram.hpp"
#include "render/renderer/IUniformBuffer.hpp"


namespace zero::render
{


    class IUniformManager : public NonCopyable
    {
    public:
        enum class UniformBufferType
        {
            BONE_BUFFER,
            CAMERA_BUFFER,
            LIGHT_INFO_BUFFER,
            DIRECTIONAL_LIGHT_BUFFER,
            POINT_LIGHT_BUFFER,
            SPOT_LIGHT_BUFFER,
            MATERIAL_BUFFER,
            MODEL_BUFFER,
            SHADOW_MAP_BUFFER,
        };

        virtual ~IUniformManager() = default;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        virtual void SetShadowSamplerName(IProgram* shader_program, uint32 texture_index) = 0;
        virtual void SetDiffuseSamplerName(IProgram* shader_program, uint32 texture_index) = 0;
        virtual void SetSkyDomeUniforms(IProgram* sky_dome_program, const Camera& camera, const SkyDome& sky_dome) = 0;

        virtual void UpdateBoneUniforms(std::vector<math::Matrix4x4> bone_matrices) = 0;

        virtual void UpdateCameraUniforms(const math::Matrix4x4& projection_matrix,
                                          const math::Matrix4x4& view_matrix,
                                          const math::Vec3f& camera_position) = 0;

        virtual void UpdateLightUniforms(const std::vector<DirectionalLight>& directional_lights,
                                         const std::vector<std::pair<PointLight, Transform>>& point_lights,
                                         const std::vector<std::pair<SpotLight, Transform>>& spot_lights) = 0;

        virtual void UpdateMaterialUniforms(const Material& material) = 0;

        virtual void UpdateModelUniforms(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix) = 0;

        virtual void UpdateShadowMapUniforms(const std::vector<math::Matrix4x4>& light_matrices,
                                             const std::vector<float>& cascade_end_clip_spaces) = 0;

        virtual IUniformBuffer* GetUniformBuffer(UniformBufferType buffer_type) = 0;

    }; // interface IUniformManager

} // namespace zero::render