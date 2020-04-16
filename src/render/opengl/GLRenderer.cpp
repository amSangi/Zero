#include "render/opengl/GLRenderer.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLInstantiator.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/opengl/GLDefaultShader.hpp"
#include "render/MeshGenerator.hpp"
#include "render/ViewVolumeBuilder.hpp"
#include "render/IViewVolume.hpp"
#include "render/Components.hpp"
#include "core/Transform.hpp"
#include <fstream>
#include <deque>
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
    texture_manager_->UnloadImages();
}

void GLRenderer::ShutDown() {
    graphics_compiler_->ClearShaders();
    model_manager_->ClearModels();
    primitive_manager_->ClearPrimitives();
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

void GLRenderer::RenderEntities(const Camera& camera, const entt::registry& registry) {
    UpdateGL(camera);
    const auto projection_matrix = camera.GetProjectionMatrix();
    const auto view_matrix = camera.GetViewMatrix();
    auto viewable_entities = GetViewableEntities(registry, camera);
    auto renderable_view = registry.view<const Transform, const Material, const Volume>();
    auto model_view = registry.view<const ModelInstance>();
    auto primitive_view = registry.view<const PrimitiveInstance>();
    for (auto viewable_entity : viewable_entities) {
        const auto& transform = renderable_view.get<const Transform>(viewable_entity);
        const auto& material = renderable_view.get<const Material>(viewable_entity);
        const auto& volume = renderable_view.get<const Volume>(viewable_entity);
        if (camera.render_bounding_volumes_) {
            RenderVolume(projection_matrix, view_matrix, volume);
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
        // Texture Maps
        auto gl_textures = texture_manager_->CreateTextureMap(material);
        for (int i = 0; i < gl_textures.size(); ++i) {
            const auto& gl_texture = gl_textures[i];
            GLenum texture_unit = GL_TEXTURE0 + i;
            gl_texture->GenerateMipMap(texture_unit);
            gl_texture->Bind(texture_unit);
            graphics_program->SetUniform(gl_texture->GetUniformName(), i);
        }

        // Set lights
        SetLightUniforms(graphics_program, registry);

        // Set uniforms
        graphics_program->SetUniform("projection_matrix", projection_matrix);
        graphics_program->SetUniform("model_view_matrix", view_matrix * transform.GetLocalToWorldMatrix());
        graphics_program->SetUniform("color", math::Vec3f::One());
        graphics_program->SetUniform("camera_position", camera.position_);
        graphics_program->FlushUniforms();

        // Draw Mesh
        if (registry.has<ModelInstance>(viewable_entity)) {
            const auto& model_instance = model_view.get(viewable_entity);
            auto model = model_manager_->GetModel(model_instance);
            if (model) model->Draw();
        }
        else {
            const auto& primitive_instance = primitive_view.get(viewable_entity);
            auto primitive = primitive_manager_->GetPrimitiveMesh(primitive_instance);
            if (primitive) primitive->Draw();
        }

        graphics_program->Finish();
    }
}

void GLRenderer::RenderVolume(const math::Matrix4x4& projection_matrix,
                              const math::Matrix4x4& view_matrix,
                              const Volume& volume) {
    Material default_material;
    default_material.shaders_.vertex_shader_ = GLDefaultShader::kVertexShader.name_;
    default_material.shaders_.fragment_shader_ = GLDefaultShader::kFragmentShader.name_;
    ToggleWireframeMode(true);

    auto model_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(volume.bounding_volume_.radius_))
            .Translate(volume.bounding_volume_.center_);
    auto gl_primitive_program = graphics_compiler_->CreateProgram(default_material);
    gl_primitive_program->Use();
    gl_primitive_program->SetUniform("projection_matrix", projection_matrix);
    gl_primitive_program->SetUniform("model_view_matrix", view_matrix * model_matrix);
    gl_primitive_program->SetUniform("color", math::Vec3f(1.0F, 0.0F, 0.0F));
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

void GLRenderer::SetLightUniforms(std::shared_ptr<IProgram> graphics_program, const entt::registry& registry) {
    // TODO: Finish Implementation
}


std::vector<zero::Component::Entity> GLRenderer::GetViewableEntities(const entt::registry& registry,
                                                                     const Camera& camera) {
    auto renderable_view = registry.view<const Transform,
                                         const Material,
                                         const Volume>();

    auto culler = ViewVolumeBuilder::create(camera);
    std::deque<Component::Entity> entities_to_cull;
    std::vector<Component::Entity> viewable_entities;
    // Get all root entities that are visible
    for (auto renderable_entity : renderable_view) {
        const auto& transform = renderable_view.get<const Transform>(renderable_entity);
        const auto& material = renderable_view.get<const Material>(renderable_entity);
        if (!material.visible_) {
            continue;
        }
        if (transform.parent_ == Component::NullEntity) {
            entities_to_cull.push_front(renderable_entity);
        }
    }

    // Cull entities
    while (!entities_to_cull.empty()) {
        auto entity = entities_to_cull.front();
        entities_to_cull.pop_front();

        // Ignore entities that do not have a mesh
        if (!registry.has<ModelInstance>(entity) && !registry.has<PrimitiveInstance>(entity)) {
            continue;
        }

        // Entity and its children are culled
        const auto& volume = renderable_view.get<const Volume>(entity);
        if (culler->IsCulled(volume.bounding_volume_)) {
            continue;
        }

        // Cull the children
        const auto& transform = renderable_view.get<const Transform>(entity);
        entities_to_cull.insert(entities_to_cull.end(), transform.children_.begin(), transform.children_.end());

        // Entity is viewable and contains a mesh
        viewable_entities.push_back(entity);
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