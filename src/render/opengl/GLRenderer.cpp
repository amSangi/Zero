#include "render/opengl/GLRenderer.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/ViewVolumeBuilder.hpp"
#include "core/Transform.hpp"
#include <fstream>
#include <vector>

using namespace zero::render;

GLRenderer::GLRenderer()
: graphics_compiler_(std::make_unique<GLCompiler>())
, model_manager_(std::make_unique<GLModelManager>())
, texture_manager_(std::make_unique<GLTextureManager>())
, texture_sampler_(std::make_shared<GLSampler>())
{}

GLRenderer::~GLRenderer() {
    ShutDown();
}

void GLRenderer::Initialize(const RenderSystemConfig& config) {
    ShutDown();

    // Initialize Shaders
    for (const auto& vertex_shader_file : config.vertex_shader_files_) {
        ShaderStage stage;
        stage.type_ = IShader::Type::VERTEX_SHADER;
        stage.filename_ = vertex_shader_file;

        // Read source
        std::ifstream in(vertex_shader_file, std::ios::in);
        if (!in) continue;
        in.seekg(0, std::ios::end);
        stage.source_.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&stage.source_[0], stage.source_.size());
        in.close();

        graphics_compiler_->InitializeShader(stage);
    }
    for (const auto& fragment_shader_file : config.fragment_shader_files_) {
        ShaderStage stage;
        stage.type_ = IShader::Type::FRAGMENT_SHADER;
        stage.filename_ = fragment_shader_file;

        // Read source
        std::ifstream in(fragment_shader_file, std::ios::in);
        if (!in) continue;
        in.seekg(0, std::ios::end);
        stage.source_.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&stage.source_[0], stage.source_.size());
        in.close();

        graphics_compiler_->InitializeShader(stage);
    }

    // Initialize 3D Models
    for (const auto& model_file : config.model_files_) {
        model_manager_->LoadModel(model_file);
    }

    // Initialize Images
    for (const auto& texture_file : config.texture_files_) {
        texture_manager_->InitializeImage(texture_file);
    }

    texture_sampler_->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    texture_sampler_->SetMagnificationFilter(ISampler::Filter::LINEAR);
    texture_sampler_->SetWrappingS(ISampler::Wrapping::REPEAT);
    texture_sampler_->SetWrappingT(ISampler::Wrapping::REPEAT);
    for (int i = 0; i < 5; ++i) {
        texture_manager_->SetSampler(texture_sampler_, i);
    }
}

void GLRenderer::Render(const entt::registry& registry, float dt) {
    auto camera_view = registry.view<const Camera>();
    auto renderable_view = registry.view<const Transform,
                                         const Volume,
                                         const Material,
                                         const MeshInstance>();

    for (auto camera_entity : camera_view) {
        const auto& camera = camera_view.get(camera_entity);

        // Do not render culled entities
        auto culler = ViewVolumeBuilder::create(camera);
        std::vector<Component::Entity> viewable_entities;
        for (auto renderable_entity : renderable_view) {
            const auto& volume = renderable_view.get<const Volume>(renderable_entity);
            if (culler->IsCulled(volume.bounding_volume_)) continue;
            viewable_entities.push_back(renderable_entity);
        }

        for (auto viewable_entity : viewable_entities) {
            const auto& transform = renderable_view.get<const Transform>(viewable_entity);
            const auto& material = renderable_view.get<const Material>(viewable_entity);
            const auto& mesh_instance = renderable_view.get<const MeshInstance>(viewable_entity);
            RenderEntity(camera, transform, material, mesh_instance);
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
        return Component::NullEntity;
    }

    auto entity = registry.create();
    registry.assign<Transform>(entity);
    registry.assign<render::Volume>(entity, gl_model->GetVolume());
    registry.assign<render::Material>(entity, gl_model->GetMaterial());
    MeshInstance mesh_instance;
    mesh_instance.model_file_ = model;
    registry.assign<render::MeshInstance>(entity, mesh_instance);

    return entity;
}

void GLRenderer::RenderEntity(const Camera& camera,
                              const Transform& transform,
                              const Material& material,
                              const MeshInstance& mesh_instance) {
    // Shader Program
    auto gl_program = graphics_compiler_->CreateProgram(material);
    gl_program->Use();

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

    if (gl_alpha_texture) gl_alpha_texture->Bind(GL_TEXTURE0 + alpha_index);
    if (gl_ambient_texture) gl_ambient_texture->Bind(GL_TEXTURE0 + ambient_index);
    if (gl_diffuse_texture) gl_diffuse_texture->Bind(GL_TEXTURE0 + diffuse_index);
    if (gl_displacement_texture) gl_displacement_texture->Bind(GL_TEXTURE0 + displacement_index);
    if (gl_normal_texture) gl_normal_texture->Bind(GL_TEXTURE0 + normal_index);

    // Uniforms
    gl_program->SetUniform("alpha_texture", alpha_index);
    gl_program->SetUniform("ambient_texture", ambient_index);
    gl_program->SetUniform("diffuse_texture", diffuse_index);
    gl_program->SetUniform("displacement_texture", displacement_index);
    gl_program->SetUniform("normal_texture", normal_index);

    // 3D Model
    auto gl_model = model_manager_->GetModel(mesh_instance.model_file_);
    // TODO: Implement model rendering

    gl_program->Finish();
}