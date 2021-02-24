#pragma once

#include "core/NonCopyable.hpp"
#include "render/IMesh.hpp"
#include "render/opengl/OpenGL.hpp"

namespace zero::render
{

    /**
     * @brief Mesh container with the ability to draw using OpenGL
     */
    class GLMesh : public NonCopyable
    {
    public:
        /**
         * @brief Setup the Vertex Array Object and Vertex Buffer Objects for rendering
         * @param vertices the vertex data for the mesh
         * @param indices the index data for the mesh
         */
        GLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);

        /**
         * @brief Setup the Vertex Array Object and Vertex Buffer Objects for rendering
         * @param mesh the mesh data
         */
        explicit GLMesh(const Mesh& mesh);

        ~GLMesh();

        /**
         * @brief Render the mesh
         */
        void Draw();

    private:
        GLuint vao_;
        GLuint vertex_vbo_;
        GLuint index_vbo_;
        uint32 index_data_size_;

    }; // class GLMesh

} // namespace zero::render