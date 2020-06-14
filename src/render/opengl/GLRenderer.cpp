#include "render/opengl/GLRenderer.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLDefaultShader.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLUniformManager.hpp"
#include "render/MeshGenerator.hpp"
#include "render/Optimizer.hpp"
#include "core/EngineCore.hpp"
#include "component/Camera.hpp"
#include <fstream>
#include <vector>

namespace zero::render
{

const char* GLRenderer::kTitle = "GLRenderer";

void GLMessageCallback(GLenum source,
                       GLenum type,
                       GLuint id,
                       GLenum severity,
                       GLsizei length,
                       const GLchar* message,
                       const void* userParam);

GLRenderer::GLRenderer(EngineCore* engine_core)
: graphics_compiler_(std::make_unique<GLCompiler>())
, model_manager_(std::make_unique<GLModelManager>())
, primitive_manager_(std::make_unique<GLPrimitiveMeshManager>())
, texture_manager_(std::make_unique<GLTextureManager>())
, uniform_manager_(std::make_unique<GLUniformManager>())
, engine_core_(engine_core)
{
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "GLRenderer instance constructed")
}

GLRenderer::~GLRenderer()
{
}

//////////////////////////////////////////////////
///// IRenderer Implementation
//////////////////////////////////////////////////
void GLRenderer::Initialize(const RenderSystemConfig& config)
{
    InitializeGL();
    InitializeShaders(config);
    InitializeModels(config);
    InitializeImages(config);
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Pre-loading primitives")
    primitive_manager_->LoadPrimitives();
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Setting up uniform buffer objects")
    uniform_manager_->Initialize();
}

void GLRenderer::Render()
{
    entt::registry& registry = engine_core_->GetRegistry();
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Updating light uniforms")
    uniform_manager_->UpdateLightUniforms(registry);

    // Render entities for each camera/viewport
    auto camera_view = registry.view<const Camera>();
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Rendering to cameras. Camera count: " + std::to_string(camera_view.size()))
    for (Entity camera_entity : camera_view)
    {
        const auto& camera = camera_view.get(camera_entity);
        RenderEntities(camera, registry);
    }
}

void GLRenderer::PostRender()
{
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Clearing cached graphics programs")
    graphics_compiler_->ClearPrograms();
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Unloading all textures from main memory")
    texture_manager_->UnloadImages();
}

void GLRenderer::ShutDown()
{
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Clearing all shaders")
    graphics_compiler_->ClearShaders();
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Clearing all models")
    model_manager_->ClearModels();
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Clearing all primitives")
    primitive_manager_->ClearPrimitives();
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Clearing all textures")
    texture_manager_->ClearImages();
}

std::weak_ptr<IModel> GLRenderer::GetModel(const std::string& model)
{
    return model_manager_->GetModel(model);
}

//////////////////////////////////////////////////
///// Initialization
//////////////////////////////////////////////////
void GLRenderer::InitializeGL()
{
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Initializing OpenGL fields")

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    glDepthFunc(GL_LEQUAL);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLMessageCallback, &engine_core_->GetLogger());
}

void GLRenderer::InitializeShaders(const RenderSystemConfig& config)
{

    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Initializing OpenGL vertex shaders. Shader count: " + std::to_string(config.vertex_shader_files_.size()))
    InitializeShaderFiles(config.vertex_shader_files_, IShader::Type::VERTEX_SHADER);

    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Initializing OpenGL fragment shaders. Shader count: " + std::to_string(config.fragment_shader_files_.size()))
    InitializeShaderFiles(config.fragment_shader_files_, IShader::Type::FRAGMENT_SHADER);

    if (!graphics_compiler_->InitializeShader(GLDefaultShader::kVertexShader))
    {
        LOG_ERROR(engine_core_->GetLogger(), kTitle, "Failed to initialize default vertex shader")

    }

    if (!graphics_compiler_->InitializeShader(GLDefaultShader::kFragmentShader))
    {
        LOG_ERROR(engine_core_->GetLogger(), kTitle, "Failed to initialize default fragment shader")
    }
}

