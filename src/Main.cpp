#include "core/Engine.hpp"
#include "render/Components.hpp"
#include <filesystem>
#include <iostream>
#include <memory>
#include "render/opengl/OpenGL.hpp"

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
    engine_config.render_system_config_ = render_system_config;
    std::shared_ptr<zero::Engine> engine = std::make_shared<Engine>(engine_config);
    engine->Initialize();

    //////////////////////////////////////////////////
    ///// Game Object Instantiation
    //////////////////////////////////////////////////
    auto& registry = engine->GetEngineCore()->GetRegistry();

    // Instantiate the camera
    auto camera_entity = registry.create();
    registry.assign<render::Camera>(camera_entity, render::Camera{render::Camera::ProjectionType::PERSPECTIVE});
    auto& camera = registry.get<render::Camera>(camera_entity);
    camera.viewport_.width_ = window_config.width_;
    camera.viewport_.height_ = window_config.height_;
    camera.position_ = math::Vec3f(0.0F, 0.0F, 15.0F);
    camera.near_clip_ = 0.1F;
    camera.far_clip_ = 100.0F;

    // Instantiate a 3D model
    auto model_entity = engine->InstantiateModel(engine_config.render_system_config_.model_files_[0]);
    auto& material = registry.get<render::Material>(model_entity);
    material.shaders_.vertex_shader_ = render_system_config.vertex_shader_files_[0];
    material.shaders_.fragment_shader_ = render_system_config.fragment_shader_files_[0];
    material.texture_map_.diffuse_map_ = render_system_config.texture_files_[0];
    auto& transform = registry.get<Transform>(model_entity);
    transform.Translate(math::Vec3f(0.0F, -2.0F, 0.0F));

    //////////////////////////////////////////////////
    ///// Engine Loop
    //////////////////////////////////////////////////
    for (int i = 0; i < 5000; ++i) {
        engine->Tick();
    }
    engine->ShutDown();
    return 0;
}