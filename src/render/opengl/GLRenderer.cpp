#include "render/opengl/GLRenderer.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/opengl/GLDefaultShader.hpp"
#include "render/MeshGenerator.hpp"
#include "render/IViewVolume.hpp"
#include "render/Components.hpp"
#include "render/Optimizer.hpp"
#include "core/Transform.hpp"
#include <fstream>
#include <vector>
#include <iostream>

using namespace zero::render;

// Lighting Constants
constexpr zero::uint32 kMaxDirectionalLights = 8;
constexpr zero::uint32 kMaxPointLights = 8;
constexpr zero::uint32 kMaxSpotLights = 8;

GLRenderer::GLRenderer()
: graphics_compiler_(std::make_unique<GLCompiler>())
, model_manager_(std::make_unique<GLModelManager>())
, primitive_manager_(std::make_unique<GLPrimitiveMeshManager>())
, texture_manager_(std::make_unique<GLTextureManager>())
{}

GLRenderer::~GLRenderer() {
    ShutDown();
}

void GLRenderer::Initialize(const RenderSystemConfig& config) {
    ShutDown();
    InitializeGL();
    InitializeShaders(config);
    InitializeModels(config);
    InitializeImages(config);
    primitive_manager_->LoadPrimitives();
}

void GLRenderer::Render(const entt::registry& registry) {
    auto camera_view = registry.view<const Camera>();
    for (auto camera_entity : camera_view) {
        const auto& camera = camera_view.get(camera_entity);
        RenderEntities(camera, registry);
    }
}

void GLRenderer::PostRender(entt::registry& registry) {
    graphics_compiler_->ClearPrograms();
    texture_manager_->UnloadImages();
}

void GLRenderer::ShutDown() {
    graphics_compiler_->ClearShaders();
    model_manager_->ClearModels();
    primitive_manager_->ClearPrimitives();
    texture_manager_->ClearImages();
}

std::weak_ptr<IModel> GLRenderer::GetModel(const std::string& model) {
    return model_manager_->GetModel(model);
}

//////////////////////////////////////////////////
///// Helpers
//////////////////////////////////////////////////
void GLRenderer::InitializeShaders(const RenderSystemConfig& config) {
    InitializeShaderFiles(config.vertex_shader_files_, IShader::Type::VERTEX_SHADER);
    InitializeShaderFiles(config.fragment_shader_files_, IShader::Type::FRAGMENT_SHADER);
    if (!graphics_compiler_->InitializeShader(GLDefaultShader::kVertexShader)) {
        std::cout << "Failed to initialize default vertex shader" << std::endl;
    }
    if (!graphics_compiler_->InitializeShader(GLDefaultShader::kFragmentShader)) {
        std::cout << "Failed to initialize default fragment shader" << std::endl;
    }
}

void GLRenderer::InitializeShaderFiles(const std::vector<std::string>& shaders, IShader::Type shader_type) {
    for (const auto& shader_file : shaders) {
        ShaderStage stage;
        stage.type_ = shader_type;
        stage.name_ = shader_file;

        // Read source
        ReadShaderSource(shader_file, stage.source_);
        if (stage.source_.empty()) {
            continue;
        }

        if (!graphics_compiler_->InitializeShader(stage)) {
            std::cout << "Failed to initialize shader: " << shader_file << std::endl;
        }
    }
}

void GLRenderer::InitializeModels(const RenderSystemConfig& config) {
    for (const auto& model_file : config.model_files_) {
        model_manager_->LoadModel(model_file);
    }
}

void GLRenderer::InitializeImages(const RenderSystemConfig& config) {
    for (const auto& texture_file : config.texture_files_) {
        texture_manager_->InitializeImage(texture_file);
    }
    auto texture_sampler = std::make_shared<GLSampler>();
    texture_sampler->Initialize();
    texture_sampler->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    texture_sampler->SetMagnificationFilter(ISampler::Filter::LINEAR);
    texture_sampler->SetWrappingS(ISampler::Wrapping::REPEAT);
    texture_sampler->SetWrappingT(ISampler::Wrapping::REPEAT);
    for (int i = 0; i < texture_manager_->GetTextureUnitCount(); ++i) {
        texture_manager_->SetSampler(texture_sampler, i);
    }
}

