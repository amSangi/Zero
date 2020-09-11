#include "render/opengl/GLUniformManager.hpp"
#include "component/Light.hpp"
#include "component/Material.hpp"
#include "component/Transform.hpp"

namespace zero::render
{
//////////////////////////////////////////////////
///// Constants
//////////////////////////////////////////////////

// General light type counts
const uint32 GLUniformManager::kMaxDirectionalLights = 4U;
const uint32 GLUniformManager::kMaxPointLights = 4U;
const uint32 GLUniformManager::kMaxSpotLights = 4U;

// Binding indices
const uint32 GLUniformManager::kCameraBindingIndex = 0U;
const uint32 GLUniformManager::kMaterialBindingIndex = 1U;
const uint32 GLUniformManager::kModelBindingIndex = 2U;
const uint32 GLUniformManager::kLightInfoBindingIndex = 3U;
const uint32 GLUniformManager::kDirectionalLightBindingIndex = 4U;
const uint32 GLUniformManager::kPointLightBindingIndex = 5U;
const uint32 GLUniformManager::kSpotLightBindingIndex = 6U;
const uint32 GLUniformManager::kShadowMapMatrixBindingIndex = 7U;

//////////////////////////////////////////////////
///// Uniform Data Structures
//////////////////////////////////////////////////
struct alignas(16) CameraData
{
    CameraData(const math::Matrix4x4& projection_matrix, const math::Matrix4x4& view_matrix, const math::Vec3f& camera_position)
    : projection_matrix_(projection_matrix)
    , view_matrix_(view_matrix)
    , camera_position_(camera_position.x_,
                       camera_position.y_,
                       camera_position.z_,
                       1.0F)
    {
    }

    math::Matrix4x4 projection_matrix_;
    math::Matrix4x4 view_matrix_;
    math::Vec4f camera_position_;
};

struct alignas(16) MaterialData
{
    explicit MaterialData(const Material& material)
    : diffuse_color_(material.diffuse_color_.x_,
                     material.diffuse_color_.y_,
                     material.diffuse_color_.z_,
                     1.0F)
    , specular_intensity_(material.specular_intensity_)
    , specular_exponent_(material.specular_exponent_)
    {
    }

    math::Vec4f diffuse_color_;
    float specular_intensity_;
    float specular_exponent_;
};

struct alignas(16) ModelData
{
    ModelData(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix)
    : model_matrix_(model_matrix)
    , normal_matrix_(normal_matrix)
    {
    }

    math::Matrix4x4 model_matrix_;
    math::Matrix4x4 normal_matrix_;
};

struct alignas(16) LightInformationData
{
    LightInformationData(uint32 directional_light_count, uint32 point_light_count, uint32 spot_light_count)
    : directional_light_count_(directional_light_count)
    , point_light_count_(point_light_count)
    , spot_light_count_(spot_light_count)
    {
    }

    uint32 directional_light_count_;
    uint32 point_light_count_;
    uint32 spot_light_count_;
};

struct alignas(16) DirectionalLightData
{
    explicit DirectionalLightData(const DirectionalLight& light)
    : color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
    , direction_(light.direction_.x_, light.direction_.y_, light.direction_.z_, 0.0F)
    , ambient_intensity_(light.ambient_intensity_)
    , diffuse_intensity_(light.diffuse_intensity_)
    , padding_()
    {
    }

    math::Vec4f color_;
    math::Vec4f direction_;
    float ambient_intensity_;
    float diffuse_intensity_;
    float padding_[2];
};

struct alignas(16) PointLightData
{
    PointLightData(const Transform& transform, const PointLight& light)
    : position_(transform.GetPosition().x_,
                transform.GetPosition().y_,
                transform.GetPosition().z_,
                0.0F)
    , color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
    , ambient_intensity_(light.ambient_intensity_)
    , diffuse_intensity_(light.diffuse_intensity_)
    , attenuation_constant_(light.attenuation_.constant_)
    , attenuation_linear_(light.attenuation_.linear_)
    , attenuation_quadratic_(light.attenuation_.quadratic_)
    , padding_()
    {
    }

