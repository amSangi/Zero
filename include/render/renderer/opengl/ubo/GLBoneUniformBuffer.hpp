#pragma once

#include <vector>
#include "math/Matrix4x4.hpp"
#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLBoneUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLBoneUniformBuffer();
        ~GLBoneUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(std::vector<math::Matrix4x4> bone_matrices);
    }; // class GLBoneUniformBuffer

} // namespace zero