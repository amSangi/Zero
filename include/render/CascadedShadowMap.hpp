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
     * - Scene independent parallel splits for each cascade
     * - Projection matrices for each cascade
     * - Crop matrix to fit the projection matrix to its associated cascade
     * - Texture matrices to convert world points into shadow map texture coordinates
     */
    class CascadedShadowMap
    {
    public:

        /**
         * @brief Constructor
         * @param cascade_count the number of cascades
         */
        explicit CascadedShadowMap(uint32 cascade_count);

        ~CascadedShadowMap() = default;

        /**
         * @brief Set the maximum far clip for shadows
         *
         * Shadows beyond this z coordinate will not have shadows.
         * If this is not set, the camera far clip will be used
         *
         * @param maximum_far_clip the maximum z far clip
         */
        void SetMaxShadowFarClip(float maximum_far_clip);

        /**
         * @brief Update the cascaded shadow map
         * @param camera the rendering camera
         * @param directional_light the directional light to cast shadows with
         */
        void Update(const Camera& camera, const DirectionalLight& directional_light);

        /**
         * @brief Get the cascade count
         * @return the cascade count
         */
        [[nodiscard]] uint32 GetCascadeCount() const;

        /**
         * @brief Get the light view matrix for each cascade
         * @return the light view matrix for each cascade
         */
        [[nodiscard]] std::vector<math::Matrix4x4> GetLightViewMatrices() const;

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
         * @brief Get the far (z-coordinate) boundary of each cascade in camera view space
         * @return the far (z-coordinate) boundaries
         */
        [[nodiscard]] std::vector<float> GetViewFarBounds() const;

    private:
        /**
         * @brief Compute the far z boundary of each cascade
         * @param camera the rendering camera
         */
        void UpdateCascadeSplits(const Camera& camera);
        /**
         * @brief Update the light view matrices and light projection matrices
         * @param camera the rendering camera
         * @param directional_light the directional light casting the shadows
         */
        void UpdateMatrices(const Camera& camera, const DirectionalLight& directional_light);
        /**
         * @brief Compute the texture matrices using the light view and light projection matrices
         */
        void UpdateTextureMatrices();

        static const float kMaxCascadeZCoverage;
        const uint32 cascade_count_;
        std::vector<math::Matrix4x4> light_view_matrices_;
        std::vector<math::Matrix4x4> projection_matrices_;
        std::vector<math::Matrix4x4> texture_matrices_;
        std::vector<float> cascade_splits_;
        std::vector<float> view_far_bounds_;
        float maximum_far_clip_;

    }; // class CascadedShadowMap

} // namespace zero::render