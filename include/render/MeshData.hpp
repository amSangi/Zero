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

    }; // struct Vertex

    struct MeshData
    {
        MeshData(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices);
        MeshData(MeshData&& mesh) noexcept ;
        std::vector<Vertex> vertices_;
        std::vector<uint32> indices_;
    }; // struct MeshData

} // namespace zero::render