#pragma once

#include <vector>
#include "component/Light.hpp"
#include "component/Transform.hpp"
#include "render/renderer/opengl/ubo/GLBaseUniformBuffer.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLPointLightUniformBuffer final : public GLBaseUniformBuffer
    {
    public:
        GLPointLightUniformBuffer();
        ~GLPointLightUniformBuffer() override = default;
        void Initialize(uint32 binding_index);
        void UpdateUniforms(const std::vector<Transform>& transforms, const std::vector<PointLight>& point_lights);
    };

} // namespace zero::render