#include "render/renderer/opengl/ubo/GLPointLightUniformBuffer.hpp"

namespace zero::render
{

struct alignas(16) PointLightData
{
    PointLightData(const Transform& transform, const PointLight& light)
    : position_(transform.GetPosition().x_,
                transform.GetPosition().y_,
                transform.GetPosition().z_,
                0.0F)
    , color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
    , ambient_intensity_(light.ambient_intensity_)
    , diffuse_intensity_(light.diffuse_intensity_)
    , attenuation_constant_(light.attenuation_.constant_)
    , attenuation_linear_(light.attenuation_.linear_)
    , attenuation_quadratic_(light.attenuation_.quadratic_)
    , padding_()
    {
    }

    math::Vec4f position_;
    math::Vec4f color_;
    float ambient_intensity_;
    float diffuse_intensity_;
    float attenuation_constant_;
    float attenuation_linear_;
    float attenuation_quadratic_;
    float padding_[3];
};

GLPointLightUniformBuffer::GLPointLightUniformBuffer()
: GLBaseUniformBuffer()
{
}

void GLPointLightUniformBuffer::Initialize(uint32 binding_index)
{
    InitializeBaseBuffer<PointLightData>(binding_index);
}

void GLPointLightUniformBuffer::UpdateUniforms(const std::vector<PointLight>& point_lights)
{
    std::vector<PointLightData> point_light_data{};
    point_light_data.reserve(point_lights.size());

    for (const PointLight& point_light : point_lights)
    {
        point_light_data.emplace_back(point_light);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLightData) * point_light_data.size(), point_light_data.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render