#pragma once

#include <memory>
#include <vector>
#include "core/NonCopyable.hpp"
#include "math/Vector2.hpp"
#include "math/Vector3.hpp"
#include "render/Constants.hpp"

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
        static constexpr int32 kInvalidBoneID = Constants::kMaxMeshBoneCount + 1;

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

    struct MeshData
    {
        MeshData(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices);
        MeshData(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices, std::vector<std::string>&& bone_names);
        MeshData(MeshData&& mesh) noexcept ;
        std::vector<std::string> bone_names_;
        std::vector<Vertex> vertices_;
        std::vector<uint32> indices_;
    }; // struct MeshData

} // namespace zero::render