void GLRenderer::InitializeShaderFiles(const std::vector<std::string>& shaders, IShader::Type shader_type)
{
    for (const auto& shader_file : shaders)
    {
        ShaderStage stage;
        stage.type_ = shader_type;
        stage.name_ = shader_file;

        // Read source
        ReadShaderSource(shader_file, stage.source_);
        if (stage.source_.empty())
        {
            continue;
        }

        if (!graphics_compiler_->InitializeShader(stage))
        {
            LOG_ERROR(engine_core_->GetLogger(), kTitle, "Failed to initialize shader: " + shader_file)
        }
    }
}

void GLRenderer::InitializeModels(const RenderSystemConfig& config)
{
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Pre-loading models. Model count: " + std::to_string(config.model_files_.size()))

    for (const auto& model_file : config.model_files_)
    {
        model_manager_->LoadModel(model_file);
    }
}

void GLRenderer::InitializeImages(const RenderSystemConfig& config)
{
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Pre-loading textures. Texture count: " + std::to_string(config.texture_files_.size()))

    for (const auto& texture_file : config.texture_files_)
    {
        texture_manager_->InitializeImage(texture_file);
    }

    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Setting up OpenGL texture sampler")
    LOG_DEBUG(engine_core_->GetLogger(), kTitle, "Texture unit count: " + std::to_string(texture_manager_->GetTextureUnitCount()))

    auto texture_sampler = std::make_shared<GLSampler>();
    texture_sampler->Initialize();
    texture_sampler->SetMinificationFilter(ISampler::Filter::LINEAR_MIPMAP_LINEAR);
    texture_sampler->SetMagnificationFilter(ISampler::Filter::LINEAR);
    texture_sampler->SetWrappingS(ISampler::Wrapping::REPEAT);
    texture_sampler->SetWrappingT(ISampler::Wrapping::REPEAT);
    for (int i = 0; i < texture_manager_->GetTextureUnitCount(); ++i)
    {
        texture_manager_->SetSampler(texture_sampler, i);
    }
}

//////////////////////////////////////////////////
///// Render
//////////////////////////////////////////////////
void GLRenderer::RenderEntities(const Camera& camera, const entt::registry& registry)
{
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Updating OpenGL display settings")

    UpdateGL(camera);
    const auto projection_matrix = camera.GetProjectionMatrix();
    const auto view_matrix = camera.GetViewMatrix();
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Updating camera uniforms")
    uniform_manager_->UpdateCameraUniforms(projection_matrix, view_matrix, camera.position_);

    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Retrieving all renderable entities")
    auto viewable_entities = Optimizer::ExtractRenderableEntities(camera, registry);
    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Viewable entity count: " + std::to_string(viewable_entities.size()))

    auto renderable_view = registry.view<const Transform, const Material, const Volume>();
    auto model_view = registry.view<const ModelInstance>();
    auto primitive_view = registry.view<const PrimitiveInstance>();

    LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Beginning entity rendering process")
    for (Entity viewable_entity : viewable_entities)
    {
        const auto& transform = renderable_view.get<const Transform>(viewable_entity);
        const auto& material = renderable_view.get<const Material>(viewable_entity);
        const auto& volume = renderable_view.get<const Volume>(viewable_entity);
        if (camera.render_bounding_volumes_)
        {
            RenderVolume(camera, projection_matrix, view_matrix, volume);
        }
        ToggleWireframeMode(material.wireframe_enabled_);

        auto graphics_program = graphics_compiler_->CreateProgram(material);
        if (!graphics_program)
        {
            LOG_ERROR(engine_core_->GetLogger(), kTitle, "Failed to create/load graphics program for the entity")
            continue;
        }
        LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Binding graphics program and uniform buffer objects together")
        GLUniformManager::BindGraphicsProgram(graphics_program);
        graphics_program->Use();

        // Update uniforms
        auto gl_textures = texture_manager_->CreateTextureMap(material);
        LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Updating sampler uniforms")
        GLUniformManager::UpdateSamplerUniforms(graphics_program, gl_textures);
        math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
        LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Updating model uniforms")
        uniform_manager_->UpdateModelUniforms(model_matrix, (view_matrix * model_matrix).Inverse());
        LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Updating material uniforms")
        uniform_manager_->UpdateMaterialUniforms(material);
        graphics_program->FlushUniforms();

        // Draw Mesh
        if (registry.has<ModelInstance>(viewable_entity))
        {
            const auto& model_instance = model_view.get(viewable_entity);
            auto model = model_manager_->GetModel(model_instance);
            if (model)
            {
                LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Drawing model")
                model->Draw();
            }
        }
        else
        {
            const auto& primitive_instance = primitive_view.get(viewable_entity);
            auto primitive = primitive_manager_->GetPrimitiveMesh(primitive_instance);
            if (primitive)
            {
                LOG_VERBOSE(engine_core_->GetLogger(), kTitle, "Drawing primitive")
                primitive->Draw();
            }
        }

        graphics_program->Finish();
    }
}

