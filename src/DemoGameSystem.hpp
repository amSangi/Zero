#pragma once

#include "engine/Engine.hpp"
#include "core/TransformSystem.hpp"
#include "core/Logger.hpp"
#include "component/Camera.hpp"
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
        SetupPrimitives();
        SetupLights();
        SetupModels();
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
            case KeyCode::NUMBER_1:
                // Move -x
                transform.position_ += (-1.0F * horizontal_speed_);
                break;
            case KeyCode::NUMBER_2:
                // Move +x
                transform.position_ += horizontal_speed_;
                break;
            case KeyCode::NUMBER_3:
                // Move +y
                transform.position_ += vertical_speed_;
                break;
            case KeyCode::NUMBER_4:
                // Move -y
                transform.position_ += -1.0F * vertical_speed_;
                break;
            case KeyCode::NUMBER_5:
                // Move -z
                transform.position_ += -1.0F * forward_speed_;
                break;
            case KeyCode::NUMBER_6:
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
    void SetupPrimitives()
    {
        entt::registry& registry = GetCore()->GetRegistry();

        // Instantiate a root primitive
        Torus root_shape{};
        PrimitiveInstance root_primitive{root_shape};
        Entity root_primitive_entity = GetInstantiator()->InstantiatePrimitive(root_primitive);
        Material& root_primitive_material = registry.get<Material>(root_primitive_entity);
        Material::Shaders root_primitive_material_shaders{};
        root_primitive_material_shaders.vertex_shader_ = "model.vertex.glsl";
        root_primitive_material_shaders.fragment_shader_ = "model.fragment.glsl";
        root_primitive_material.SetShaders(root_primitive_material_shaders);
        root_primitive_material.wireframe_enabled_ = false;
        root_primitive_material.diffuse_color_ = math::Vec3f(0.65F, 0.0F, 0.0F);
        root_primitive_material.visible_ = true;

        // Instantiate a child primitive
        Torus shape{};
        PrimitiveInstance primitive{shape};
        Entity primitive_entity = GetInstantiator()->InstantiatePrimitive(primitive);
        Material& primitive_material = registry.get<Material>(primitive_entity);
        Material::Shaders primitive_material_shaders{};
        primitive_material_shaders.vertex_shader_ = "model.vertex.glsl";
        primitive_material_shaders.fragment_shader_ = "model.fragment.glsl";
        primitive_material.SetShaders(primitive_material_shaders);
        primitive_material.wireframe_enabled_ = false;
        primitive_material.diffuse_color_ = math::Vec3f(0.0F, 0.65F, 0.0F);
        primitive_material.visible_ = true;

        // Incorporate parent-child hierarchy
        Transform& root_transform = registry.get<Transform>(root_primitive_entity);
        Transform& child_transform = registry.get<Transform>(primitive_entity);
        child_transform.local_orientation_ = math::Quaternion::FromAngleAxis(math::Vec3f::Up(), math::Radian::FromDegree(90.0F));
        child_transform.local_position_ = math::Vec3f(0.0F, 1.5F, 0.0F);
        root_transform.children_.push_back(primitive_entity);
        child_transform.parent_ = root_primitive_entity;

        math::Matrix4x4 transformation = math::Matrix4x4::Identity()
                .Translate(math::Vec3f(5.0F, 0.0F, 0.0F));
        TransformSystem::UpdateTransform(registry, root_primitive_entity, transformation);

        // Instantiate the floor
        Plane plane{};
        plane.width_ = 32U;
        plane.height_ = 32U;
        plane.u_axis_ = math::Vec3f::Right();
        plane.v_axis_ = math::Vec3f::Back();
        PrimitiveInstance floor_primitive{plane};
        Entity floor_entity = GetInstantiator()->InstantiatePrimitive(floor_primitive);
        Material& floor_material = registry.get<Material>(floor_entity);
        Material::Shaders floor_material_shaders{};
        floor_material_shaders.vertex_shader_ = "model.vertex.glsl";
        floor_material_shaders.fragment_shader_ = "model.fragment.glsl";
        floor_material.SetShaders(floor_material_shaders);
        floor_material.diffuse_color_ = math::Vec3f(0.52F, 0.37F, 0.26F);
        floor_material.two_sided_ = true;
        auto& floor_transform = registry.get<Transform>(floor_entity);
        floor_transform.scale_ = math::Vec3f(10.0F);
        floor_transform.position_ = math::Vec3f(-10.0F, -2.0F, 10.0F);
    }
    void SetupLights()
    {
        entt::registry& registry = GetCore()->GetRegistry();

        PrimitiveInstance sphere_primitive{Sphere{}};
        Light light{};

        // Instantiate directional light
        {
            DirectionalLight directional_light{};
            directional_light.ambient_intensity_ = 0.65F;
            directional_light.diffuse_intensity_ = 0.25F;
            directional_light.color_ = math::Vec3f(1.0F, 1.0F, 1.0F);
            directional_light.direction_ = math::Vec3f(1.0F, -1.0F, 0.0F);
            directional_light.casts_shadows_ = true;
            light.Set(directional_light);
            GetInstantiator()->InstantiateLight(light, NullEntity);
        }

        // Instantiate spotlight
        {
            Entity spot_light_entity = GetInstantiator()->InstantiatePrimitive(sphere_primitive);
            SpotLight spot_light{};
            spot_light.direction_ = math::Vec3f::Normalize(math::Vec3f(0.0F, -1.0F, 0.0F));
            spot_light.color_ = math::Vec3f(1.0F, 1.0F, 1.0F);
            spot_light.casts_shadows_ = true;
            light.Set(spot_light);
            GetInstantiator()->InstantiateLight(light, spot_light_entity);

            Material& light_primitive_material = registry.get<Material>(spot_light_entity);
            Material::Shaders light_primitive_material_shaders{};
            light_primitive_material_shaders.vertex_shader_ = "model.vertex.glsl";
            light_primitive_material_shaders.fragment_shader_ = "model_unlit.fragment.glsl";
            light_primitive_material.SetShaders(light_primitive_material_shaders);
            light_primitive_material.diffuse_color_ = math::Vec3f(1.0F, 0.0F, 0.0F);
            light_primitive_material.visible_ = true;

            auto& light_transform = registry.get<Transform>(spot_light_entity);
            light_transform.scale_ = math::Vec3f(0.1F);
            light_transform.position_ = math::Vec3f(2.0F, 5.0F, 0.0F);
            entity_to_move_ = spot_light_entity;
        }
    }
    void SetupModels()
    {
        entt::registry& registry = GetCore()->GetRegistry();
        {
            // Instantiate a 3D model
            auto model_entity = GetInstantiator()->InstantiateModel("illidan\\IllidanLegion.obj", NullEntity);
            auto& model_material = registry.get<Material>(model_entity);
            Material::Shaders shaders{};
            shaders.vertex_shader_ = "model.vertex.glsl";
            shaders.fragment_shader_ = "model.fragment.glsl";
            Material::TextureMap texture_map{};
            texture_map.diffuse_map_ = "IllidanLegion_illidan2.png";
            model_material.SetShaders(shaders);
            model_material.SetTextureMap(texture_map);
            model_material.wireframe_enabled_ = false;
            model_material.visible_ = true;
            model_material.specular_exponent_ = 32.0F;
            auto& transform = registry.get<Transform>(model_entity);
            transform.position_ = math::Vec3f(0.0F, -2.0F, 0.0F);
        }
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