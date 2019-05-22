#pragma once

#include <string>
#include <vector>
#include "WindowConfig.hpp"

namespace zero::render {

    struct RenderConfig {
        WindowConfig window_config_;
        std::vector<std::string> vertex_shader_files_;
        std::vector<std::string> fragment_shader_files_;
        std::vector<std::string> texture_files_;
    }; // struct RenderConfig

} // namespace zero::render