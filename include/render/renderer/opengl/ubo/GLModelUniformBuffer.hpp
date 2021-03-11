#pragma once

#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLModelUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLModelUniformBuffer();
        ~GLModelUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix);
    };

} // namespace zero::render