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

        ~GLMesh() override = default;

        /**
         * @brief Render the mesh
         */
        void Draw();

    }; // class GLMesh

} // namespace zero::render