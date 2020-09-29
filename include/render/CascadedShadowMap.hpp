#pragma once

#include <vector>
#include "component/Camera.hpp"
#include "component/Light.hpp"
#include "math/Matrix4x4.hpp"

namespace zero::render
{

    /**
     * @brief Compute cascaded shadow map information
     *
     * Computes:
     * - Light view matrix
     * - Projection matrices for each cascade
     * - Far bounds for each cascade to identify which cascade a world point belongs in
     * - Texture matrices to convert world points into shadow map texture coordinates
     */
    class CascadedShadowMap
    {
    public:

        /**
         * @brief Constructor
         * @param cascade_count the number of cascades
         * @param shadow_map_width the width of the shadow maps
         * @param shadow_map_height the height of the shadow maps
         */
        CascadedShadowMap(uint32 cascade_count, uint32 shadow_map_width, uint32 shadow_map_height);

        ~CascadedShadowMap() = default;

        /**
         * @brief Update the cascaded shadow map
         * @param camera the camera to render to
         * @param directional_light the directional light to cast shadows with
         */
        void Update(const Camera& camera, const DirectionalLight& directional_light);

        /**
         * @brief Get the cascade count
         * @return the cascade count
         */
        [[nodiscard]] uint32 GetCascadeCount() const;

        /**
         * @brief Get the light view matrix
         * @return the light view matrix
         */
        [[nodiscard]] math::Matrix4x4 GetLightViewMatrix() const;

        /**
         * @brief Get the projection matrices for each cascade
         * @return the projection matrices for each cascade
         */
        [[nodiscard]] std::vector<math::Matrix4x4> GetProjectionMatrices() const;

        /**
         * @brief Get the texture matrices for each cascade
         *
         * The texture matrix converts a world point into shadow map texture coordinates
         *
         * @return the texture matrices
         */
        [[nodiscard]] std::vector<math::Matrix4x4> GetTextureMatrices() const;

        /**
         * @brief Get the far (z) bounds for each cascade
         *
         * The far bounds are used during rendering to identify which cascade a world point belongs to
         *
         * @return the far (z) bounds
         */
        [[nodiscard]] std::vector<float> GetCascadeFarBounds() const;

    private:
        void UpdateProjectionMatrices(const Camera& camera);

        const uint32 cascade_count_;
        const uint32 shadow_map_width_;
        const uint32 shadow_map_height_;
        math::Matrix4x4 light_view_matrix_;
        std::vector<math::Matrix4x4> projection_matrices_;
        std::vector<math::Matrix4x4> texture_matrices_;
        std::vector<float> far_bounds_;
    };
} // namespace zero::render