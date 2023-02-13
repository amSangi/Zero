#include "engine/Engine.hpp"
#include "core/Logger.hpp"
#include "component/Camera.hpp"
#include <memory>
#include <queue>

using namespace zero;

class DemoGameSystem : public GameSystem
{
public:
    DemoGameSystem(EngineCore* engine_core,
                   IEntityInstantiator* entity_instantiator,
                   EngineConfig engine_config)
    : GameSystem(engine_core, entity_instantiator)
    , engine_config_(std::move(engine_config))
    , camera_entity_(NullEntity)
    , entity_to_move_(NullEntity)
    , default_camera_position_(math::Vec3f(0.0F, 1.0F, 15.0F))
    , horizontal_speed_(math::Vec3f(1.0F, 0.0F, 0.0F))
    , vertical_speed_(math::Vec3f(0.0F, 1.0F, 0.0F))
    , forward_speed_(math::Vec3f(0.0F, 0.0F, 1.0F))
    , angle_(math::Radian::FromDegree(10.0F))
    , opposite_angle_(math::Radian::FromDegree(-10.0F))
    {
    }

    ~DemoGameSystem() override = default;

    void Initialize() override
    {
        SetupCamera();
        SetupSkyDome();
        GetCore()->GetEventBus().RegisterKeyPressedListener(this);
    }

    void PreUpdate() override {}
    void Update(const TimeDelta& time_delta) override {}
    void PostUpdate() override {}
    void ShutDown() override {}

    void OnKeyPressed(KeyCode key_code) override
    {
        auto& registry = GetCore()->GetRegistry();
        Camera& camera = registry.get<Camera>(camera_entity_);
        Transform& transform = registry.get<Transform>(entity_to_move_);
        switch (key_code)
        {
            case KeyCode::A:
                camera.TranslateRelative(-1.0F * horizontal_speed_);
                break;
            case KeyCode::D:
                camera.TranslateRelative(horizontal_speed_);
                break;
            case KeyCode::W:
                camera.TranslateRelative(vertical_speed_);
                break;
            case KeyCode::S:
                camera.TranslateRelative(-1.0F * vertical_speed_);
                break;
            case KeyCode::Z:
                camera.TranslateRelative(forward_speed_);
                break;
            case KeyCode::X:
                camera.TranslateRelative(-1.0F * forward_speed_);
                break;
            case KeyCode::Q:
                camera.RollRelative(angle_);
                break;
            case KeyCode::E:
                camera.RollRelative(opposite_angle_);
                break;
            case KeyCode::UP:
                camera.PitchRelative(angle_);
                break;
            case KeyCode::DOWN:
                camera.PitchRelative(opposite_angle_);
                break;
            case KeyCode::RIGHT:
                camera.YawRelative(opposite_angle_);
                break;
            case KeyCode::LEFT:
                camera.YawRelative(angle_);
                break;
            case KeyCode::ESCAPE:
                camera.position_ = default_camera_position_;
                camera.orientation_ = math::Quaternion::Identity();
                break;
            case KeyCode::NUM_PAD_4:
                // Move -x
                transform.position_ += (-1.0F * horizontal_speed_);
                break;
            case KeyCode::NUM_PAD_6:
                // Move +x
                transform.position_ += horizontal_speed_;
                break;
            case KeyCode::NUM_PAD_8:
                // Move +y
                transform.position_ += vertical_speed_;
                break;
            case KeyCode::NUM_PAD_2:
                // Move -y
                transform.position_ += -1.0F * vertical_speed_;
                break;
            case KeyCode::NUM_PAD_1:
                // Move -z
                transform.position_ += -1.0F * forward_speed_;
                break;
            case KeyCode::NUM_PAD_3:
                // Move +z
                transform.position_ += forward_speed_;
                break;
            default:
                break;
        }
    }

private:
    void SetupCamera()
    {
        entt::registry& registry = GetCore()->GetRegistry();

        // Instantiate the camera
        auto camera_entity = registry.create();
        registry.emplace<Camera>(camera_entity, Camera{Camera::ProjectionType::PERSPECTIVE});
        auto& camera = registry.get<Camera>(camera_entity);
        camera.viewport_.width_ = engine_config_.render_system_config_.window_config_.width_;
        camera.viewport_.height_ = engine_config_.render_system_config_.window_config_.height_;
        camera.position_ = default_camera_position_;
        camera.near_clip_ = 1.0F;
        camera.far_clip_ = 1000.0F;
        camera.render_bounding_volumes_ = false;

        camera_entity_ = camera_entity;
    }
    void SetupSkyDome()
    {
        entt::registry& registry = GetCore()->GetRegistry();

        // Instantiate the sky dome
        auto sky_dome_entity = GetInstantiator()->InstantiateSkyDome(SkyDome{});
        SkyDome& sky_dome = registry.get<SkyDome>(sky_dome_entity);
        sky_dome.is_active_ = true;
        sky_dome.SetShaders("sky_dome.vertex.glsl", "sky_dome.fragment.glsl");
        sky_dome.apex_color_ = math::Vec3f(0.0F, 0.15F, 0.66F);
        sky_dome.center_color_ = math::Vec3f(0.3F, 0.45F, 0.96F);
    }

    EngineConfig engine_config_;
    Entity camera_entity_;
    Entity entity_to_move_;
    math::Vec3f default_camera_position_;
    math::Vec3f horizontal_speed_;
    math::Vec3f vertical_speed_;
    math::Vec3f forward_speed_;
    math::Radian angle_;
    math::Radian opposite_angle_;
};

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