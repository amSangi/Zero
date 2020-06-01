#pragma once

#include <string>
#include "core/ZeroBase.hpp"

namespace zero
{

    /**
     * @brief Window mode to display as
     */
    enum class WindowMode
    {
        FULLSCREEN,            ///< Display the window in full screen mode
        WINDOWED,              ///< Display the window in bordered window mode
        BORDERLESS_WINDOWED    ///< Display the window in borderless windowed mode
    }; // enum class WindowMode

    /**
     * @brief Low Level Graphics API Types
     */
    enum class GraphicsAPI
    {
        OPENGL,    ///< Use the OpenGL Graphics API
    }; // enum class GraphicsAPI

    /**
     * @brief The window refresh rate
     */
    enum class RefreshRate
    {
        IMMEDIATE,         ///< Immediate updates
        SYNCHRONIZED,      ///< Updates synchronized with the vertical retrace
        ADAPTIVE_VSYNC,    ///< Swap buffers immediately if vertical retrace was missed for a given frame
    }; // enum class RefreshRate


    /**
     * @brief Additional window flags
     */
    enum WindowFlags
    {
        NO_FLAGS          = 0U,        ///< No window flags
        HIDE              = 1U << 0U,  ///< Hide the window
        WINDOW_MAXIMIZED  = 1U << 1U,  ///< Maximize the window
        WINDOW_MINIMIZED  = 1U << 2U,  ///< Minimize the window
    }; // enum WindowFlags


    /**
     * @brief Window configuration parameters
     */
    struct WindowConfig
    {

        /**
         * @brief Additional window flags
         */
        uint8 window_flags_;

        /**
         * @brief The width of the window in pixels
         */
        uint32 width_;

        /**
         * @brief The height of the window in pixels
         */
        uint32 height_;

        /**
         * @brief The window mode
         */
        WindowMode window_mode_;

        /**
         * @brief The graphics API to use for rendering
         */
        GraphicsAPI api_;

        /**
         * @brief The window refresh rate
         */
        RefreshRate refresh_rate_;

        /**
         * @brief The title of the window
         */
        std::string title_;

        /**
         * @brief The fully qualified filename for the window icon
         */
        std::string window_icon_image_file_;

    }; // struct WindowConfig

} // namespace zero