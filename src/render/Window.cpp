#include "render/Window.hpp"
#include "render/opengl/OpenGL.hpp"
#include <SDL.h>
#include <SDL_image.h>

using namespace zero::render;

Window::Window(WindowConfig config)
: config_(std::move(config))
, sdl_window_(nullptr)
, sdl_gl_context_(nullptr)
{}

Window::~Window() {
    Cleanup();
}

void Window::Initialize() {

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER);

    // Get SDL window flags
    uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    switch (config_.window_mode_) {
        case WindowMode::FULLSCREEN:
            window_flags |= SDL_WINDOW_FULLSCREEN;
            break;
        case WindowMode::BORDERLESS_WINDOWED:
            window_flags |= SDL_WINDOW_BORDERLESS;
            break;
        default:
            // Default value is windowed (0)
            break;
    }

    if (config_.window_flags_ & WindowFlags::WINDOW_MAXIMIZED) window_flags |= SDL_WINDOW_MAXIMIZED;
    if (config_.window_flags_ & WindowFlags::WINDOW_MINIMIZED) window_flags |= SDL_WINDOW_MINIMIZED;
    if (config_.window_flags_ & WindowFlags::HIDE) window_flags |= SDL_WINDOW_HIDDEN;

    if (sdl_window_ == nullptr) {
        // Create Window and OpenGL Context
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        sdl_window_ = SDL_CreateWindow(config_.title_.c_str(),
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       config_.width_,
                                       config_.height_,
                                       window_flags);
        sdl_gl_context_ = SDL_GL_CreateContext(sdl_window_);
        SDL_GL_MakeCurrent(sdl_window_, sdl_gl_context_);

        // Initialize glew after window/context creation
        glewInit();
    }
    else {
        SDL_SetWindowSize(sdl_window_, config_.width_, config_.height_);
        SDL_SetWindowTitle(sdl_window_, config_.title_.c_str());
        SDL_SetWindowFullscreen(sdl_window_, window_flags);
    }

    // Window icon
    if (!config_.window_icon_image_file_.empty()) {
        SDL_SetWindowIcon(sdl_window_, IMG_Load(config_.window_icon_image_file_.c_str()));
    }

    // Refresh rate
    int error_code = 0;
    switch (config_.refresh_rate_) {
        case RefreshRate::SYNCHRONIZED:
            error_code = SDL_GL_SetSwapInterval(1);
            break;
        case RefreshRate::ADAPTIVE_VSYNC:
            error_code = SDL_GL_SetSwapInterval(-1);
            break;
        default:
            // Default to immediate updates
            error_code = SDL_GL_SetSwapInterval(0);
            break;
    }

    // Default refresh rate during error
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
        sdl_window_ = nullptr;
    }
    SDL_Quit();
}