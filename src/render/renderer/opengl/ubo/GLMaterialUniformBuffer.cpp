#include "render/renderer/opengl/ubo/GLMaterialUniformBuffer.hpp"

namespace zero::render
{

struct alignas(16) MaterialData
{
    explicit MaterialData(const Material& material)
    : diffuse_color_(material.diffuse_color_.x_,
                     material.diffuse_color_.y_,
                     material.diffuse_color_.z_,
                     1.0F)
    , specular_intensity_(material.specular_intensity_)
    , specular_exponent_(material.specular_exponent_)
    {
    }

    math::Vec4f diffuse_color_;
    float specular_intensity_;
    float specular_exponent_;
};

GLMaterialUniformBuffer::GLMaterialUniformBuffer()
: GLBaseUniformBuffer()
{
}

void GLMaterialUniformBuffer::Initialize(uint32 binding_index)
{
    InitializeBaseBuffer<MaterialData>(binding_index);
}

void GLMaterialUniformBuffer::UpdateUniforms(const Material& material)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    MaterialData material_data{material};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialData), &material_data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render