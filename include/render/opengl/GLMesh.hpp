#pragma once

#include "OpenGL.hpp"
#include "render/Mesh.hpp"

namespace zero::render {

    /**
     * @brief Mesh container with the ability to draw using OpenGL
     */
    class GLMesh final : public Mesh {
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

        ~GLMesh() override = default;

        /**
         * @brief Render the mesh
         */
        void Draw();

    }; // class GLMesh

} // namespace zero::render