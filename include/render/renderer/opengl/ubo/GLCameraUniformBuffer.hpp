#pragma once

#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLCameraUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLCameraUniformBuffer();
        ~GLCameraUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(const math::Matrix4x4& projection_matrix,
                            const math::Matrix4x4& view_matrix,
                            const math::Vec3f& camera_position);
    };

} // namespace zero::render