void GLRenderer::RenderEntities(const Camera& camera, const entt::registry& registry) {
    UpdateGL(camera);
    const auto projection_matrix = camera.GetProjectionMatrix();
    const auto view_matrix = camera.GetViewMatrix();
    auto viewable_entities = Optimizer::ExtractRenderableEntities(camera, registry);
    auto renderable_view = registry.view<const Transform, const Material, const Volume>();
    auto model_view = registry.view<const ModelInstance>();
    auto primitive_view = registry.view<const PrimitiveInstance>();
    for (auto viewable_entity : viewable_entities) {
        const auto& transform = renderable_view.get<const Transform>(viewable_entity);
        const auto& material = renderable_view.get<const Material>(viewable_entity);
        const auto& volume = renderable_view.get<const Volume>(viewable_entity);
        if (camera.render_bounding_volumes_) {
            RenderVolume(camera, projection_matrix, view_matrix, volume);
        }
        ToggleWireframeMode(material.wireframe_enabled_);

        // Shader Program
        auto graphics_program = graphics_compiler_->CreateProgram(material);
        if (!graphics_program) {
            // TODO: Add error logging
            std::cout << "Failed to create graphics program" << std::endl;
            continue;
        }
        graphics_program->Use();

        // Set Material Uniforms
        auto gl_textures = texture_manager_->CreateTextureMap(material);
        for (int i = 0; i < gl_textures.size(); ++i) {
            const auto& gl_texture = gl_textures[i];
            GLenum texture_unit = GL_TEXTURE0 + i;
            gl_texture->GenerateMipMap(texture_unit);
            gl_texture->Bind(texture_unit);
            graphics_program->SetUniform("material." + gl_texture->GetName(), i);
        }
        graphics_program->SetUniform("material.specular_exponent", material.specular_exponent_);
        graphics_program->SetUniform("material.diffuse_color", material.diffuse_color_);

        // Set lights
        SetLightUniforms(graphics_program, registry);

        // Set matrices and camera position uniforms
        auto model_view_matrix = view_matrix * transform.GetLocalToWorldMatrix();
        graphics_program->SetUniform("projection_matrix", projection_matrix);
        graphics_program->SetUniform("model_view_matrix", model_view_matrix);
        graphics_program->SetUniform("normal_matrix", model_view_matrix.Inverse().Transpose().GetMatrix3x3());
        graphics_program->SetUniform("camera_position", camera.position_);
        graphics_program->FlushUniforms();

        // Draw Mesh
        if (registry.has<ModelInstance>(viewable_entity)) {
            const auto& model_instance = model_view.get(viewable_entity);
            auto model = model_manager_->GetModel(model_instance);
            if (model) {
                model->Draw();
            }
        }
        else {
            const auto& primitive_instance = primitive_view.get(viewable_entity);
            auto primitive = primitive_manager_->GetPrimitiveMesh(primitive_instance);
            if (primitive) {
                primitive->Draw();
            }
        }

        graphics_program->Finish();
    }
}

void GLRenderer::RenderVolume(const Camera& camera,
                              const math::Matrix4x4& projection_matrix,
                              const math::Matrix4x4& view_matrix,
                              const Volume& volume) {
    ToggleWireframeMode(true);

    auto model_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(volume.bounding_volume_.radius_))
            .Translate(volume.bounding_volume_.center_);
    // use default shaders
    auto gl_primitive_program = graphics_compiler_->CreateProgram(Material{});
    gl_primitive_program->Use();
    auto model_view_matrix = view_matrix * model_matrix;
    gl_primitive_program->SetUniform("projection_matrix", projection_matrix);
    gl_primitive_program->SetUniform("model_view_matrix", model_view_matrix);
    gl_primitive_program->SetUniform("normal_matrix", model_view_matrix.Inverse().Transpose().GetMatrix3x3());
    gl_primitive_program->SetUniform("camera_position", camera.position_);
    gl_primitive_program->SetUniform("material.specular_exponent", 32.0F);
    gl_primitive_program->SetUniform("material.diffuse_color", math::Vec3f(1.0F, 0.0F, 0.0F));
    gl_primitive_program->FlushUniforms();

    PrimitiveInstance primitive_instance{};
    primitive_instance.Set(render::Sphere());
    auto gl_primitive = primitive_manager_->GetPrimitiveMesh(primitive_instance);
    gl_primitive->Draw();
    gl_primitive_program->Finish();
}

