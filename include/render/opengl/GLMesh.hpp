#pragma once

#include "render/Mesh.hpp"
#include "render/opengl/OpenGL.hpp"

namespace zero::render
{

    /**
     * @brief Mesh container with the ability to draw using OpenGL
     */
    class GLMesh final : public Mesh
    {
    public:

        /**
         * @brief Constructor
         * @param vertices the interleaved vertex data
         * @param indices the vertex indices
         */
        GLMesh(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices);

        /**
         * @brief Constructor
         * @param mesh the mesh
         */
        explicit GLMesh(Mesh&& mesh);

        ~GLMesh() override;

        /**
         * @brief Setup the Vertex Array Object and Vertex Buffer Objects for rendering
         */
        void Initialize();

        /**
         * @brief Render the mesh
         */
        void Draw();

    private:
        GLuint vao_;
        GLuint vertex_vbo_;
        GLuint index_vbo_;

    }; // class GLMesh

} // namespace zero::render