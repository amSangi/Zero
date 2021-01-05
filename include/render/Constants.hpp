#pragma once

namespace zero::render
{

    /**
     * @brief Rendering and animation constants
     */
    class Constants
    {
    public:
        Constants() = delete;
        ~Constants() = delete;

        /**
         * @brief The maximum directional light count
         */
        static constexpr uint32 kMaxDirectionalLights = 4U;

        /**
         * @brief The maximum point light count
         */
        static constexpr uint32 kMaxPointLights = 4U;

        /**
         * @brief The maximum spot light count
         */
        static constexpr uint32 kMaxSpotLights = 4U;

        /**
         * @brief The number of cascades used in Cascaded Shadow Mapping
         */
        static constexpr uint32 kShadowCascadeCount = 3U;

        /**
         * @brief The maximum number of bones in a mesh
         */
        static constexpr uint32 kMaxMeshBoneCount = 64U;

        /**
         * @brief The maximum number of bone weights for a vertex
         */
        static constexpr uint32 kMaxVertexBoneWeightCount = 4U;

    }; // class Constants

} // namespace zero