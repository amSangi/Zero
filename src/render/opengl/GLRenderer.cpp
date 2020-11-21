#include "render/opengl/GLRenderer.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLModel.hpp"
#include "render/opengl/GLModelManager.hpp"
#include "render/opengl/GLPrimitiveMeshManager.hpp"
#include "render/opengl/GLSampler.hpp"
#include "render/opengl/GLTexture.hpp"
#include "render/opengl/GLTextureManager.hpp"
#include "render/opengl/GLUniformManager.hpp"
#include "render/opengl/pipeline/GLEntityRenderPass.hpp"
#include "render/opengl/pipeline/GLShadowMapPass.hpp"
#include "render/opengl/pipeline/GLSkyDomePass.hpp"
#include "render/ShaderStage.hpp"
#include "core/EngineCore.hpp"
#include "core/Logger.hpp"
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
, model_loader_(std::make_unique<AssimpLoader>(model_manager_.get()))
, primitive_manager_(std::make_unique<GLPrimitiveMeshManager>())
, texture_manager_(std::make_unique<GLTextureManager>())
, uniform_manager_(std::make_unique<GLUniformManager>())
, render_pipeline_(std::make_unique<RenderPipeline>())
, engine_core_(engine_core)
{
    LOG_VERBOSE(kTitle, "GLRenderer instance constructed");
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
    InitializeShaders();
    InitializeModels();
    InitializeImages();
    LOG_DEBUG(kTitle, "Pre-loading primitives");
    primitive_manager_->LoadPrimitives();
    LOG_DEBUG(kTitle, "Setting up uniform buffer objects");
    uniform_manager_->Initialize();
    InitializeRenderPasses(config);
}

void GLRenderer::Render()
{
    entt::registry& registry = engine_core_->GetRegistry();
    LOG_VERBOSE(kTitle, "Updating light uniforms");
    uniform_manager_->UpdateLightUniforms(registry);

    // Render entities for each camera/viewport
    auto camera_view = registry.view<const Camera>();
    LOG_VERBOSE(kTitle, "Rendering to cameras. Camera count: " + std::to_string(camera_view.size()));
    for (Entity camera_entity : camera_view)
    {
        const auto& camera = camera_view.get<const Camera>(camera_entity);
        render_pipeline_->Execute(camera, registry);
    }
}

void GLRenderer::PostRender()
{
    LOG_DEBUG(kTitle, "Clearing all images");
    texture_manager_->UnloadImages();
}

void GLRenderer::ShutDown()
{
    LOG_DEBUG(kTitle, "Clearing all shaders");
    graphics_compiler_->ClearShaders();
    LOG_DEBUG(kTitle, "Clearing all models");
    model_manager_->ClearModels();
    LOG_DEBUG(kTitle, "Clearing all primitives");
    primitive_manager_->ClearPrimitives();
    LOG_DEBUG(kTitle, "Clearing all textures");
    texture_manager_->UnloadGLTextures();
    LOG_DEBUG(kTitle, "Clearing all images");
    texture_manager_->UnloadImages();
}

std::weak_ptr<Model> GLRenderer::GetModel(const std::string& model)
{
    return model_manager_->GetModel(model);
}

//////////////////////////////////////////////////
///// Initialization
//////////////////////////////////////////////////
void GLRenderer::InitializeGL()
{
    LOG_VERBOSE(kTitle, "Initializing OpenGL fields");

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

#if LOGGING_ENABLED
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLMessageCallback, nullptr);
#endif
}

