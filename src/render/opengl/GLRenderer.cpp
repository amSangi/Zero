#include "render/opengl/GLRenderer.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/opengl/GLPrimitiveGenerator.hpp"
#include "render/opengl/GLDefaultShader.hpp"
#include "render/MeshGenerator.hpp"
#include "render/ViewVolumeBuilder.hpp"
#include "core/Transform.hpp"
#include <fstream>
#include <vector>
#include <iostream>

using namespace zero::render;

constexpr zero::uint8 kAlphaTextureIndex = 0;
constexpr zero::uint8 kAmbientTextureIndex = 1;
constexpr zero::uint8 kDiffuseTextureIndex = 2;
constexpr zero::uint8 kDisplacementTextureIndex = 3;
constexpr zero::uint8 kNormalTextureIndex = 4;

GLRenderer::GLRenderer()
: graphics_compiler_(std::make_unique<GLCompiler>())
, model_manager_(std::make_unique<GLModelManager>())
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
}

void GLRenderer::Render(const entt::registry& registry) {
    auto camera_view = registry.view<const Camera>();
    auto renderable_view = registry.view<const Transform,
                                         const Volume,
                                         const Material,
                                         const ModelInstance>();

    for (auto camera_entity : camera_view) {
        const auto& camera = camera_view.get(camera_entity);
        RenderWithCamera(camera, registry);
    }
}

void GLRenderer::PostRender(entt::registry& registry) {
    texture_manager_->UnloadImages();
}

void GLRenderer::ShutDown() {
    graphics_compiler_->ClearShaders();
    model_manager_->ClearModels();
    texture_manager_->ClearImages();
}

zero::Component::Entity GLRenderer::InstantiateModel(entt::registry& registry, const std::string& model) {
    auto gl_model = model_manager_->GetModel(model);
    if (!gl_model) {
        // TODO: Add error logging
        return Component::NullEntity;
    }

    return GLInstantiator::InstantiateModel(registry, gl_model);
}