//////////////////////////////////////////////////
///// Static Helpers
//////////////////////////////////////////////////
void GLRenderer::InitializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    glDepthFunc(GL_LEQUAL);
}

void GLRenderer::UpdateGL(const Camera& camera) {
    glViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLRenderer::ToggleWireframeMode(bool enable_wireframe) {
    if (enable_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void GLRenderer::SetLightUniforms(std::shared_ptr<IProgram>& graphics_program, const entt::registry& registry) {
    // Uniform Array Index - Always reset to 0 before each loop
    uint32 i = 0;
    int32 light_count = 0;

    // Set Directional Light Uniforms
    auto directional_light_view = registry.view<const DirectionalLight>();
    light_count = static_cast<int32>(math::Min(kMaxDirectionalLights, directional_light_view.size()));
    graphics_program->SetUniform("directional_light_count", light_count);
    for (auto entity : directional_light_view) {
        if (i >= kMaxDirectionalLights) {
            break;
        }
        const auto& directional_light = directional_light_view.get(entity);
        // Set Directional Light Uniform
        std::string prefix = "directional_lights[" + std::to_string(i) + "].";
        graphics_program->SetUniform(prefix + "color", directional_light.color_);
        graphics_program->SetUniform(prefix + "direction", directional_light.direction_);
        graphics_program->SetUniform(prefix + "intensity", directional_light.intensity_);

        ++i;
    }

    // Set Point Light Uniforms
    i = 0;
    auto point_light_view = registry.view<const Transform, const PointLight>();
    light_count = static_cast<int32>(math::Min(kMaxPointLights, point_light_view.size()));
    graphics_program->SetUniform("point_light_count", light_count);
    for (auto entity: point_light_view) {
        if (i >= kMaxPointLights) {
            break;
        }
        const auto& transform = point_light_view.get<const Transform>(entity);
        const auto& point_light = point_light_view.get<const PointLight>(entity);
        // Set Point Light Uniform
        std::string prefix = "point_lights[" + std::to_string(i) + "].";
        graphics_program->SetUniform(prefix + "color", point_light.color_);
        graphics_program->SetUniform(prefix + "position", transform.GetPosition());
        graphics_program->SetUniform(prefix + "attenuation_constant", point_light.attenuation_constant_);
        graphics_program->SetUniform(prefix + "attenuation_linear", point_light.attenuation_linear_);
        graphics_program->SetUniform(prefix + "attenuation_quadratic", point_light.attenuation_quadratic_);

        ++i;
    }

    // Set Spot Light Uniforms
    i = 0;
    auto spot_light_view = registry.view<const Transform, const SpotLight>();
    light_count = static_cast<int32>(math::Min(kMaxSpotLights, spot_light_view.size()));
    graphics_program->SetUniform("spot_light_count", light_count);
    for (auto entity: spot_light_view) {
        if (i >= kMaxSpotLights) {
            break;
        }
        const auto& transform = spot_light_view.get<const Transform>(entity);
        const auto& spot_light = spot_light_view.get<const SpotLight>(entity);
        // Set Spot Light Uniform
        std::string prefix = "spot_lights[" + std::to_string(i) + "].";
        graphics_program->SetUniform(prefix + "color", spot_light.color_);
        graphics_program->SetUniform(prefix + "position", transform.GetPosition());
        graphics_program->SetUniform(prefix + "direction", transform.GetOrientation() * math::Vec3f::Down());
        graphics_program->SetUniform(prefix + "inner_cosine", math::Cos(spot_light.inner_cone_angle_.ToRadian().rad_));
        graphics_program->SetUniform(prefix + "outer_cosine", math::Cos(spot_light.outer_cone_angle_.ToRadian().rad_));

        ++i;
    }
}

void GLRenderer::ReadShaderSource(const std::string& filename, std::string& destination) {
    std::ifstream input_file(filename);
    if (!input_file) return;
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    destination = buffer.str();
}