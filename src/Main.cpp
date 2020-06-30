#include "engine/Engine.hpp"
#include "component/Camera.hpp"
#include "component/Light.hpp"
#include "component/PrimitiveInstance.hpp"
#include "component/SkyDome.hpp"
#include <memory>

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
        SetupModel();
        SetupPrimitives();
        SetupLights();
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
                camera.position_ = default_camera_position_;
                camera.orientation_ = math::Quaternion::Identity();
                break;
            case KeyCode::NUM_PAD_4:
                // Move -x
                transform.Translate(-1.0F * horizontal_speed_);
                break;
            case KeyCode::NUM_PAD_6:
                // Move +x
                transform.Translate(horizontal_speed_);
                break;
            case KeyCode::NUM_PAD_8:
                // Move +y
                transform.Translate(vertical_speed_);
                break;
            case KeyCode::NUM_PAD_2:
                // Move -y
                transform.Translate(-1.0F * vertical_speed_);
                break;
            case KeyCode::NUM_PAD_1:
                // Move -z
                transform.Translate(-1.0F * forward_speed_);
                break;
            case KeyCode::NUM_PAD_3:
                // Move +z
                transform.Translate(forward_speed_);
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
        registry.assign<Camera>(camera_entity, Camera{Camera::ProjectionType::PERSPECTIVE});
        auto& camera = registry.get<Camera>(camera_entity);
        camera.viewport_.width_ = engine_config_.render_system_config_.window_config_.width_;
        camera.viewport_.height_ = engine_config_.render_system_config_.window_config_.height_;
        camera.position_ = default_camera_position_;
        camera.near_clip_ = 0.01F;
        camera.far_clip_ = 1000.0F;
        camera.render_bounding_volumes_ = false;

        camera_entity_ = camera_entity;
    }
    void SetupSkyDome()
    {
        entt::registry& registry = GetCore()->GetRegistry();
        FileManager& file_manager = GetCore()->GetFileManager();
        const auto& vertex_shaders = file_manager.GetVertexFiles();
        const auto& fragment_shaders = file_manager.GetFragmentFiles();

        // Instantiate the sky dome
        auto sky_dome_entity = GetInstantiator()->InstantiateSkyDome(SkyDome{});
        SkyDome& sky_dome = registry.get<SkyDome>(sky_dome_entity);
        sky_dome.is_active_ = true;
        sky_dome.vertex_shader_ = vertex_shaders[1];
        sky_dome.fragment_shader_ = fragment_shaders[2];
        sky_dome.apex_color_ = math::Vec3f(0.0F, 0.15F, 0.66F);
        sky_dome.center_color_ = math::Vec3f(0.3F, 0.45F, 0.96F);
    }
    void SetupModel()
    {
        entt::registry& registry = GetCore()->GetRegistry();
        FileManager& file_manager = GetCore()->GetFileManager();
        const auto& vertex_shaders = file_manager.GetVertexFiles();
        const auto& fragment_shaders = file_manager.GetFragmentFiles();
        const auto& models = file_manager.GetModelFiles();
        const auto& textures = file_manager.GetTextureFiles();

        // Instantiate a 3D model
        auto model_entity = GetInstantiator()->InstantiateModel(models[0]);
        auto& model_material = registry.get<Material>(model_entity);
        model_material.shaders_.vertex_shader_ = vertex_shaders[0];
        model_material.shaders_.fragment_shader_ = fragment_shaders[0];
        model_material.texture_map_.diffuse_map_ = textures[0];
        model_material.wireframe_enabled_ = false;
        model_material.visible_ = true;
        model_material.specular_exponent_ = 32.0F;
        auto& transform = registry.get<Transform>(model_entity);
        transform.Translate(math::Vec3f(0.0F, -2.0F, 0.0F));
    }
    void SetupPrimitives()
    {
        entt::registry& registry = GetCore()->GetRegistry();
        FileManager& file_manager = GetCore()->GetFileManager();
        const auto& vertex_shaders = file_manager.GetVertexFiles();
        const auto& fragment_shaders = file_manager.GetFragmentFiles();

        // Instantiate a root primitive
        PrimitiveInstance root_primitive{};
        Torus root_shape{};
        root_primitive.Set(root_shape);
        auto root_primitive_entity = GetInstantiator()->InstantiatePrimitive(root_primitive);
        auto& root_primitive_material = registry.get<Material>(root_primitive_entity);
        root_primitive_material.shaders_.vertex_shader_ = vertex_shaders[0];
        root_primitive_material.shaders_.fragment_shader_ = fragment_shaders[0];
        root_primitive_material.wireframe_enabled_ = false;
        root_primitive_material.diffuse_color_ = math::Vec3f(0.65F, 0.0F, 0.0F);
        root_primitive_material.visible_ = true;

        // Instantiate a child primitive
        PrimitiveInstance primitive{};
        Torus shape{};
        primitive.Set(shape);
        auto primitive_entity = GetInstantiator()->InstantiatePrimitive(primitive);
        auto& primitive_material = registry.get<Material>(primitive_entity);
        primitive_material.shaders_.vertex_shader_ = vertex_shaders[0];
        primitive_material.shaders_.fragment_shader_ = fragment_shaders[0];
        primitive_material.wireframe_enabled_ = false;
        primitive_material.diffuse_color_ = math::Vec3f(0.0F, 0.65F, 0.0F);
        primitive_material.visible_ = true;

        // Incorporate parent-child hierarchy
        auto& root_transform = registry.get<Transform>(root_primitive_entity);
        auto& child_transform = registry.get<Transform>(primitive_entity);
        child_transform.LocalRotate(root_transform,
                                    math::Quaternion::FromAngleAxis(math::Vec3f::Up(), math::Radian::FromDegree(90.0F)));
        child_transform.LocalTranslate(root_transform, math::Vec3f(0.0F, 1.5F, 0.0F));
        root_transform.children_.push_back(primitive_entity);
        child_transform.parent_ = root_primitive_entity;
        root_transform.Translate(math::Vec3f(5.0F, 0.0F, 0.0F));

        // Instantiate the floor
        PrimitiveInstance floor_primitive{};
        Plane plane{};
        plane.width_ = 32U;
        plane.height_ = 32U;
        plane.u_axis_ = math::Vec3f::Right();
        plane.v_axis_ = math::Vec3f::Back();
        floor_primitive.Set(plane);
        auto floor_entity = GetInstantiator()->InstantiatePrimitive(floor_primitive);
        auto& floor_material = registry.get<Material>(floor_entity);
        floor_material.shaders_.vertex_shader_ = vertex_shaders[0];
        floor_material.shaders_.fragment_shader_ = fragment_shaders[0];
        floor_material.diffuse_color_ = math::Vec3f(0.1F);
        auto& floor_transform = registry.get<Transform>(floor_entity);
        floor_transform.Scale(math::Vec3f(10.0F));
        floor_transform.Translate(math::Vec3f(-10.0F, -3.0F, 10.0F));

        entity_to_move_ = root_primitive_entity;
    }
    void SetupLights()
    {
        entt::registry& registry = GetCore()->GetRegistry();
        FileManager& file_manager = GetCore()->GetFileManager();
        const auto& vertex_shaders = file_manager.GetVertexFiles();
        const auto& fragment_shaders = file_manager.GetFragmentFiles();

        // Instantiate directional, point, and spot lights
        PrimitiveInstance light_primitive{Sphere{}};
        auto light_entity = GetInstantiator()->InstantiatePrimitive(light_primitive);
        Light light{};
        DirectionalLight directional_light{};
        directional_light.intensity_ = 1.35F;
        directional_light.direction_ = math::Vec3f::Normalize(math::Vec3f(0.0F, -1.0F, -1.0F));
        light.Set(directional_light);
        GetInstantiator()->InstantiateLight(light, light_entity);
        auto& light_primitive_material = registry.get<Material>(light_entity);
        light_primitive_material.shaders_.vertex_shader_ = vertex_shaders[0];
        light_primitive_material.shaders_.fragment_shader_ = fragment_shaders[1];
        light_primitive_material.diffuse_color_ = math::Vec3f(1.0F, 1.0F, 1.0F);
        light_primitive_material.visible_ = false;
        auto& light_transform = registry.get<Transform>(light_entity);
        light_transform.Scale(math::Vec3f(0.1F));
        light_transform.Translate(math::Vec3f(2.0F, 5.0F, 0.0F));
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

int main(int argc, char *argv[])
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
    while(!engine->IsDone())
    {
        engine->Tick();
    }
    engine->ShutDown();
    return 0;
}