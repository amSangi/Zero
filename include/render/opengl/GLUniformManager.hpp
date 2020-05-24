#pragma once

#include "OpenGL.hpp"
#include "GLProgram.hpp"
#include "core/NonCopyable.hpp"
#include "core/Transform.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Vector3.hpp"
#include "render/Camera.hpp"
#include "render/Material.hpp"
#include <entt/entt.hpp>
#include <vector>

namespace zero::render
{

    // Forward declarations
    class GLTexture;

    class GLUniformManager final : public NonCopyable
    {
    public:
        GLUniformManager();
        ~GLUniformManager();

        void Initialize();

        void UpdateCameraUniforms(const math::Matrix4x4& projection_matrix,
                                  const math::Matrix4x4& view_matrix,
                                  const math::Vec3f& camera_position) const;

        void UpdateModelUniforms(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix) const;

        void UpdateLightUniforms(const entt::registry& registry) const;

        void UpdateMaterialUniforms(const Material& material) const;

        static void UpdateSamplerUniforms(const std::shared_ptr<GLProgram>& program,
                                   const std::vector<std::shared_ptr<GLTexture>>& textures);

        static void BindGraphicsProgram(const std::shared_ptr<GLProgram>& program);

        static uint32 GetMaxDirectionalLightCount();
        static uint32 GetMaxPointLightCount();
        static uint32 GetMaxSpotLightCount();

    private:
        /**
         * @brief Destroy the program resources
         */
        void Cleanup();

        static uint32 kMaxDirectionalLights;
        static uint32 kMaxPointLights;
        static uint32 kMaxSpotLights;

        GLuint camera_buffer_id_;
        GLuint model_buffer_id_;
        GLuint light_buffer_id_;
        GLuint material_id_;

    }; // class GLUniformManager

} // namespace zero::render
