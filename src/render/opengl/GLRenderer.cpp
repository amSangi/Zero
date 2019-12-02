#include "render/opengl/GLRenderer.hpp"
#include "render/ViewVolumeBuilder.hpp"
#include <fstream>

using namespace zero::render;

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
}

void GLRenderer::Render(const entt::registry& registry, float dt) {
    // TODO: Finish implementation
    /**
     * 1. Get all entities with the Camera component
     * 2. Get all entities with the Volume component
     * 3. Get all entities that are viewable (within Camera Frustrum) using Volumes
     * 4. Get the Transform, Material, and MeshInstance components of viewable entities
     * 5. Begin naive rendering:
     *      5a. Create GraphicsProgram using Material component
     *      5b. Create GLTexture using Material component
     *      5c. Retrieve GLModel using Mesh component
     *      5d. Set GLProgram uniforms/attributes
     *      5e. Render our GLDrawable
     *
     * Possible Optimizations:
     *       - Mesh storage
     *       - Sort by texture
     *       - Sort by graphics program
     */
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
    if (!gl_model) return Component::NullEntity;

    auto entity = registry.create();
    // TODO: Finish Implementation

    return entity;
}
