#pragma once

#include "component/Material.hpp"
#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLMaterialUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLMaterialUniformBuffer();
        ~GLMaterialUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(const Material& material);
    };

} // namespace zero::render