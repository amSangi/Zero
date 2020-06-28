#pragma once

#include <string>
#include <vector>
#include "engine/WindowConfig.hpp"

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
    }; // struct RenderSystemConfig

} // namespace zero