void GLRenderer::RenderVolume(const Camera& camera,
                              const math::Matrix4x4& projection_matrix,
                              const math::Matrix4x4& view_matrix,
                              const Volume& volume)
{
    ToggleWireframeMode(true);

    auto model_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(volume.bounding_volume_.radius_))
            .Translate(volume.bounding_volume_.center_);
    // use default shaders
    std::shared_ptr<IProgram> gl_primitive_program = graphics_compiler_->CreateProgram(Material{});
    gl_primitive_program->Use();
    math::Matrix4x4 model_view_matrix = view_matrix * model_matrix;
    gl_primitive_program->SetUniform("projection_matrix", projection_matrix);
    gl_primitive_program->SetUniform("model_view_matrix", model_view_matrix);
    gl_primitive_program->SetUniform("normal_matrix", model_view_matrix.Inverse().Transpose().GetMatrix3x3());
    gl_primitive_program->SetUniform("camera_position", camera.position_);
    gl_primitive_program->SetUniform("material.specular_exponent", 32.0F);
    gl_primitive_program->SetUniform("material.diffuse_color", math::Vec3f(1.0F, 0.0F, 0.0F));
    gl_primitive_program->FlushUniforms();

    PrimitiveInstance primitive_instance{};
    primitive_instance.Set(Sphere());
    std::shared_ptr<GLMesh> gl_primitive = primitive_manager_->GetPrimitiveMesh(primitive_instance);
    gl_primitive->Draw();
    gl_primitive_program->Finish();
}

//////////////////////////////////////////////////
///// OpenGL Helpers
//////////////////////////////////////////////////
void GLRenderer::UpdateGL(const Camera& camera)
{
    glViewport(camera.viewport_.x_, camera.viewport_.y_, camera.viewport_.width_, camera.viewport_.height_);
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLRenderer::ToggleWireframeMode(bool enable_wireframe)
{
    if (enable_wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

//////////////////////////////////////////////////
///// Miscellaneous Helpers
//////////////////////////////////////////////////
void GLRenderer::ReadShaderSource(const std::string& filename, std::string& destination)
{
    std::ifstream input_file(filename);
    if (!input_file)
    {
        return;
    }
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    destination = buffer.str();
}

void GLMessageCallback(GLenum /* source */,
                       GLenum type,
                       GLuint /* id */,
                       GLenum /* severity */,
                       GLsizei length,
                       const GLchar* message,
                       const void* userParam)
{
    if (userParam == nullptr)
    {
        return;
    }

    auto* logger = (Logger*)(userParam);
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        LOG_ERROR((*logger), "OpenGL", std::string(message, length))
    }
    else
    {
        LOG_DEBUG((*logger), "OpenGL", std::string(message, length))
    }
}

} // namespace zero::render