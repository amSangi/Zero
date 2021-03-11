#include "render/renderer/opengl/ubo/GLDirectionalLightUniformBuffer.hpp"

namespace zero::render
{

struct alignas(16) DirectionalLightData
{
    explicit DirectionalLightData(const DirectionalLight& light)
    : color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
    , direction_(light.direction_.x_, light.direction_.y_, light.direction_.z_, 0.0F)
    , ambient_intensity_(light.ambient_intensity_)
    , diffuse_intensity_(light.diffuse_intensity_)
    , padding_()
    {
    }

    math::Vec4f color_;
    math::Vec4f direction_;
    float ambient_intensity_;
    float diffuse_intensity_;
    float padding_[2];
};


GLDirectionalLightUniformBuffer::GLDirectionalLightUniformBuffer()
: GLBaseUniformBuffer()
{
}

void GLDirectionalLightUniformBuffer::Initialize(uint32 binding_index)
{
    InitializeBaseBuffer<DirectionalLightData>(binding_index);
}

void GLDirectionalLightUniformBuffer::UpdateUniforms(const std::vector<DirectionalLight>& directional_lights)
{
    std::vector<DirectionalLightData> directional_light_data{};
    directional_light_data.reserve(directional_lights.size());

    for (const DirectionalLight& directional_light : directional_lights)
    {
        directional_light_data.emplace_back(directional_light);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightData) * directional_light_data.size(), directional_light_data.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render