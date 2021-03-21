#pragma once

#include "render/renderer/IUniformManager.hpp"
#include "render/renderer/opengl/ubo/GLCameraUniformBuffer.hpp"
#include "render/renderer/opengl/ubo/GLDirectionalLightUniformBuffer.hpp"
#include "render/renderer/opengl/ubo/GLLightInfoUniformBuffer.hpp"
#include "render/renderer/opengl/ubo/GLMaterialUniformBuffer.hpp"
#include "render/renderer/opengl/ubo/GLModelUniformBuffer.hpp"
#include "render/renderer/opengl/ubo/GLPointLightUniformBuffer.hpp"
#include "render/renderer/opengl/ubo/GLShadowMapUniformBuffer.hpp"
#include "render/renderer/opengl/ubo/GLSpotLightUniformBuffer.hpp"

namespace zero::render
{

    class GLUniformManager final : public IUniformManager
    {
    public:

        GLUniformManager();
        ~GLUniformManager() override;

        void Initialize() override;
        void Shutdown() override;
        void SetShadowSamplerName(IProgram* shader_program, uint32 texture_index) override;
        void SetDiffuseSamplerName(IProgram* shader_program, uint32 texture_index) override;
        void SetSkyDomeUniforms(IProgram* sky_dome_program, const Camera& camera, const SkyDome& sky_dome) override;

        void UpdateCameraUniforms(const math::Matrix4x4& projection_matrix,
                                  const math::Matrix4x4& view_matrix,
                                  const math::Vec3f& camera_position) override;

        void UpdateLightUniforms(const std::vector<DirectionalLight>& directional_lights,
                                 const std::vector<std::pair<PointLight, Transform>>& point_lights,
                                 const std::vector<std::pair<SpotLight, Transform>>& spot_lights) override;

        void UpdateMaterialUniforms(const Material& material) override;

        void UpdateModelUniforms(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix) override;

        void UpdateShadowMapUniforms(const std::vector<math::Matrix4x4>& light_matrices,
                                     const std::vector<float>& cascade_end_clip_spaces) override;

        IUniformBuffer* GetUniformBuffer(UniformBufferType buffer_type) override;

    private:
        std::unique_ptr<GLCameraUniformBuffer> camera_buffer_;
        std::unique_ptr<GLMaterialUniformBuffer> material_buffer_;
        std::unique_ptr<GLModelUniformBuffer> model_buffer_;
        std::unique_ptr<GLShadowMapUniformBuffer> shadow_map_buffer_;
        std::unique_ptr<GLLightInfoUniformBuffer> light_info_buffer_;
        std::unique_ptr<GLDirectionalLightUniformBuffer> directional_light_buffer_;
        std::unique_ptr<GLPointLightUniformBuffer> point_light_buffer_;
        std::unique_ptr<GLSpotLightUniformBuffer> spot_light_buffer_;

    }; // class GLUniformManager

} // namespace zero::render
