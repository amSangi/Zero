#include "core/Engine.hpp"
#include <memory>

using namespace zero;

int main(int argc, char *argv[]) {
    //////////////////////////////////////////////////
    ///// Window Configuration
    //////////////////////////////////////////////////
    WindowConfig window_config;
    window_config.window_flags_ = WindowFlags::NO_FLAGS;
    window_config.width_ = 800;
    window_config.height_ = 600;
    window_config.window_mode_ = WindowMode::BORDERLESS_WINDOWED;
    window_config.api_ = GraphicsAPI::OPENGL;
    window_config.refresh_rate_ = RefreshRate::SYNCHRONIZED;
    window_config.title_ = "Zero Engine Demo";
    window_config.window_icon_image_file_ = "";

    //////////////////////////////////////////////////
    ///// Render System Configuration
    //////////////////////////////////////////////////
    RenderSystemConfig render_system_config;
    render_system_config.window_config_ = std::move(window_config);
    // Vertex Shaders
    // Fragment Shaders
    // Texture Files
    // Model Files

    //////////////////////////////////////////////////
    ///// Engine Configuration
    //////////////////////////////////////////////////
    EngineConfig engine_config;
    engine_config.render_system_config_ = std::move(render_system_config);
    std::shared_ptr<zero::Engine> engine = std::make_shared<Engine>(engine_config);

    //////////////////////////////////////////////////
    ///// Game Object Instantiation
    //////////////////////////////////////////////////


    //////////////////////////////////////////////////
    ///// Engine Loop
    //////////////////////////////////////////////////
    engine->Initialize();
    for (int i = 0; i < 1000; ++i) {
        engine->Tick();
    }
    engine->ShutDown();
    return 0;
}