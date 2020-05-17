#pragma once

#include <vector>
#include "math/Vector2.hpp"
#include "math/Vector3.hpp"

namespace zero::render
{

    /**
     * @brief Interleaved Mesh Vertex
     */
    struct Vertex
    {
        /**
         * @brief The vertex position
         */
        math::Vec3f position_;
        /**
         * @brief The vertex normal
         */
        math::Vec3f normal_;
        /**
         * @brief The vertex UV texture coordinate
         */
        math::Vec2f texture_coordinate_;
    }; // struct Vertex

    /**
     * @brief Interleaved Mesh Container
     */
    class Mesh
    {
    public:
        /**
         * @brief Constructor
         * @param vertices Interleaved vertices
         * @param indices Vertex indices
         */
        Mesh(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices);

        /**
         * @brief Move Constructor
         * @param mesh the mesh
         */
        Mesh(Mesh&& mesh) noexcept;

        virtual ~Mesh() = default;

        /**
         * @brief Get the interleaved vertices
         * @return the vertices
         */
        [[nodiscard]] const std::vector<Vertex>& GetVertices() const;

        /**
         * @brief Get the vertex indices
         * @return the indices
         */
        [[nodiscard]] const std::vector<uint32>& GetIndices() const;

    protected:
        std::vector<Vertex> vertices_;
        std::vector<uint32> indices_;

    }; // class Mesh

} // namespace zero::render