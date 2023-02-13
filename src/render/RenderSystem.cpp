#include "render/RenderSystem.hpp"
#include "render/EntityFactory.hpp"
#include "render/renderer/opengl/GLRenderHardware.hpp"
#include "component/Camera.hpp"
#include "component/Mesh.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

const char* RenderSystem::kTitle = "RenderSystem";

RenderSystem::RenderSystem(EngineCore* engine_core, const RenderSystemConfig& config)
: zero::System(engine_core)
, config_(config)
, rhi_(std::make_unique<GLRenderHardware>())
, window_(std::make_unique<Window>(config.window_config_))
, rendering_pipeline_(std::make_unique<RenderingPipeline>())
, scene_manager_(std::make_unique<SceneManager>())
, model_cache_()
{
    LOG_VERBOSE(kTitle, "RenderSystem instance constructed");
}

void RenderSystem::Initialize()
{
	LOG_VERBOSE(kTitle, "Initializing Window");
	window_->Initialize();

	LOG_VERBOSE(kTitle, "Initializing Render Hardware Interface");
	rhi_->Initialize();

    LoadModels();

	LOG_VERBOSE(kTitle, "Initializing rendering pipeline");
	rendering_pipeline_->Initialize(rhi_.get(), GetCore()->GetAssetManager());
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

	LOG_VERBOSE(kTitle, "Generating Draw Calls");
	GenerateDrawCalls(render_view.get());

	LOG_VERBOSE(kTitle, "Sorting Draw Calls");
	rendering_pipeline_->Sort();

	LOG_VERBOSE(kTitle, "Executing Draw Calls");
	rendering_pipeline_->Render(render_view.get(), rhi_.get());

	LOG_VERBOSE(kTitle, "Swapping buffers");
	window_->SwapBuffers();

	LOG_VERBOSE(kTitle, "Clearing Render Queue");
	rendering_pipeline_->ClearRenderCalls();
}

void RenderSystem::PostUpdate()
{
}

void RenderSystem::ShutDown()
{
    LOG_VERBOSE(kTitle, "Clearing model cache");
	model_cache_.clear();

	LOG_VERBOSE(kTitle, "Shutting down rendering pipeline");
	rendering_pipeline_->Shutdown();

    LOG_VERBOSE(kTitle, "Shutting down rendering hardware");
    rhi_->Shutdown();

    LOG_VERBOSE(kTitle, "Cleaning up window and graphics context");
    window_->Cleanup();
}

Entity RenderSystem::CreateModelInstance(const std::string& model_filename, Entity parent_entity)
{
    LOG_VERBOSE(kTitle, "Instantiating a new model");
	auto model_search = model_cache_.find(model_filename);
	if (model_search == model_cache_.end())
	{
		LOG_ERROR(kTitle, "The model has not been loaded yet. Failed to instantiate model: " + model_filename);
		return NullEntity;
	}
	return EntityFactory::InstantiateModel(GetCore()->GetRegistry(), model_search->second, parent_entity);
}

Entity RenderSystem::CreatePrimitiveInstance(const PrimitiveInstance& primitive)
{
    LOG_VERBOSE(kTitle, "Instantiating a new primitive");
    return EntityFactory::InstantiatePrimitive(GetCore()->GetRegistry(), rendering_pipeline_->GetPrimitiveMeshId(primitive.GetType()), primitive);
}

Entity RenderSystem::CreateLightInstance(const Light& light, Entity entity)
{
    LOG_VERBOSE(kTitle, "Instantiating a new light source");
    return EntityFactory::InstantiateLight(GetCore()->GetRegistry(), light, entity);
}

void RenderSystem::LoadModels()
{
    AssetManager& asset_manager = GetCore()->GetAssetManager();

    const std::vector<std::string>& model_files = asset_manager.GetModelFiles();
    LOG_DEBUG(kTitle, "Loading 3D assets. Asset count: " + std::to_string(model_files.size()));
    for (const std::string& model_file : model_files)
    {
        std::string model_file_path = asset_manager.GetModelFilePath(model_file);
        std::unique_ptr<Model> model = ModelLoader::LoadModel(model_file, model_file_path);
        if (model)
        {
	        LOG_DEBUG(kTitle, "Loading 3D asset geometry onto the GPU for asset: " + model_file_path);
	        for (GeometryData& geometry_data: model->geometries_)
			{
				geometry_data.geometry_id_ = rendering_pipeline_->LoadMesh(rhi_.get(), geometry_data.mesh_data_.get());
			}
	        model_cache_.emplace(model_file_path, std::move(model));
        }
        else
        {
            LOG_ERROR(kTitle, "Failed to load 3D asset: " + model_file_path);
        }
    }
}

void RenderSystem::GenerateDrawCalls(IRenderView* render_view)
{
	entt::registry& registry = GetCore()->GetRegistry();
	auto drawable_view = registry.view<const Transform, const Material, const Mesh, const Volume>();

	// Generate SkyDome Draw Call
	rendering_pipeline_->GenerateSkyDomeDrawCall(rhi_.get(), render_view->GetCamera(), render_view->GetSkyDome());

	const math::Matrix4x4 view_matrix = render_view->GetCamera().GetViewMatrix();

	// Generate Draw Calls for Renderable entities
	const std::vector<std::shared_ptr<ITexture>> shadow_map_textures = rhi_->GetShadowMapTextures();
	for (Entity renderable_entity: render_view->GetRenderableEntities())
	{
		const auto& [transform, material, mesh, _] = drawable_view.get(renderable_entity);
		math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
		rendering_pipeline_->GenerateDrawCall(rhi_.get(), mesh, material, model_matrix, view_matrix);
	}

	// Generate Draw Calls for Shadow Casting entities for each cascade
	for (uint32 cascade_index = 0; cascade_index < Constants::kShadowCascadeCount; ++cascade_index)
	{
		for (Entity shadow_casting_entity: render_view->GetShadowCastingEntities(cascade_index))
		{
			const auto& [transform, material, mesh, _] = drawable_view.get(shadow_casting_entity);
			math::Matrix4x4 model_matrix = transform.GetLocalToWorldMatrix();
			rendering_pipeline_->GenerateShadowDrawCall(rhi_.get(), cascade_index, mesh, material, model_matrix);
		}
	}
}

bool RenderSystem::ContainsCamera() const
{
    auto camera_view = GetCore()->GetRegistry().view<const Camera>();
    return !camera_view.empty();
}

} // namespace zero::render