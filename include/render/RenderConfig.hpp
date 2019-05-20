#pragma once

#include <string>
#include <vector>
#include "WindowConfig.hpp"

namespace zero::render {

    struct RenderConfig {
        WindowConfig window_config;
        std::vector<std::string> vertex_shader_files;
        std::vector<std::string> fragment_shader_files;
        std::vector<std::string> texture_files;
    }; // struct RenderConfig

} // namespace zero::render