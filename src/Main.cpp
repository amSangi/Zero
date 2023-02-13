#include "engine/Engine.hpp"
#include "core/Logger.hpp"
#include "DemoGameSystem.hpp"
#include <memory>

using namespace zero;

int main(int /* argc */, char** /* argv */)
{
    //////////////////////////////////////////////////
    ///// Engine Configuration
    //////////////////////////////////////////////////
    WindowConfig window_config;
    window_config.window_flags_ = WindowFlags::NO_FLAGS;
    window_config.width_ = 800;
    window_config.height_ = 600;
    window_config.window_mode_ = WindowMode::WINDOWED;
    window_config.api_ = GraphicsAPI::OPENGL;
    window_config.refresh_rate_ = RefreshRate::SYNCHRONIZED;
    window_config.title_ = "Zero Engine Demo";
    window_config.window_icon_image_file_ = "";
    RenderSystemConfig render_system_config{};
    render_system_config.window_config_ = window_config;
    EngineConfig engine_config;
    engine_config.render_system_config_ = render_system_config;

    //////////////////////////////////////////////////
    ///// Engine Loop
    //////////////////////////////////////////////////
    auto engine = std::make_unique<Engine>(engine_config);
    engine->AddGameSystem<DemoGameSystem>(engine_config);
    engine->Initialize();

    Logger::GetLogger().SetFilter(Logger::Level::LEVEL_VERBOSE);
    while(!engine->IsDone())
    {
        engine->Tick();
    }
    engine->ShutDown();
    return 0;
}