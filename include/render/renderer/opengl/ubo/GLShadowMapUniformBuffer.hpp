#pragma once

#include <vector>
#include "math/Matrix4x4.hpp"
#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLShadowMapUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLShadowMapUniformBuffer();
        ~GLShadowMapUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(const std::vector<math::Matrix4x4>& shadow_map_matrices,
                            const std::vector<float>& cascade_end_clip_spaces);
    };

} // namespace zero::render