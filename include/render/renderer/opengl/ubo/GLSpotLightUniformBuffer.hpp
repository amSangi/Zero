#pragma once

#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"
#include "component/Light.hpp"
#include "component/Transform.hpp"

namespace zero::render
{

    class GLSpotLightUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLSpotLightUniformBuffer();
        ~GLSpotLightUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(const std::vector<std::pair<SpotLight, Transform>>& spot_lights);
    };

} // namespace zero::render