#pragma once

#include <SDL_video.h>
#include "core/WindowConfig.hpp"

namespace zero {
namespace event {
    // Forward declaration
    class EventBus;
} // namespace event

namespace render {

    /**
     * @brief SDL Window and SDL GL Context Wrapper
     */
    class Window {
    public:

        /**
         * @brief Explicit Constructor
         * @param config The window configuration parameters
         */
        explicit Window(WindowConfig config);

        ~Window();

        /**
         * @brief Create and setup the Window and GL Context
         */
        void Initialize();

        /**
         * @brief Change the existing Window and GL context.
         *   Create a new Window and GL Context if they do not exist.
         * @param config The new Window configuration parameters
         */
        void Reinitialize(WindowConfig config);

        /**
         * @brief Swap the buffer to display the current contents of the buffer
         */
        void SwapBuffers();

        /**
         * @brief Clean up the Window and GL Context
         */
        void Cleanup();

    private:

        /**
         * @brief The Window configuration
         */
        WindowConfig config_;

        /**
         * @brief The SDL Window
         */
        SDL_Window* sdl_window_;

        /**
         * @brief The SDL OpenGL Context
         */
        SDL_GLContext sdl_gl_context_;

    }; // class Window

} // namespace render
} // namespace zero