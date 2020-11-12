#pragma once

#include <vector>
#include <entt/entt.hpp>
#include "core/NonCopyable.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Vector3.hpp"
#include "render/opengl/OpenGL.hpp"
#include "render/opengl/GLProgram.hpp"

namespace zero
{
    // Forward declarations
    class Material;

namespace render
{

    // Forward declarations
    class GLTexture;

    /**
     * @brief Manage all GLSL uniform updates through the use of uniform buffer objects
     */
    class GLUniformManager final : public NonCopyable
    {
    public:
        //////////////////////////////////////////////////
        ///// Maximum Uniform Counts
        //////////////////////////////////////////////////

        /**
         * @brief The maximum directional light count
         */
        static constexpr uint32 kMaxDirectionalLights = 4U;
        /**
         * @brief The maximum point light count
         */
        static constexpr uint32 kMaxPointLights = 4U;
        /**
         * @brief The maximum spot light count
         */
        static constexpr uint32 kMaxSpotLights = 4U;
        /**
         * @brief The number of cascades used in Cascaded Shadow Mapping
         */
        static constexpr uint32 kShadowCascadeCount = 3U;


        GLUniformManager();
        ~GLUniformManager();

        /**
         * @brief Generate and bind all uniform buffer objects
         */
        void Initialize();

        //////////////////////////////////////////////////
        ///// Update Uniforms
        //////////////////////////////////////////////////

        /**
         * @brief Update the camera uniforms
         * @param projection_matrix the projection matrix
         * @param view_matrix the view matrix
         * @param camera_position the camera's position
         */
        void UpdateCameraUniforms(const math::Matrix4x4& projection_matrix,
                                  const math::Matrix4x4& view_matrix,
                                  const math::Vec3f& camera_position) const;

        /**
         * @brief Update the material uniforms
         * @param material the material component of an entity
         */
        void UpdateMaterialUniforms(const Material& material) const;

        /**
         * @brief Update the model uniforms
         * @param model_matrix the model local to world matrix
         * @param normal_matrix the model's normal matrix used to transform normals
         */
        void UpdateModelUniforms(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix) const;

        /**
         * @brief Update the light uniforms
         * @param registry the registry containing all of the entities and their components
         */
        void UpdateLightUniforms(const entt::registry& registry) const;

        /**
         * @brief Update the cascaded shadow map uniforms
         * @param light_matrices the light matrices for each cascade
         * @param cascade_end_clip_spaces the end z position in clip space for each cascade
         */
        void UpdateShadowMapMatrices(const std::vector<math::Matrix4x4>& light_matrices,
                                     const std::vector<float>& cascade_end_clip_spaces) const;

        //////////////////////////////////////////////////
        ///// Bind Graphics Program Uniforms
        //////////////////////////////////////////////////

        /**
         * @brief Get the uniform name for a specific sampler2D in a list of sampler2Ds
         * @param sampler_index the index into the list of sampler2Ds
         * @return the indexed uniform sampler2D name
         */
        static std::string GetShadowSamplerUniformName(int32 sampler_index);

        static std::string GetDiffuseSamplerName();

        /**
         * @brief Bind the block indices of a graphics program to their predefined binding points for a certain
         *        group of uniforms
         *
         * @param program the graphics program
         */
        ///@{
        void BindCameraUniforms(const std::shared_ptr<GLProgram>& program);
        void BindMaterialUniforms(const std::shared_ptr<GLProgram>& program);
        void BindModelUniforms(const std::shared_ptr<GLProgram>& program);
        void BindLightUniforms(const std::shared_ptr<GLProgram>& program);
        void BindShadowMapUniforms(const std::shared_ptr<GLProgram>& program);
        ///@}

    private:
        /**
         * @brief Destroy the buffer objects
         */
        void Cleanup();

        void InitializeCameraUniformBuffer();
        void InitializeMaterialUniformBuffer();
        void InitializeModelUniformBuffer();
        void InitializeLightInfoUniformBuffer();
        void InitializeDirectionalLightUniformBuffer();
        void InitializePointLightUniformBuffer();
        void InitializeSpotLightUniformBuffer();
        void InitializeShadowMapMatrixUniformBuffer();

        void UpdateDirectionalLightUniforms(const entt::registry& registry) const;
        void UpdatePointLightUniforms(const entt::registry& registry) const;
        void UpdateSpotLightUniforms(const entt::registry& registry) const;

        static const uint32 kCameraBindingIndex;
        static const uint32 kMaterialBindingIndex;
        static const uint32 kModelBindingIndex;
        static const uint32 kLightInfoBindingIndex;
        static const uint32 kDirectionalLightBindingIndex;
        static const uint32 kPointLightBindingIndex;
        static const uint32 kSpotLightBindingIndex;
        static const uint32 kShadowMapMatrixBindingIndex;

        GLuint camera_buffer_id_;
        GLuint material_buffer_id_;
        GLuint model_buffer_id_;
        GLuint light_info_buffer_id_;
        GLuint directional_light_buffer_id_;
        GLuint point_light_buffer_id_;
        GLuint spot_light_buffer_id_;
        GLuint shadow_map_matrix_buffer_id_;

    }; // class GLUniformManager

} // namespace render
} // namespace zero
