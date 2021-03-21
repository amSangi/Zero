#include "render/RenderSystem.hpp"
#include "render/VolumePropagator.hpp"
#include "render/Instantiator.hpp"
#include "render/renderer/opengl/GLRenderingManager.hpp"
#include "render/renderer/pipeline/EntityRenderStage.hpp"
#include "render/renderer/pipeline/ShadowMapStage.hpp"
#include "render/renderer/pipeline/SkyDomeStage.hpp"
#include "core/Logger.hpp"
#include <fstream>


namespace zero::render
{

const char* RenderSystem::kTitle = "RenderSystem";

RenderSystem::RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config)
: zero::System(engine_core)
, config_(config)
, rendering_manager_(std::make_unique<GLRenderingManager>())
, model_loader_(std::make_unique<AssimpLoader>(rendering_manager_->GetModelManager()))
, render_pipeline_(std::make_unique<RenderPipeline>())
, scene_manager_(std::make_unique<SceneManager>())
, window_(std::make_unique<Window>(config.window_config_))
{
    LOG_VERBOSE(kTitle, "RenderSystem instance constructed");
}

void RenderSystem::Initialize()
{
    window_->Initialize();

    rendering_manager_->GetModelManager()->Initialize();
    rendering_manager_->GetRenderingContext()->Initialize(config_);
    rendering_manager_->GetTextureManager()->Initialize();
    rendering_manager_->GetUniformManager()->Initialize();

    LoadModels();
    LoadShaders();
    LoadTextures();

    render_pipeline_->AddRenderStage(std::make_unique<ShadowMapStage>(rendering_manager_.get()));
    render_pipeline_->AddRenderStage(std::make_unique<SkyDomeStage>(rendering_manager_.get()));
    render_pipeline_->AddRenderStage(std::make_unique<EntityRenderStage>(rendering_manager_.get()));
    render_pipeline_->Initialize();
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
    scene_manager_->UpdateView(GetCore()->GetRegistry(), time_delta);

    std::unique_ptr<IRenderView> render_view = scene_manager_->GetLatestView();

    LOG_VERBOSE(kTitle, "Updating light uniform buffers");
    rendering_manager_->GetUniformManager()->UpdateLightUniforms(render_view->GetDirectionalLights(),
                                                                 render_view->GetPointLights(),
                                                                 render_view->GetSpotLights());

    LOG_VERBOSE(kTitle, "Rendering IRenderView");
    render_pipeline_->Render(std::move(render_view));

    LOG_VERBOSE(kTitle, "Swapping buffers");
    window_->SwapBuffers();
}

void RenderSystem::PostUpdate()
{
    // Apply transforms to bounding volumes and expand parent volumes
    LOG_VERBOSE(kTitle, "Propagating entity volumes");
    VolumePropagator::PropagateVolume(GetCore()->GetRegistry());
}

void RenderSystem::ShutDown()
{
    LOG_VERBOSE(kTitle, "Clearing model and primitive data");
    rendering_manager_->GetModelManager()->ClearModels();
    rendering_manager_->GetModelManager()->ClearPrimitives();

    LOG_VERBOSE(kTitle, "Clearing texture data");
    rendering_manager_->GetTextureManager()->ReleaseTextures();

    LOG_VERBOSE(kTitle, "Clearing uniform buffer data");
    rendering_manager_->GetUniformManager()->Shutdown();

    LOG_VERBOSE(kTitle, "Cleaning up window and graphics context");
    window_->Cleanup();
}

Entity RenderSystem::CreateModelInstance(const std::string& model_filename)
{
    LOG_VERBOSE(kTitle, "Instantiating a new 3D model instance");
    auto model = rendering_manager_->GetModelManager()->GetModel(model_filename);
    if (!model)
    {
        LOG_WARN(kTitle, "Failed to instantiate 3D model. Returning NullEntity.");
        return NullEntity;
    }
    return Instantiator::InstantiateModel(GetCore()->GetRegistry(), model);
}

Entity RenderSystem::CreatePrimitiveInstance(const PrimitiveInstance& primitive)
{
    LOG_VERBOSE(kTitle, "Instantiating a new primitive instance");
    return Instantiator::InstantiatePrimitive(GetCore()->GetRegistry(), primitive);
}

Entity RenderSystem::CreateLightInstance(const Light& light, Entity entity)
{
    LOG_VERBOSE(kTitle, "Instantiating a new light instance");
    return Instantiator::InstantiateLight(GetCore()->GetRegistry(), light, entity);
}

void RenderSystem::LoadModels()
{
    AssetManager& asset_manager = GetCore()->GetAssetManager();

    const std::vector<std::string>& model_files = asset_manager.GetModelFiles();
    LOG_DEBUG(kTitle, "Loading 3D models. Model count: " + std::to_string(model_files.size()));
    for (const auto& model_file : model_files)
    {
        std::string model_file_path = asset_manager.GetModelFilePath(model_file);
        model_loader_->LoadModel(model_file, model_file_path);
    }
}

void RenderSystem::LoadShaders()
{
    IShaderManager* shader_manager = rendering_manager_->GetShaderManager();
    AssetManager& asset_manager = GetCore()->GetAssetManager();

    std::string compile_error_message;

    // Initialize vertex shaders
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

        if (!shader_manager->InitializeShader(stage))
        {
            LOG_ERROR(kTitle, "Failed to initialize shader: " + fully_qualified_file);
        }
    }

    // Initialize fragment shaders
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

        if (!shader_manager->InitializeShader(stage))
        {
            LOG_ERROR(kTitle, "Failed to initialize shader: " + fully_qualified_file);
        }
    }
}

void RenderSystem::LoadTextures()
{
    ITextureManager* texture_manager = rendering_manager_->GetTextureManager();
    AssetManager& asset_manager = GetCore()->GetAssetManager();

    const std::vector<std::string>& texture_files = asset_manager.GetTextureFiles();
    LOG_DEBUG(kTitle, "Pre-loading textures. Texture count: " + std::to_string(texture_files.size()));

    for (const auto& texture_file : texture_files)
    {
        if (!texture_manager->LoadTexture(texture_file, asset_manager.GetTextureFilePath(texture_file)))
        {
            LOG_ERROR(kTitle, "Failed to load graphics texture: " + texture_file);
        }
    }
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

bool RenderSystem::ContainsCamera() const
{
    auto camera_view = GetCore()->GetRegistry().view<const Camera>();
    return camera_view.size() > 0;
}

} // namespace zero::render