void GLRenderer::InitializeShaders()
{
    AssetManager& asset_manager = engine_core_->GetAssetManager();

    std::string compile_error_message;

    // Initialize vertex shaders
    const std::vector<std::string>& vertex_shaders = asset_manager.GetVertexFiles();
    LOG_DEBUG(kTitle, "Initializing OpenGL vertex shaders. Shader count: " + std::to_string(vertex_shaders.size()));
    for (const auto& vertex_shader_file : vertex_shaders)
    {
        ShaderStage stage;
        stage.type_ = IShader::Type::VERTEX_SHADER;
        stage.name_ = vertex_shader_file;

        // Read source from fully qualified file
        std::string fully_qualified_file = asset_manager.GetVertexShaderFilePath(vertex_shader_file);
        ReadShaderSource(fully_qualified_file, stage.source_);
        if (stage.source_.empty())
        {
            LOG_ERROR(kTitle, "Failed to read vertex shader source: " + fully_qualified_file);
            continue;
        }

        if (!graphics_compiler_->InitializeShader(stage))
        {
            LOG_ERROR(kTitle, "Failed to initialize shader: " + fully_qualified_file);
        }
    }

    // Initialize fragment shaders
    const std::vector<std::string>& fragment_shaders = asset_manager.GetFragmentFiles();
    LOG_DEBUG(kTitle, "Initializing OpenGL fragment shaders. Shader count: " + std::to_string(fragment_shaders.size()));
    for (const auto& fragment_shader_file : fragment_shaders)
    {
        ShaderStage stage;
        stage.type_ = IShader::Type::FRAGMENT_SHADER;
        stage.name_ = fragment_shader_file;

        // Read source from fully qualified file
        std::string fully_qualified_file = asset_manager.GetFragmentShaderFilePath(fragment_shader_file);
        ReadShaderSource(fully_qualified_file, stage.source_);
        if (stage.source_.empty())
        {
            LOG_ERROR(kTitle, "Failed to read fragment shader source: " + fully_qualified_file);
            continue;
        }

        if (!graphics_compiler_->InitializeShader(stage))
        {
            LOG_ERROR(kTitle, "Failed to initialize shader: " + fully_qualified_file);
        }
    }
}

void GLRenderer::InitializeModels()
{
    AssetManager& asset_manager = engine_core_->GetAssetManager();

    const std::vector<std::string>& model_files = asset_manager.GetModelFiles();
    LOG_DEBUG(kTitle, "Pre-loading models. Model count: " + std::to_string(model_files.size()));
    for (const auto& model_file : model_files)
    {
        std::string model_file_path = asset_manager.GetModelFilePath(model_file);
        model_loader_->LoadModel(model_file, model_file_path);
    }
}

void GLRenderer::InitializeImages()
{
    AssetManager& asset_manager = engine_core_->GetAssetManager();

    const std::vector<std::string>& texture_files = asset_manager.GetTextureFiles();
    LOG_DEBUG(kTitle, "Pre-loading textures. Texture count: " + std::to_string(texture_files.size()));

    for (const auto& texture_file : texture_files)
    {
        if (!texture_manager_->InitializeImage(texture_file, asset_manager.GetTextureFilePath(texture_file)))
        {
            LOG_ERROR(kTitle, "Failed to load image: " + texture_file);
        }
    }

    LOG_DEBUG(kTitle, "Setting up OpenGL texture sampler");
    LOG_DEBUG(kTitle, "Texture unit count: " + std::to_string(texture_manager_->GetTextureUnitCount()));

    LOG_DEBUG(kTitle, "Loading all OpenGL textures");
    if (!texture_manager_->InitializeGLTextures())
    {
        LOG_ERROR(kTitle, "Failed to load all OpenGL textures");
    }
}

void GLRenderer::InitializeRenderPasses(const RenderSystemConfig& config)
{
    constexpr uint32 shadow_map_resolution_width_height = 2048U;

    auto gl_shadow_map_pass = std::make_unique<GLShadowMapPass>(graphics_compiler_.get(),
                                                                model_manager_.get(),
                                                                primitive_manager_.get(),
                                                                texture_manager_.get(),
                                                                uniform_manager_.get(),
                                                                shadow_map_resolution_width_height,
                                                                shadow_map_resolution_width_height);
    gl_shadow_map_pass->Initialize();
    render_pipeline_->AddRenderPass(std::move(gl_shadow_map_pass));
    render_pipeline_->AddRenderPass(std::make_unique<GLSkyDomePass>(graphics_compiler_.get(),
                                                                    primitive_manager_.get(),
                                                                    uniform_manager_.get()));
    render_pipeline_->AddRenderPass(std::make_unique<GLEntityRenderPass>(graphics_compiler_.get(),
                                                                         model_manager_.get(),
                                                                         primitive_manager_.get(),
                                                                         texture_manager_.get(),
                                                                         uniform_manager_.get()));
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
                       const void* /* user_param */)
{
    constexpr auto kOpenGLTitle = "GLMessageCallback";

    if (type == GL_DEBUG_TYPE_ERROR)
    {
        LOG_ERROR(kOpenGLTitle, std::string(message, length));
    }
    else
    {
        LOG_DEBUG(kOpenGLTitle, std::string(message, length));
    }
}

} // namespace zero::render