#include "render/renderer/opengl/ubo/GLModelUniformBuffer.hpp"

namespace zero::render
{

struct alignas(16) ModelData
{
    ModelData(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix)
    : model_matrix_(model_matrix)
    , normal_matrix_(normal_matrix)
    {
    }

    math::Matrix4x4 model_matrix_;
    math::Matrix4x4 normal_matrix_;
};

GLModelUniformBuffer::GLModelUniformBuffer()
: GLBaseUniformBuffer()
{
}

void GLModelUniformBuffer::Initialize(uint32 binding_index)
{
    InitializeBaseBuffer<ModelData>(binding_index);
}

void GLModelUniformBuffer::UpdateUniforms(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix)
{
    // Transpose matrices to switch from row-major to column-major order
    math::Matrix4x4 transpose_model_matrix = model_matrix.Transpose();
    math::Matrix4x4 transpose_normal_matrix = normal_matrix.Transpose();

    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
    ModelData model_data{transpose_model_matrix, transpose_normal_matrix};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ModelData), &model_data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace zero::render