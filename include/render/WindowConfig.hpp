#pragma once

#include <string>
#include "core/ZBase.hpp"

namespace zero::render {

    /**
     * @brief Window mode to display as
     */
    enum class WindowMode {
        WINDOW_FULLSCREEN,            //<! Display the window in full screen mode
        WINDOW_WINDOWED,              //<! Display the window in bordered window mode
        WINDOW_BORDERLESS_WINDOWED    //<! Display the window in borderless windowed mode
    }; // enum class WindowMode

    /**
     * @brief Low Level Graphics API Types
     */
    enum class GraphicsAPI {
        GRAPHICS_OPENGL,    //<! Use the OpenGL Graphics API
    }; // enum class GraphicsAPI

    /**
     * @brief The window refresh rate
     */
    enum class RefreshRate {
        REFRESH_IMMEDIATE,         //<! Immediate updates
        REFRESH_SYNCHRONIZED,      //<! Updates synchronized with the vertical retrace
        REFRESH_ADAPTIVE_VSYNC,    //<! Swap buffers immediately if vertical retrace was missed for a given frame
    }; // enum class RefreshRate

    /**
     * @brief Window configuration parameters
     */
    struct WindowConfig {
        uint32 width_;                          //<! The width of the window in pixels
        uint32 height_;                         //<! The height of the window in pixels
        WindowMode window_mode_;                //<! The window mode of the window
        GraphicsAPI api_;                       //<! The graphics API of the window
        RefreshRate refresh_rate_;              //<! The refresh rate of the window
        std::string title_;                     //<! The title of the window
        std::string window_icon_image_file_;    //<! Fully qualified filename for the window icon
    }; // struct WindowConfig

} // namespace zero::render