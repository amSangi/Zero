#pragma once

#include <vector>
#include "math/Vector2.hpp"
#include "math/Vector3.hpp"

namespace zero::render {

    /**
     * @brief Interleaved Mesh Vertex
     */
    struct Vertex {
        math::Vec3f position_;
        math::Vec3f normal_;
        math::Vec2f texture_coordinate_;
    }; // struct Vertex

    /**
     * @brief Interleaved Mesh Container
     */
    class Mesh {
    public:
        /**
         * @brief Constructor
         * @param vertices Interleaved vertices
         * @param indices Vertex indices
         */
        Mesh(std::vector<Vertex>&& vertices, std::vector<uint16>&& indices);

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
        [[nodiscard]] const std::vector<uint16>& GetIndices() const;

    protected:
        std::vector<Vertex> vertices_;
        std::vector<uint16> indices_;

    }; // class Mesh

} // namespace zero::render