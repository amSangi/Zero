#pragma once

#include <memory>
#include "render/MeshData.hpp"
#include "render/renderer/IMesh.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLMesh final : public IMesh
    {
    public:
        GLMesh(GLuint vao_identifier, GLuint vertex_buffer, GLuint index_buffer, uint32 index_data_size);

        ~GLMesh() override = default;

        [[nodiscard]] GLuint GetVAOIdentifier() const;
        [[nodiscard]] GLuint GetVertexBufferIdentifier() const;
        [[nodiscard]] GLuint GetIndexBufferIdentifier() const;
        [[nodiscard]] uint32 GetIndexDataSize() const;

    private:
        GLuint vao_id_;
        GLuint vertex_buffer_id_;
        GLuint index_buffer_id_;
        uint32 index_data_size_;

    }; // class GLMesh

} // namespace zero::render