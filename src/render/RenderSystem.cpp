#include "render/RenderSystem.hpp"
#include "render/Instantiator.hpp"
#include "render/renderer/opengl/GLRenderHardware.hpp"
#include "component/Camera.hpp"
#include "core/Logger.hpp"
#include <fstream>


namespace zero::render
{

const char* RenderSystem::kTitle = "RenderSystem";

RenderSystem::RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config)
: zero::System(engine_core)
, config_(config)
, rhi_(std::make_unique<GLRenderHardware>())
, window_(std::make_unique<Window>(config.window_config_))
, mesh_cache_()
, program_cache_()
, shader_cache_()
, texture_cache_()
{
    LOG_VERBOSE(kTitle, "RenderSystem instance constructed");
}

void RenderSystem::Initialize()
{
    window_->Initialize();
    rhi_->Initialize();
    LoadModels();
    LoadShaders();
    LoadTextures();
}

void RenderSystem::PreUpdate()
{
}

void RenderSystem::Update(const TimeDelta& time_delta)
{
    if (!ContainsCamera())
    {
        // Do not render if a camera does not exist
        return;
    }

    LOG_VERBOSE(kTitle, "Generating IRenderView");
    LOG_VERBOSE(kTitle, "Updating light uniform buffers");
    LOG_VERBOSE(kTitle, "Updating shadow map buffers");
    LOG_VERBOSE(kTitle, "Rendering IRenderView");

    LOG_VERBOSE(kTitle, "Swapping buffers");
    window_->SwapBuffers();
}

void RenderSystem::PostUpdate()
{
}

void RenderSystem::ShutDown()
{
    LOG_VERBOSE(kTitle, "Clearing mesh cache");
    mesh_cache_.clear();
    LOG_VERBOSE(kTitle, "Clearing shader program cache");
    program_cache_.clear();
    LOG_VERBOSE(kTitle, "Clearing shader cache");
    shader_cache_.clear();
    LOG_VERBOSE(kTitle, "Clearing texture cache");
    texture_cache_.clear();

    LOG_VERBOSE(kTitle, "Shutting down rendering hardware");
    rhi_->Shutdown();

    LOG_VERBOSE(kTitle, "Cleaning up window and graphics context");
    window_->Cleanup();
}

Entity RenderSystem::CreateModelInstance(const std::string& model_filename)
{
    LOG_VERBOSE(kTitle, "Instantiating a new model");
    // TODO: Instantiate Model Instance
}

Entity RenderSystem::CreatePrimitiveInstance(const PrimitiveInstance& primitive)
{
    LOG_VERBOSE(kTitle, "Instantiating a new primitive");
    return Instantiator::InstantiatePrimitive(GetCore()->GetRegistry(), primitive);
}

Entity RenderSystem::CreateLightInstance(const Light& light, Entity entity)
{
    LOG_VERBOSE(kTitle, "Instantiating a new light source");
    return Instantiator::InstantiateLight(GetCore()->GetRegistry(), light, entity);
}

void RenderSystem::LoadModels()
{
    AssetManager& asset_manager = GetCore()->GetAssetManager();

    const std::vector<std::string>& model_files = asset_manager.GetModelFiles();
    LOG_DEBUG(kTitle, "Loading 3D assets. Asset count: " + std::to_string(model_files.size()));
    for (const auto& model_file : model_files)
    {
        std::string model_file_path = asset_manager.GetModelFilePath(model_file);
        std::shared_ptr<Model> model = asset_loader_->LoadModel(model_file, model_file_path);
        if (model)
        {
            // TODO: Cache Model
        }
        else
        {
            LOG_ERROR(kTitle, "Failed to load 3D asset: " + model_file_path);
        }
    }
}

void RenderSystem::LoadShaders()
{
    AssetManager& asset_manager = GetCore()->GetAssetManager();

    std::string compile_error_message;

    // Load vertex shaders
    const std::vector<std::string>& vertex_shaders = asset_manager.GetVertexFiles();
    LOG_DEBUG(kTitle, "Initializing vertex shaders. Shader count: " + std::to_string(vertex_shaders.size()));
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

        std::shared_ptr<IShader> shader = rhi_->CreateShader(stage);
        if (shader)
        {
            shader_cache_.emplace(std::hash<std::string>()(stage.name_), shader);
        }
        else
        {
            LOG_ERROR(kTitle, "Failed to load shader: " + fully_qualified_file);
        }
    }

    // Load fragment shaders
    const std::vector<std::string>& fragment_shaders = asset_manager.GetFragmentFiles();
    LOG_DEBUG(kTitle, "Initializing fragment shaders. Shader count: " + std::to_string(fragment_shaders.size()));
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

        std::shared_ptr<IShader> shader = rhi_->CreateShader(stage);
        if (shader)
        {
            shader_cache_.emplace(std::hash<std::string>()(stage.name_), shader);
        }
        else
        {
            LOG_ERROR(kTitle, "Failed to load shader: " + fully_qualified_file);
        }
    }
}

void RenderSystem::LoadTextures()
{
    AssetManager& asset_manager = GetCore()->GetAssetManager();

    const std::vector<std::string>& texture_files = asset_manager.GetTextureFiles();
    LOG_DEBUG(kTitle, "Pre-loading textures. Texture count: " + std::to_string(texture_files.size()));

    for (const auto& texture_file : texture_files)
    {
        std::string texture_file_path = asset_manager.GetTextureFilePath(texture_file);
        std::shared_ptr<ITexture> texture = rhi_->CreateTexture(std::make_unique<Image>(texture_file_path));

        if (texture)
        {
            texture_cache_.emplace(std::hash<std::string>()(texture_file), texture);
        }
        else
        {
            LOG_ERROR(kTitle, "Failed to load texture: " + texture_file_path);
        }
    }
}

bool RenderSystem::ContainsCamera() const
{
    auto camera_view = GetCore()->GetRegistry().view<const Camera>();
    return !camera_view.empty();
}

void RenderSystem::ReadShaderSource(const std::string& filename, std::string& destination)
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

} // namespace zero::render