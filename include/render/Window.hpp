#pragma once

#include <SDL_video.h>
#include "WindowConfig.hpp"

namespace zero::render {

    /**
     * @brief SDL Window and SDL GL Context Wrapper
     */
    class Window {
    public:
        explicit Window(WindowConfig config);
        ~Window();

        void Initialize();
        void Reinitialize(WindowConfig config);
        void SwapBuffers();
    private:
        void Cleanup();

        WindowConfig config_;
        SDL_Window* sdl_window_;
        SDL_GLContext sdl_gl_context_;
    }; // class Window

} // namespace zero::render