zero::Component::Entity GLRenderer::InstantiatePrimitive(entt::registry& registry, const PrimitiveInstance& primitive) {
    return GLInstantiator::InstantiatePrimitive(registry, primitive);
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

void GLRenderer::RenderVolume(const math::Matrix4x4& projection_matrix,
                              const math::Matrix4x4& view_matrix,
                              const Volume& volume) {
    Material default_material;
    default_material.shaders_.vertex_shader_ = GLDefaultShader::kVertexShader.name_;
    default_material.shaders_.fragment_shader_ = GLDefaultShader::kFragmentShader.name_;
    ToggleWireframeMode(true);
    auto gl_primitive_program = graphics_compiler_->CreateProgram(default_material);
    gl_primitive_program->Use();
    gl_primitive_program->SetUniform("projection_matrix", projection_matrix);
    // View Matrix * Identity Matrix = View Matrix
    gl_primitive_program->SetUniform("model_view_matrix", view_matrix);
    gl_primitive_program->FlushUniforms();

    auto gl_primitive = GLPrimitiveGenerator::Generate(volume.bounding_volume_);
    gl_primitive->Draw();
    gl_primitive_program->Finish();
}

void GLRenderer::RenderWithCamera(const Camera& camera,
                                  const entt::registry& registry) {
    // TODO: Update rendering loop to render primitives too
    const auto projection_matrix = camera.GetProjectionMatrix();
    const auto view_matrix = camera.GetViewMatrix();
    UpdateGL(camera);
    std::vector<Component::Entity> viewable_entities = GetViewableEntities(registry, camera);
    auto renderable_view = registry.view<const Transform,
                                         const Volume,
                                         const Material,
                                         const ModelInstance>();

    for (auto viewable_entity : viewable_entities) {
        const auto& model_instance = renderable_view.get<const ModelInstance>(viewable_entity);
        const auto& transform = renderable_view.get<const Transform>(viewable_entity);
        const auto& material = renderable_view.get<const Material>(viewable_entity);
        if (!material.visible_) {
            continue;
        }
        if (camera.render_bounding_volumes_) {
            const auto& volume = renderable_view.get<const Volume>(viewable_entity);
            RenderVolume(projection_matrix,
                         view_matrix,
                         volume);
        }
        ToggleWireframeMode(material.wireframe_enabled_);

        // GLModel
        auto model = model_manager_->GetModel(model_instance);
        if (!model) {
            continue;
        }

        // Shader Program
        auto graphics_program = graphics_compiler_->CreateProgram(material);
        if (!graphics_program) {
            // TODO: Add error logging
            std::cout << "Failed to create graphics program" << std::endl;
            continue;
        }
        graphics_program->Use();

        // Texture Maps
        auto gl_alpha_texture = texture_manager_->CreateTexture(material.texture_map_.alpha_map_, kAlphaTextureIndex);
        auto gl_ambient_texture = texture_manager_->CreateTexture(material.texture_map_.ambient_map_, kAmbientTextureIndex);
        auto gl_diffuse_texture = texture_manager_->CreateTexture(material.texture_map_.diffuse_map_, kDiffuseTextureIndex);
        auto gl_displacement_texture = texture_manager_->CreateTexture(material.texture_map_.displacement_map_, kDisplacementTextureIndex);
        auto gl_normal_texture = texture_manager_->CreateTexture(material.texture_map_.normal_map_, kNormalTextureIndex);
        if (gl_alpha_texture) {
            GLenum texture_unit = GL_TEXTURE0 + kAlphaTextureIndex;
            gl_alpha_texture->GenerateMipMap(texture_unit);
            gl_alpha_texture->Bind(texture_unit);
            graphics_program->SetUniform("alpha_texture", kAlphaTextureIndex);
        }
        if (gl_ambient_texture) {
            GLenum texture_unit = GL_TEXTURE0 + kAmbientTextureIndex;
            gl_ambient_texture->GenerateMipMap(texture_unit);
            gl_ambient_texture->Bind(texture_unit);
            graphics_program->SetUniform("ambient_texture", kAmbientTextureIndex);
        }
        if (gl_diffuse_texture) {
            GLenum texture_unit = GL_TEXTURE0 + kDiffuseTextureIndex;
            gl_diffuse_texture->GenerateMipMap(texture_unit);
            gl_diffuse_texture->Bind(texture_unit);
            graphics_program->SetUniform("diffuse_texture", kDiffuseTextureIndex);
        }
        if (gl_displacement_texture) {
            GLenum texture_unit = GL_TEXTURE0 + kDisplacementTextureIndex;
            gl_displacement_texture->GenerateMipMap(texture_unit);
            gl_displacement_texture->Bind(texture_unit);
            graphics_program->SetUniform("displacement_texture", kDisplacementTextureIndex);
        }
        if (gl_normal_texture) {
            GLenum texture_unit = GL_TEXTURE0 + kNormalTextureIndex;
            gl_normal_texture->GenerateMipMap(texture_unit);
            gl_normal_texture->Bind(texture_unit);
            graphics_program->SetUniform("normal_texture", kNormalTextureIndex);
        }

        // Camera dependent uniforms
        graphics_program->SetUniform("projection_matrix", projection_matrix);
        graphics_program->SetUniform("camera_position", camera.position_);
        graphics_program->FlushUniforms();

        auto model_view_matrix = view_matrix * transform.GetLocalToWorldMatrix();
        graphics_program->FlushUniform("model_view_matrix", model_view_matrix);
        model->Draw();

        graphics_program->Finish();
    }
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

std::vector<zero::Component::Entity> GLRenderer::GetViewableEntities(const entt::registry& registry,
                                                                     const Camera& camera) {
    auto renderable_view = registry.view<const Transform,
                                         const Volume,
                                         const Material,
                                         const ModelInstance>();

    auto culler = ViewVolumeBuilder::create(camera);
    std::vector<Component::Entity> viewable_entities;

    for (auto renderable_entity : renderable_view) {
        const auto& volume = renderable_view.get<const Volume>(renderable_entity);
        if (culler->IsCulled(volume.bounding_volume_)) {
            continue;
        }
        viewable_entities.push_back(renderable_entity);
    }

    return viewable_entities;
}

void GLRenderer::ReadShaderSource(const std::string& filename, std::string& destination) {
    std::ifstream input_file(filename);
    if (!input_file) return;
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    destination = buffer.str();
}