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
         * @brief Invalid Bone ID
         */
        static constexpr int32 kInvalidBoneID = -1;

        Vertex();

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

        /**
         * @brief The bone IDs influencing the vertex.
         *
         * The bone weight and ID at the same index are associated with the same bone.
         */
        math::Vec4i bone_ids_;

        /**
         * @brief The bone weights influencing the vertex
         *
         * If the vertex is affected by weights, the sum is equal to 1. Otherwise, the sum is 0.
         * Each weight is in the range [0, 1].
         * The bone weight and ID at the same index are associated with the same bone.
         */
        math::Vec4f bone_weights_;

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
        const std::vector<Vertex>& GetVertices() const;

        /**
         * @brief Get the vertex indices
         * @return the indices
         */
        const std::vector<uint32>& GetIndices() const;

    protected:
        std::vector<Vertex> vertices_;
        std::vector<uint32> indices_;

    }; // class Mesh

} // namespace zero::render