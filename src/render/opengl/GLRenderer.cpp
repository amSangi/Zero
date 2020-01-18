#include "render/opengl/GLRenderer.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/ViewVolumeBuilder.hpp"
#include "core/Transform.hpp"
#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>

using namespace zero::render;

GLRenderer::GLRenderer()
: graphics_compiler_(std::make_unique<GLCompiler>())
, model_manager_(std::make_unique<GLModelManager>())
, texture_manager_(std::make_unique<GLTextureManager>())
, instantiator_(std::make_unique<GLInstantiator>())
{}

GLRenderer::~GLRenderer() {
    ShutDown();
}

void GLRenderer::Initialize(const RenderSystemConfig& config) {
    ShutDown();

    InitializeGL();

    // Initialize Shaders
    for (const auto& vertex_shader_file : config.vertex_shader_files_) {
        ShaderStage stage;
        stage.type_ = IShader::Type::VERTEX_SHADER;
        stage.filename_ = vertex_shader_file;

        // Read source
        ReadShaderSource(stage.filename_, stage.source_);
        if (stage.source_.empty()) {
            continue;
        }

        if (!graphics_compiler_->InitializeShader(stage)) {
            std::cout << "Failed to initialize vertex shader" << std::endl;
        }
    }
    for (const auto& fragment_shader_file : config.fragment_shader_files_) {
        ShaderStage stage;
        stage.type_ = IShader::Type::FRAGMENT_SHADER;
        stage.filename_ = fragment_shader_file;

        // Read source
        ReadShaderSource(stage.filename_, stage.source_);
        if (stage.source_.empty()) {
            continue;
        }

        if (!graphics_compiler_->InitializeShader(stage)) {
            std::cout << "Failed to initialize fragment shader" << std::endl;
        }
    }

    // Initialize 3D Models
    for (const auto& model_file : config.model_files_) {
        model_manager_->LoadModel(model_file);
    }
    model_manager_->InitializeModels();

    // Initialize Images
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

void GLRenderer::Render(const entt::registry& registry, const TimeDelta& time_delta) {
    auto camera_view = registry.view<const Camera>();
    auto renderable_view = registry.view<const Transform,
                                         const Volume,
                                         const Material,
                                         const ModelInstance>();

    for (auto camera_entity : camera_view) {
        const auto& camera = camera_view.get(camera_entity);
        const auto projection_matrix = camera.GetProjectionMatrix();
        const auto view_matrix = camera.GetViewMatrix();

        // Do not render culled entities
        auto culler = ViewVolumeBuilder::create(camera);
        std::vector<Component::Entity> viewable_entities;
        for (auto renderable_entity : renderable_view) {
            const auto& volume = renderable_view.get<const Volume>(renderable_entity);
            if (culler->IsCulled(volume.bounding_volume_)) continue;
            viewable_entities.push_back(renderable_entity);
        }

        glViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
        glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
        glClear(GL_COLOR_BUFFER_BIT);
        
        for (auto viewable_entity : viewable_entities) {
            const auto& model_instance = renderable_view.get<const ModelInstance>(viewable_entity);
            if (model_instance.child_identifier_ != 0) {
                // Entity is a child entity. Already rendered.
                continue;
            }
            const auto& transform = renderable_view.get<const Transform>(viewable_entity);
            const auto& material = renderable_view.get<const Material>(viewable_entity);

            auto model = model_manager_->GetModel(model_instance.filename_);
            if (!model) {
                // TODO: Add error logging
                std::cout << "Failed to load model" << std::endl;
                continue;
            }
            model->Initialize();

            // Shader Program
            auto graphics_program = graphics_compiler_->CreateProgram(material);
            if (!graphics_program) {
                // TODO: Add error logging
                std::cout << "Failed to create graphics program" << std::endl;
                continue;
            }
            graphics_program->Use();

            // Texture Maps
            constexpr uint8 alpha_index = 0;
            constexpr uint8 ambient_index = 1;
            constexpr uint8 diffuse_index = 2;
            constexpr uint8 displacement_index = 3;
            constexpr uint8 normal_index = 4;
            auto gl_alpha_texture = texture_manager_->CreateTexture(material.texture_map_.alpha_map_, alpha_index);
            auto gl_ambient_texture = texture_manager_->CreateTexture(material.texture_map_.ambient_map_, ambient_index);
            auto gl_diffuse_texture = texture_manager_->CreateTexture(material.texture_map_.diffuse_map_, diffuse_index);
            auto gl_displacement_texture = texture_manager_->CreateTexture(material.texture_map_.displacement_map_, displacement_index);
            auto gl_normal_texture = texture_manager_->CreateTexture(material.texture_map_.normal_map_, normal_index);
            if (gl_alpha_texture) {
                GLenum texture_unit = GL_TEXTURE0 + alpha_index;
                gl_alpha_texture->GenerateMipMap(texture_unit);
                gl_alpha_texture->Bind(texture_unit);
                graphics_program->SetUniform("alpha_texture", alpha_index);
            }
            if (gl_ambient_texture) {
                GLenum texture_unit = GL_TEXTURE0 + ambient_index;
                gl_ambient_texture->GenerateMipMap(texture_unit);
                gl_ambient_texture->Bind(texture_unit);
                graphics_program->SetUniform("ambient_texture", ambient_index);
            }
            if (gl_diffuse_texture) {
                GLenum texture_unit = GL_TEXTURE0 + diffuse_index;
                gl_diffuse_texture->GenerateMipMap(texture_unit);
                gl_diffuse_texture->Bind(texture_unit);
                graphics_program->SetUniform("diffuse_texture", diffuse_index);
            }
            if (gl_displacement_texture) {
                GLenum texture_unit = GL_TEXTURE0 + displacement_index;
                gl_displacement_texture->GenerateMipMap(texture_unit);
                gl_displacement_texture->Bind(texture_unit);
                graphics_program->SetUniform("displacement_texture", displacement_index);
            }
            if (gl_normal_texture) {
                GLenum texture_unit = GL_TEXTURE0 + normal_index;
                gl_normal_texture->GenerateMipMap(texture_unit);
                gl_normal_texture->Bind(texture_unit);
                graphics_program->SetUniform("normal_texture", normal_index);
            }

            graphics_program->SetUniform("projection_matrix", projection_matrix);
            graphics_program->SetUniform("camera_position", camera.position_);
            graphics_program->FlushUniforms();

            RenderEntity(registry,
                         viewable_entities,
                         graphics_program,
                         view_matrix,
                         projection_matrix,
                         transform,
                         model);

            graphics_program->Finish();
        }

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

    return instantiator_->InstantiateModel(registry, gl_model);
}

void GLRenderer::RenderEntity(const entt::registry& registry,
                              const std::vector<Component::Entity>& viewable_entities,
                              const std::shared_ptr<IProgram>& graphics_program,
                              const math::Matrix4x4& view_matrix,
                              const math::Matrix4x4& projection_matrix,
                              const Transform& transform,
                              const std::shared_ptr<GLModel>& model) {

    // Render model
    auto model_view_matrix = view_matrix * transform.GetLocalToWorldMatrix();
    graphics_program->FlushUniform("model_view_matrix", model_view_matrix);
    model->Draw();

    // Render child models
    for (auto child_entity : transform.children_) {
        // Skip culled children
        auto search = std::find(viewable_entities.begin(), viewable_entities.end(), child_entity);
        if (search == viewable_entities.end()) {
            continue;
        }

        // Skip entities if the child model does not exist
        const auto& child_model_instance = registry.get<const ModelInstance>(child_entity);
        auto child_gl_model = model->FindChild(child_model_instance.child_identifier_);
        if (!child_gl_model) {
            // TODO: Add error logging
            continue;
        }

        const auto& child_transform = registry.get<const Transform>(child_entity);
        RenderEntity(registry,
                     viewable_entities,
                     graphics_program,
                     view_matrix,
                     projection_matrix,
                     child_transform,
                     child_gl_model);
    }
}

void GLRenderer::ReadShaderSource(const std::string& filename, std::string& destination) {
    std::ifstream input_file(filename);
    if (!input_file) return;
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    destination = buffer.str();
}

void GLRenderer::InitializeGL() {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}