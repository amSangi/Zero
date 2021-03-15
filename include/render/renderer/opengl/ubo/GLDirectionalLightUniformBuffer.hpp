#pragma once

#include <vector>
#include "component/Light.hpp"
#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLDirectionalLightUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLDirectionalLightUniformBuffer();
        ~GLDirectionalLightUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(const std::vector<DirectionalLight>& directional_lights);
    };

} // namespace zero::render