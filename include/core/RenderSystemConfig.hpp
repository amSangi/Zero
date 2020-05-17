#pragma once

#include <string>
#include <vector>
#include "WindowConfig.hpp"

namespace zero
{

    /**
     * @brief RenderSystem Configuration Parameters
     */
    struct RenderSystemConfig
    {

        /**
         * @brief The Window Configuration
         */
        WindowConfig window_config_;

        /**
         * @brief The fully qualified vertex shader files to use for rendering
         */
        std::vector<std::string> vertex_shader_files_;

        /**
         * @brief The fully qualified fragment shader files to use for rendering
         */
        std::vector<std::string> fragment_shader_files_;

        /**
         * @brief The fully qualified texture files to use for rendering
         */
        std::vector<std::string> texture_files_;

        /**
         * @brief The fully qualified 3D model files to use
         */
        std::vector<std::string> model_files_;

    }; // struct RenderSystemConfig

} // namespace zero