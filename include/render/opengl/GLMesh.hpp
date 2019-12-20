#pragma once

#include "OpenGL.hpp"
#include "render/IRenderResource.hpp"
#include "render/Mesh.hpp"

namespace zero::render {

    /**
     * @brief OpenGL mesh container. Stores the mesh data in main memory as well.
     */
    class GLMesh final : public Mesh, public IRenderResource {
    public:

        /**
         * @brief Constructor
         * @param vertices the interleaved vertex data
         * @param indices the vertex indices
         */
        GLMesh(std::vector<Vertex>&& vertices, std::vector<uint16>&& indices);

        ~GLMesh() override;

        /**
         * @brief Initialize the OpenGL identifiers (VAO, VBO, EBO)
         */
        void Initialize();

    protected:
        /**
         * @brief Cleanup all of the rendering resources
         */
        void Cleanup() override;

    private:

        /**
         * @brief Vertex Array Object identifier
         */
        GLuint vao_;

        /**
         * @brief Interleaved Vertex Buffer Identifier
         */
        GLuint vbo_;

        /**
         * @brief Vertex Index Buffer Identifier
         */
        GLuint ebo_;

    }; // class GLMesh

} // namespace zero::render