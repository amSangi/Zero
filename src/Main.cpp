#include "core/Engine.hpp"
#include "render/Components.hpp"
#include <filesystem>
#include <iostream>
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
    window_config.window_mode_ = WindowMode::WINDOWED;
    window_config.api_ = GraphicsAPI::OPENGL;
    window_config.refresh_rate_ = RefreshRate::SYNCHRONIZED;
    window_config.title_ = "Zero Engine Demo";
    window_config.window_icon_image_file_ = "";

    //////////////////////////////////////////////////
    ///// Render System Configuration
    //////////////////////////////////////////////////
    RenderSystemConfig render_system_config;
    render_system_config.window_config_ = window_config;
    std::filesystem::path resources_path{std::filesystem::current_path().append("..\\resources\\")};
    for (const auto& entry : std::filesystem::recursive_directory_iterator(resources_path)) {
        if (entry.is_regular_file()) {
            const auto& entry_path = entry.path();
            auto fully_qualified_path = entry_path.string();
            auto filename = entry_path.filename().string();
            auto folder_name = entry_path.parent_path().filename().string();
            auto extension = entry_path.extension().filename().string();

            if (extension == ".obj") {
                std::cout << "[OBJ Model]       " << filename << std::endl;
                render_system_config.model_files_.push_back(fully_qualified_path);
            }
            else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                std::cout << "[Texture]         " << filename << std::endl;
                render_system_config.texture_files_.push_back(fully_qualified_path);
            }
            else if (extension == ".glsl") {
                if (folder_name == "vertex") {
                    std::cout << "[Vertex Shader]   " << filename << std::endl;
                    render_system_config.vertex_shader_files_.push_back(fully_qualified_path);
                }
                else if (folder_name == "fragment") {
                    std::cout << "[Fragment Shader] " << filename << std::endl;
                    render_system_config.fragment_shader_files_.push_back(fully_qualified_path);
                }
            }
            else {
                std::cout << "[Unknown file]    " << filename << std::endl;
            }
        }
    }

    //////////////////////////////////////////////////
    ///// Engine Configuration
    //////////////////////////////////////////////////
    EngineConfig engine_config;
    engine_config.render_system_config_ = std::move(render_system_config);
    std::shared_ptr<zero::Engine> engine = std::make_shared<Engine>(engine_config);

    //////////////////////////////////////////////////
    ///// Game Object Instantiation
    //////////////////////////////////////////////////
    auto& registry = engine->GetCoreEngine()->GetRegistry();

    // Instantiate the camera
    render::Camera camera{render::Camera::ProjectionType::PERSPECTIVE};
    camera.position_ = math::Vec3f(0.0F, 0.0F, 50.0F);
    camera.viewport_.width_ = window_config.width_;
    camera.viewport_.height_ = window_config.height_;
    auto camera_entity = registry.create();
    registry.assign<render::Camera>(camera_entity, camera);

    // TODO: Create Illidan model instantiation
    // Instantiate a 3D model

    //////////////////////////////////////////////////
    ///// Engine Loop
    //////////////////////////////////////////////////
    engine->Initialize();
    while (true) {
        engine->Tick();
    }
    engine->ShutDown();
    return 0;
}