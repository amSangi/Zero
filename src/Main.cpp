#include "core/Engine.hpp"
#include "core/Transform.hpp"
#include "render/Components.hpp"
#include <filesystem>
#include <iostream>
#include <memory>
#include <SDL.h>

using namespace zero;

WindowConfig CreateWindowConfig() {
    WindowConfig window_config;
    window_config.window_flags_ = WindowFlags::NO_FLAGS;
    window_config.width_ = 800;
    window_config.height_ = 600;
    window_config.window_mode_ = WindowMode::WINDOWED;
    window_config.api_ = GraphicsAPI::OPENGL;
    window_config.refresh_rate_ = RefreshRate::SYNCHRONIZED;
    window_config.title_ = "Zero Engine Demo";
    window_config.window_icon_image_file_ = "";
    return window_config;
}

RenderSystemConfig CreateRenderSystemConfig(const WindowConfig& window_config) {
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
    return render_system_config;
}

void HandleCameraMovement(render::Camera& camera, SDL_Keycode keycode, const math::Vec3f& default_position) {
    math::Vec3f horizontal_speed = math::Vec3f(1.0F, 0.0F, 0.0F);
    math::Vec3f vertical_speed = math::Vec3f(0.0F, 1.0F, 0.0F);
    math::Vec3f forward_speed = math::Vec3f(0.0F, 0.0F, 1.0F);
    math::Radian angle = math::Radian::FromDegree(10.0F);
    math::Radian opposite_angle = math::Radian::FromDegree(-10.0F);
    switch (keycode)
    {
        case SDLK_a:
            camera.TranslateRelative(-1.0F * horizontal_speed);
            break;
        case SDLK_d:
            camera.TranslateRelative(horizontal_speed);
            break;
        case SDLK_w:
            camera.TranslateRelative(vertical_speed);
            break;
        case SDLK_s:
            camera.TranslateRelative(-1.0F * vertical_speed);
            break;
        case SDLK_z:
            camera.TranslateRelative(forward_speed);
            break;
        case SDLK_x:
            camera.TranslateRelative(-1.0F * forward_speed);
            break;
        case SDLK_q:
            camera.RollRelative(angle);
            break;
        case SDLK_e:
            camera.RollRelative(opposite_angle);
            break;
        case SDLK_UP:
            camera.PitchRelative(angle);
            break;
        case SDLK_DOWN:
            camera.PitchRelative(opposite_angle);
            break;
        case SDLK_RIGHT:
            camera.YawRelative(opposite_angle);
            break;
        case SDLK_LEFT:
            camera.YawRelative(angle);
            break;
        case SDLK_ESCAPE:
            camera.position_ = default_position;
            camera.orientation_ = math::Quaternion::Identity();
            break;
        default:
            break;
    }
}

void HandleEntityMovement(Transform& transform, SDL_Keycode keycode) {
    math::Vec3f horizontal_speed = math::Vec3f(1.0F, 0.0F, 0.0F);
    math::Vec3f vertical_speed = math::Vec3f(0.0F, 1.0F, 0.0F);
    math::Vec3f forward_speed = math::Vec3f(0.0F, 0.0F, 1.0F);
    switch (keycode)
    {
        case SDLK_KP_4:
            // Move -x
            transform.Translate(-1.0F * horizontal_speed);
            break;
        case SDLK_KP_6:
            // Move +x
            transform.Translate(horizontal_speed);
            break;
        case SDLK_KP_8:
            // Move +y
            transform.Translate(vertical_speed);
            break;
        case SDLK_KP_2:
            // Move -y
            transform.Translate(-1.0F * vertical_speed);
            break;
        case SDLK_KP_1:
            // Move -z
            transform.Translate(-1.0F * forward_speed);
            break;
        case SDLK_KP_3:
            // Move +z
            transform.Translate(forward_speed);
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    //////////////////////////////////////////////////
    ///// Engine Configuration
    //////////////////////////////////////////////////
    // region Engine Configuration
    WindowConfig window_config = CreateWindowConfig();
    RenderSystemConfig render_system_config = CreateRenderSystemConfig(window_config);
    EngineConfig engine_config;
    engine_config.render_system_config_ = render_system_config;
    std::shared_ptr<zero::Engine> engine = std::make_shared<Engine>(engine_config);
    engine->Initialize();
    // endregion

    //////////////////////////////////////////////////
    ///// Game Object Instantiation
    //////////////////////////////////////////////////
    // region Game Object Instantiation
    auto& registry = engine->GetEngineCore()->GetRegistry();

    // Instantiate the camera
    auto camera_entity = registry.create();
    registry.assign<render::Camera>(camera_entity, render::Camera{render::Camera::ProjectionType::PERSPECTIVE});
    auto& camera = registry.get<render::Camera>(camera_entity);
    auto default_camera_position = math::Vec3f(0.0F, 1.0F, 15.0F);
    camera.viewport_.width_ = window_config.width_;
    camera.viewport_.height_ = window_config.height_;
    camera.position_ = default_camera_position;
    camera.near_clip_ = 0.01F;
    camera.far_clip_ = 1000.0F;
    camera.render_bounding_volumes_ = true;

    // Instantiate a root primitive
    render::PrimitiveInstance root_primitive{};
    render::Box root_shape{};
    root_primitive.Set(root_shape);
    auto root_primitive_entity = engine->InstantiatePrimitive(root_primitive);
    auto& root_primitive_material = registry.get<render::Material>(root_primitive_entity);
    root_primitive_material.wireframe_enabled_ = false;
    root_primitive_material.visible_ = true;

    // Instantiate a primitive
    render::PrimitiveInstance primitive{};
    render::Torus shape{};
    primitive.Set(shape);
    auto primitive_entity = engine->InstantiatePrimitive(primitive);
    auto& primitive_material = registry.get<render::Material>(primitive_entity);
    primitive_material.wireframe_enabled_ = false;
    primitive_material.visible_ = false;
    // endregion

    auto& root_transform = registry.get<Transform>(root_primitive_entity);
    auto& child_transform = registry.get<Transform>(primitive_entity);
    root_transform.children_.push_back(primitive_entity);
    child_transform.parent_ = root_primitive_entity;

    //////////////////////////////////////////////////
    ///// Engine Loop
    //////////////////////////////////////////////////
    // region Engine Loop
    bool quit = false;
    SDL_Event event;
    while( !quit ){
        while( SDL_PollEvent( &event ) ){
            switch( event.type ){
                case SDL_KEYDOWN:
                    HandleCameraMovement(camera, event.key.keysym.sym, default_camera_position);
                    HandleEntityMovement(root_transform, event.key.keysym.sym);
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
        engine->Tick();
    }
    // endregion
    engine->ShutDown();
    return 0;
}