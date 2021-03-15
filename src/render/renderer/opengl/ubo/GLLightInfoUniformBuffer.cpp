#include "render/renderer/opengl/ubo/GLLightInfoUniformBuffer.hpp"

namespace zero::render
{

struct alignas(16) LightInformationData
{
    LightInformationData(uint32 directional_light_count, uint32 point_light_count, uint32 spot_light_count)
    : directional_light_count_(directional_light_count)
    , point_light_count_(point_light_count)
    , spot_light_count_(spot_light_count)
    {
    }

    uint32 directional_light_count_;
    uint32 point_light_count_;
    uint32 spot_light_count_;
};

GLLightInfoUniformBuffer::GLLightInfoUniformBuffer()
: GLBaseUniformBuffer("LightInformation")
{
}

void GLLightInfoUniformBuffer::Initialize(uint32 binding_index)
{
    InitializeBaseBuffer<LightInformationData>(binding_index);
}

void GLLightInfoUniformBuffer::UpdateUniforms(uint32 directional_light_count,
                                              uint32 point_light_count,
                                              uint32 spot_light_count)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    LightInformationData light_information_data{directional_light_count, point_light_count, spot_light_count};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(light_information_data), &light_information_data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render

