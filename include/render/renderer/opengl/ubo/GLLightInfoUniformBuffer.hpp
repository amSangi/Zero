#pragma once

#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLLightInfoUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLLightInfoUniformBuffer();
        ~GLLightInfoUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(uint32 directional_light_count,
                            uint32 point_light_count,
                            uint32 spot_light_count);
    };

} // namespace zero::render