    math::Vec4f position_;
    math::Vec4f color_;
    float ambient_intensity_;
    float diffuse_intensity_;
    float attenuation_constant_;
    float attenuation_linear_;
    float attenuation_quadratic_;
    float padding_[3];
};

struct alignas(16) SpotLightData
{
    SpotLightData(const Transform& transform, const SpotLight& light)
    : position_(transform.GetPosition().x_,
                transform.GetPosition().y_,
                transform.GetPosition().z_,
                0.0F)
    , color_(light.color_.x_, light.color_.y_, light.color_.z_, 1.0F)
    , direction_(light.direction_.x_, light.direction_.y_, light.direction_.z_, 0.0F)
    , ambient_intensity_(light.ambient_intensity_)
    , diffuse_intensity_(light.diffuse_intensity_)
    , inner_cosine_(math::Cos(light.inner_cone_angle_.ToRadian().rad_))
    , outer_cosine_(math::Cos(light.outer_cone_angle_.ToRadian().rad_))
    , attenuation_constant_(light.attenuation_.constant_)
    , attenuation_linear_(light.attenuation_.linear_)
    , attenuation_quadratic_(light.attenuation_.quadratic_)
    , padding_()
    {
    }

    math::Vec4f position_;
    math::Vec4f color_;
    math::Vec4f direction_;
    float ambient_intensity_;
    float diffuse_intensity_;
    float inner_cosine_;
    float outer_cosine_;
    float attenuation_constant_;
    float attenuation_linear_;
    float attenuation_quadratic_;
    float padding_[1];
};

//////////////////////////////////////////////////
///// GLUniformManager
//////////////////////////////////////////////////

GLUniformManager::GLUniformManager()
: camera_buffer_id_(0)
, material_buffer_id_(0)
, model_buffer_id_(0)
, light_info_buffer_id_(0)
, directional_light_buffer_id_(0)
, point_light_buffer_id_(0)
, spot_light_buffer_id_(0)
, shadow_map_matrix_buffer_id_(0)
{
}

GLUniformManager::~GLUniformManager()
{
    Cleanup();
}

void GLUniformManager::Cleanup()
{
    glDeleteBuffers(1, &camera_buffer_id_);
    glDeleteBuffers(1, &material_buffer_id_);
    glDeleteBuffers(1, &model_buffer_id_);
    glDeleteBuffers(1, &light_info_buffer_id_);
    glDeleteBuffers(1, &directional_light_buffer_id_);
    glDeleteBuffers(1, &point_light_buffer_id_);
    glDeleteBuffers(1, &spot_light_buffer_id_);
    glDeleteBuffers(1, &shadow_map_matrix_buffer_id_);
    glDeleteBuffers(1, &spot_light_buffer_id_);
}

//////////////////////////////////////////////////
///// Initialization
//////////////////////////////////////////////////

void GLUniformManager::Initialize()
{
    InitializeCameraUniformBuffer();
    InitializeMaterialUniformBuffer();
    InitializeModelUniformBuffer();

    InitializeLightInfoUniformBuffer();
    InitializeDirectionalLightUniformBuffer();
    InitializePointLightUniformBuffer();
    InitializeSpotLightUniformBuffer();

    InitializeShadowMapMatrixUniformBuffer();
}

void GLUniformManager::InitializeCameraUniformBuffer()
{
    glGenBuffers(1, &camera_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, camera_buffer_id_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, kCameraBindingIndex, camera_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::InitializeMaterialUniformBuffer()
{
    glGenBuffers(1, &material_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, material_buffer_id_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, kMaterialBindingIndex, material_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::InitializeModelUniformBuffer()
{
    glGenBuffers(1, &model_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, model_buffer_id_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ModelData), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, kModelBindingIndex, model_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::InitializeLightInfoUniformBuffer()
{
    glGenBuffers(1, &light_info_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, light_info_buffer_id_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightInformationData), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, kLightInfoBindingIndex, light_info_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::InitializeDirectionalLightUniformBuffer()
{
    glGenBuffers(1, &directional_light_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, directional_light_buffer_id_);
    uint32 buffer_size = (sizeof(DirectionalLightData) * GetMaxDirectionalLightCount());
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, kDirectionalLightBindingIndex, directional_light_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::InitializePointLightUniformBuffer()
{
    glGenBuffers(1, &point_light_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, point_light_buffer_id_);
    uint32 buffer_size = (sizeof(PointLightData) * GetMaxPointLightCount());
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, kPointLightBindingIndex, point_light_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::InitializeSpotLightUniformBuffer()
{
    glGenBuffers(1, &spot_light_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, spot_light_buffer_id_);
    uint32 buffer_size = (sizeof(SpotLightData) * GetMaxSpotLightCount());
    glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, kSpotLightBindingIndex, spot_light_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::InitializeShadowMapMatrixUniformBuffer()
{
    glGenBuffers(1, &shadow_map_matrix_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, shadow_map_matrix_buffer_id_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(math::Matrix4x4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, kShadowMapMatrixBindingIndex, shadow_map_matrix_buffer_id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


//////////////////////////////////////////////////
///// Update Uniforms
//////////////////////////////////////////////////

void GLUniformManager::UpdateCameraUniforms(const math::Matrix4x4& projection_matrix,
                                            const math::Matrix4x4& view_matrix,
                                            const math::Vec3f& camera_position) const
{
    // Transpose matrices to switch from row-major to column-major order
    math::Matrix4x4 transpose_projection_matrix = projection_matrix.Transpose();
    math::Matrix4x4 transpose_view_matrix = view_matrix.Transpose();

    glBindBuffer(GL_UNIFORM_BUFFER, camera_buffer_id_);
    CameraData camera_data{transpose_projection_matrix, transpose_view_matrix, camera_position};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraData), &camera_data);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateMaterialUniforms(const Material& material) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, material_buffer_id_);

    MaterialData material_data{material};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialData), &material_data);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateModelUniforms(const math::Matrix4x4& model_matrix, const math::Matrix4x4& normal_matrix) const
{
    // Transpose matrices to switch from row-major to column-major order
    math::Matrix4x4 transpose_model_matrix = model_matrix.Transpose();
    math::Matrix4x4 transpose_normal_matrix = normal_matrix.Transpose();

    glBindBuffer(GL_UNIFORM_BUFFER, model_buffer_id_);

    ModelData model_data{transpose_model_matrix, transpose_normal_matrix};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ModelData), &model_data);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateLightUniforms(const entt::registry& registry) const
{
    auto directional_light_view = registry.view<const DirectionalLight>();
    auto point_light_view = registry.view<const Transform, const PointLight>();
    auto spot_light_view = registry.view<const Transform, const SpotLight>();
    // Cap the number of lights
    auto directional_light_count = static_cast<uint32>(math::Min(kMaxDirectionalLights, directional_light_view.size()));
    auto point_light_count = static_cast<uint32>(math::Min(kMaxPointLights, point_light_view.size()));
    auto spot_light_count = static_cast<uint32>(math::Min(kMaxSpotLights, spot_light_view.size()));

    // Light Metadata
    LightInformationData light_information_data{directional_light_count, point_light_count, spot_light_count};
    glBindBuffer(GL_UNIFORM_BUFFER, light_info_buffer_id_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(light_information_data), &light_information_data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Light Data
    UpdateDirectionalLightUniforms(registry);
    UpdatePointLightUniforms(registry);
    UpdateSpotLightUniforms(registry);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateDirectionalLightUniforms(const entt::registry& registry) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, directional_light_buffer_id_);

    uint32 i = 0;
    auto directional_light_view = registry.view<const DirectionalLight>();

    std::vector<DirectionalLightData> directional_light_data{};
    directional_light_data.reserve(directional_light_view.size());

    for (Entity entity : directional_light_view)
    {
        if (i >= kMaxDirectionalLights)
        {
            break;
        }
        const auto& directional_light = directional_light_view.get(entity);
        DirectionalLightData data{directional_light};
        directional_light_data.push_back(data);
        ++i;
    }

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightData) * directional_light_data.size(), directional_light_data.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdatePointLightUniforms(const entt::registry& registry) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, point_light_buffer_id_);

    uint32 i = 0;
    auto point_light_view = registry.view<const Transform, const PointLight>();

    std::vector<PointLightData> point_light_data{};
    point_light_data.reserve(point_light_view.size());

    for (Entity entity: point_light_view)
    {
        if (i >= kMaxPointLights)
        {
            break;
        }
        const auto& transform = point_light_view.get<const Transform>(entity);
        const auto& point_light = point_light_view.get<const PointLight>(entity);
        PointLightData data{transform, point_light};
        point_light_data.push_back(data);
        ++i;
    }

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLightData) * point_light_data.size(), point_light_data.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateSpotLightUniforms(const entt::registry& registry) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, spot_light_buffer_id_);

    uint32 i = 0;
    auto spot_light_view = registry.view<const Transform, const SpotLight>();

    std::vector<SpotLightData> spot_light_data{};
    spot_light_data.reserve(spot_light_view.size());

    for (Entity entity: spot_light_view)
    {
        if (i >= kMaxSpotLights)
        {
            break;
        }
        const auto& transform = spot_light_view.get<const Transform>(entity);
        const auto& spot_light = spot_light_view.get<const SpotLight>(entity);
        SpotLightData data{transform, spot_light};
        spot_light_data.push_back(data);
        ++i;
    }

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SpotLightData) * spot_light_data.size(), spot_light_data.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLUniformManager::UpdateShadowMapMatrix(const math::Matrix4x4& light_matrix) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, shadow_map_matrix_buffer_id_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(math::Matrix4x4), &light_matrix);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

//////////////////////////////////////////////////
///// Bind Graphics Program Uniforms
//////////////////////////////////////////////////

const char* GLUniformManager::GetShadowSamplerUniformName()
{
    return "u_shadow_map";
}

const char* GLUniformManager::GetDiffuseSamplerName()
{
    return "u_diffuse_texture";
}

void GLUniformManager::BindCameraUniforms(const std::shared_ptr<GLProgram>& program)
{
    program->BindBlockIndex(program->GetUniformBlockIndex("Camera"), kCameraBindingIndex);
}

void GLUniformManager::BindMaterialUniforms(const std::shared_ptr<GLProgram>& program)
{
    program->BindBlockIndex(program->GetUniformBlockIndex("Material"), kMaterialBindingIndex);
}

void GLUniformManager::BindModelUniforms(const std::shared_ptr<GLProgram>& program)
{
    program->BindBlockIndex(program->GetUniformBlockIndex("Model"), kModelBindingIndex);
}

void GLUniformManager::BindLightUniforms(const std::shared_ptr<GLProgram>& program)
{
    program->BindBlockIndex(program->GetUniformBlockIndex("LightInformation"), kLightInfoBindingIndex);
    program->BindBlockIndex(program->GetUniformBlockIndex("DirectionalLights"), kDirectionalLightBindingIndex);
    program->BindBlockIndex(program->GetUniformBlockIndex("PointLights"), kPointLightBindingIndex);
    program->BindBlockIndex(program->GetUniformBlockIndex("SpotLights"), kSpotLightBindingIndex);
}

void GLUniformManager::BindShadowMapUniforms(const std::shared_ptr<GLProgram>& program)
{
    program->BindBlockIndex(program->GetUniformBlockIndex("ShadowMapInformation"), kShadowMapMatrixBindingIndex);
}

//////////////////////////////////////////////////
///// Max Light and Shadow Caster Counts
//////////////////////////////////////////////////

uint32 GLUniformManager::GetMaxDirectionalLightCount()
{
    return kMaxDirectionalLights;
}

uint32 GLUniformManager::GetMaxPointLightCount()
{
    return kMaxPointLights;
}

uint32 GLUniformManager::GetMaxSpotLightCount()
{
    return kMaxSpotLights;
}

} // namespace zero::render