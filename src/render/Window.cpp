#include <utility>

#include <SDL.h>
#include <SDL_image.h>
#include "render/Window.hpp"

using namespace zero::render;

Window::Window(WindowConfig config)
: config_(std::move(config))
, sdl_window_(nullptr)
, sdl_gl_context_(nullptr)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
}

Window::~Window()
{
    Cleanup();
}

void Window::Initialize() {

    // Get SDL window flags
    uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    switch (config_.window_mode_) {
        case WindowMode::WINDOW_FULLSCREEN:
            window_flags |= SDL_WINDOW_FULLSCREEN;
            break;
        case WindowMode::WINDOW_BORDERLESS_WINDOWED:
            window_flags |= SDL_WINDOW_BORDERLESS;
            break;
        default:
            // Default value is windowed (0)
            break;
    }

    if (sdl_window_ == nullptr) {
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        sdl_window_ = SDL_CreateWindow(config_.title_.c_str(),
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       config_.width_,
                                       config_.height_,
                                       window_flags);
        sdl_gl_context_ = SDL_GL_CreateContext(sdl_window_);
        SDL_GL_MakeCurrent(sdl_window_, sdl_gl_context_);
    }
    else {
        SDL_SetWindowSize(sdl_window_, config_.width_, config_.height_);
        SDL_SetWindowTitle(sdl_window_, config_.title_.c_str());
        SDL_SetWindowFullscreen(sdl_window_, window_flags);
    }

    // Set the window icon
    if (!config_.window_icon_image_file_.empty()) {
        SDL_SetWindowIcon(sdl_window_, IMG_Load(config_.window_icon_image_file_.c_str()));
    }

    // Set the window refresh rate
    int error_code = 0;
    switch (config_.refresh_rate_) {
        case RefreshRate::REFRESH_SYNCHRONIZED:
            error_code = SDL_GL_SetSwapInterval(1);
            break;
        case RefreshRate::REFRESH_ADAPTIVE_VSYNC:
            error_code = SDL_GL_SetSwapInterval(-1);
            break;
        default:
            // Default to immediate updates
            error_code = SDL_GL_SetSwapInterval(0);
            break;
    }

    if (error_code) SDL_GL_SetSwapInterval(0);
}

void Window::Reinitialize(WindowConfig config) {
    config_ = std::move(config);
    Initialize();
}

void Window::SwapBuffers() {
    SDL_GL_SwapWindow(sdl_window_);
}

void Window::Cleanup() {
    if (sdl_gl_context_) {
        SDL_GL_DeleteContext(sdl_gl_context_);
        sdl_gl_context_ = nullptr;
    }

    if (sdl_window_) {
        SDL_DestroyWindow(sdl_window_);
        SDL_Quit();
        sdl_window_ = nullptr;
    }
}