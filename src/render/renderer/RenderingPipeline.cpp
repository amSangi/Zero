#include <fstream>
#include "render/renderer/RenderingPipeline.hpp"
#include "render/renderer/UniformBufferData.hpp"
#include "render/renderer/renderpass/EntityRenderPass.hpp"
#include "render/renderer/renderpass/CascadedShadowMapRenderPass.hpp"
#include "render/renderer/drawcall/EntityDrawCall.hpp"
#include "render/renderer/drawcall/ShadowMapDrawCall.hpp"
#include "render/renderer/drawcall/SkyDomeDrawCall.hpp"
#include "render/MeshGenerator.hpp"
#include "core/Logger.hpp"

namespace zero::render
{

const char* RenderingPipeline::kTitle = "RenderingPipeline";


constexpr uint8 kBoxMeshIdIndex = 0;
constexpr uint8 kConeMeshIdIndex = 1;
constexpr uint8 kCylinderMeshIdIndex = 2;
constexpr uint8 kPlaneMeshIdIndex = 3;
constexpr uint8 kSphereMeshIdIndex = 4;
constexpr uint8 kTorusMeshIdIndex = 5;

RenderingPipeline::RenderingPipeline()
: mesh_cache_()
, program_cache_()
, shader_cache_()
, texture_cache_()
, unique_identifier_(1)
, primitive_mesh_id_cache_()
, render_passes_()
, entity_render_pass_index_(Constants::kShadowCascadeCount)
, uniform_manager_(nullptr)
{
    shadow_map_material_.SetShaders({"model.vertex.glsl", "shadow_map.fragment.glsl"});
}

uint32 RenderingPipeline::LoadMesh(IRenderHardware* rhi, MeshData* mesh_data)
{
    std::shared_ptr<IMesh> gpu_mesh = rhi->CreateMesh(mesh_data);
    uint32 mesh_id = GenerateNewUniqueIdentifier();
    mesh_cache_.emplace(mesh_id, gpu_mesh);
    return mesh_id;
}

void RenderingPipeline::Initialize(IRenderHardware* rhi, AssetManager& asset_manager)
{
    LoadPrimitiveMeshes(rhi);
    LoadShaders(rhi, asset_manager);
    LoadTextures(rhi, asset_manager);
    uniform_manager_ = std::make_shared<UniformManager>();
    uniform_manager_->Initialize(rhi);

    for (uint32 cascade_index = 0; cascade_index < Constants::kShadowCascadeCount; ++cascade_index)
    {
        std::unique_ptr<CascadedShadowMapRenderPass> shadow_map_render_pass = std::make_unique<CascadedShadowMapRenderPass>(cascade_index);
        shadow_map_render_pass->Initialize(rhi, uniform_manager_->GetCameraUniform());
        render_passes_.push_back(std::move(shadow_map_render_pass));
    }
    std::unique_ptr<EntityRenderPass> entity_render_pass = std::make_unique<EntityRenderPass>();
    entity_render_pass->Initialize(rhi, uniform_manager_->GetCameraUniform());
    render_passes_.push_back(std::move(entity_render_pass));
}

void RenderingPipeline::Shutdown()
{
    LOG_VERBOSE(kTitle, "Clearing mesh cache");
    mesh_cache_.clear();
    LOG_VERBOSE(kTitle, "Clearing program cache");
    program_cache_.clear();
    LOG_VERBOSE(kTitle, "Clearing shader cache");
    shader_cache_.clear();
    LOG_VERBOSE(kTitle, "Clearing texture cache");
    texture_cache_.clear();
    ClearRenderCalls();
    uniform_manager_ = nullptr;
}

uint32 RenderingPipeline::GetPrimitiveMeshId(PrimitiveInstance::Type primitive_type) const
{
    switch (primitive_type)
    {
        case PrimitiveInstance::Type::BOX:
            return primitive_mesh_id_cache_[kBoxMeshIdIndex];
        case PrimitiveInstance::Type::CONE:
            return primitive_mesh_id_cache_[kConeMeshIdIndex];
        case PrimitiveInstance::Type::CYLINDER:
            return primitive_mesh_id_cache_[kCylinderMeshIdIndex];
        case PrimitiveInstance::Type::PLANE:
            return primitive_mesh_id_cache_[kPlaneMeshIdIndex];
        case PrimitiveInstance::Type::SPHERE:
            return primitive_mesh_id_cache_[kSphereMeshIdIndex];
        case PrimitiveInstance::Type::TORUS:
            return primitive_mesh_id_cache_[kTorusMeshIdIndex];
        default:
            // Box Mesh
            LOG_WARN(kTitle, "GetPrimitiveMeshId called with invalid primitive type. Returning box mesh by default.");
            return primitive_mesh_id_cache_[kBoxMeshIdIndex];
    }
}

void RenderingPipeline::GenerateSkyDomeDrawCall(IRenderHardware *rhi, const Camera& camera, const SkyDome& sky_dome)
{
    std::shared_ptr<IProgram> program = GenerateShaderProgram(rhi, sky_dome.GetShaderID(), {sky_dome.GetVertexShader(), sky_dome.GetFragmentShader()});
    if (program == nullptr)
    {
        LOG_ERROR(kTitle, "Failed to generate shader program for sky dome. The sky dome will not be rendered.");
        return;
    }

    // Sphere scaled and centered around the camera
    constexpr float kSkyDomeSphereScale = 1000.0F;
    math::Matrix4x4 model_matrix = math::Matrix4x4::Identity()
            .Scale(math::Vec3f(kSkyDomeSphereScale))
            .Translate(camera.position_);
    ModelData model_data{model_matrix, math::Matrix4x4::Identity()};
    std::shared_ptr<IMesh> sky_dome_mesh = mesh_cache_[primitive_mesh_id_cache_[kSphereMeshIdIndex]];
    render_passes_[entity_render_pass_index_]->Submit(std::make_unique<SkyDomeDrawCall>(model_data,
                                                                                        sky_dome.apex_color_,
                                                                                        sky_dome.center_color_,
                                                                                        sky_dome_mesh,
                                                                                        program,
                                                                                        uniform_manager_));
}

void RenderingPipeline::GenerateDrawCall(IRenderHardware* rhi,
                                         const Mesh& mesh,
                                         const Material& material,
                                         math::Matrix4x4 model_matrix,
                                         math::Matrix4x4 view_matrix)
{
    // Retrieve the mesh
    auto mesh_search = mesh_cache_.find(mesh.mesh_id_);
    if (mesh_search == mesh_cache_.end())
    {
        LOG_ERROR(kTitle, "Failed to retrieve the mesh used by the entity. The entity will not be rendered.");
        return;
    }

    // Retrieve the shader program used by the entity
    std::shared_ptr<IProgram> program = GenerateShaderProgram(rhi, material.GetShaderID(), material.GetShadersAsList());
    if (program == nullptr)
    {
        LOG_ERROR(kTitle, "Failed to generate shader program for entity. The entity will not be rendered.");
        return;
    }

    // Construct ModelData
    ModelData model_data{model_matrix, (view_matrix * model_matrix).Inverse()};
    std::unique_ptr<IDrawCall> draw_call = std::make_unique<EntityDrawCall>(mesh.mesh_id_,
                                                                            material,
                                                                            model_data,
                                                                            mesh_search->second,
                                                                            program,
                                                                            rhi->GetDiffuseMapSampler(),
                                                                            rhi->GetShadowMapSampler(),
                                                                            uniform_manager_,
                                                                            texture_cache_[material.GetTextureMap().diffuse_map_]);
    render_passes_[entity_render_pass_index_]->Submit(std::move(draw_call));
}

void RenderingPipeline::GenerateShadowDrawCall(IRenderHardware* rhi,
                                               uint32 cascade_index,
                                               const Mesh& mesh,
                                               const Material& material,
                                               math::Matrix4x4 model_matrix)
{
    // Retrieve the mesh
    auto mesh_search = mesh_cache_.find(mesh.mesh_id_);
    if (mesh_search == mesh_cache_.end())
    {
        LOG_ERROR(kTitle, "Failed to retrieve the mesh used by the entity. The entity will not be rendered.");
        return;
    }

    // Retrieve the shader program used by the entity
    std::shared_ptr<IProgram> program = GenerateShaderProgram(rhi, shadow_map_material_.GetShaderID(), shadow_map_material_.GetShadersAsList());
    if (program == nullptr)
    {
        LOG_ERROR(kTitle, "Failed to generate shader program for entity. The entity will not be rendered.");
        return;
    }

    // Construct ModelData. Normal matrix is not needed for shadow maps
    ModelData model_data{model_matrix, math::Matrix4x4::Identity()};
    std::unique_ptr<IDrawCall> draw_call = std::make_unique<ShadowMapDrawCall>(mesh.mesh_id_,
                                                                               material,
                                                                               model_data,
                                                                               mesh_search->second,
                                                                               program,
                                                                               uniform_manager_);
    render_passes_[cascade_index]->Submit(std::move(draw_call));
}

void RenderingPipeline::Sort()
{
    for (const std::unique_ptr<IRenderPass>& render_pass : render_passes_)
    {
        render_pass->Sort();
    }
}

void RenderingPipeline::Render(IRenderView* render_view, IRenderHardware* rhi)
{
    UpdateLightUniforms(render_view, rhi);
    UpdateShadowMapUniform(render_view, rhi);
    for (const std::unique_ptr<IRenderPass>& render_pass : render_passes_)
    {
        render_pass->Render(render_view, rhi);
    }
}

void RenderingPipeline::ClearRenderCalls()
{
    for (const std::unique_ptr<IRenderPass>& render_pass : render_passes_)
    {
        render_pass->ClearDrawCalls();
    }
}

void RenderingPipeline::UpdateLightUniforms(IRenderView* render_view, IRenderHardware* rhi)
{
    assert(render_view->GetDirectionalLights().size() <= Constants::kMaxDirectionalLights);
    assert(render_view->GetPointLights().size() <= Constants::kMaxPointLights);
    assert(render_view->GetSpotLights().size() <= Constants::kMaxSpotLights);

    // Update light uniform data
    LightInformationData light_information_data{static_cast<uint32>(render_view->GetDirectionalLights().size()),
                                                static_cast<uint32>(render_view->GetPointLights().size()),
                                                static_cast<uint32>(render_view->GetSpotLights().size())};
    std::vector<DirectionalLightData> directional_light_data_list{};
    std::vector<PointLightData> point_light_data_list{};
    std::vector<SpotLightData> spot_light_data_list{};
    for (const DirectionalLight& directional_light: render_view->GetDirectionalLights())
    {
        directional_light_data_list.emplace_back(directional_light);
    }
    for (const auto& [point_light, transform]: render_view->GetPointLights())
    {
        point_light_data_list.emplace_back(transform, point_light);
    }
    for (const auto& [spot_light, transform]: render_view->GetSpotLights())
    {
        spot_light_data_list.emplace_back(transform, spot_light);
    }
    rhi->UpdateUniformData(uniform_manager_->GetLightInformationUniform(), &light_information_data, sizeof(light_information_data), 0);
    rhi->UpdateUniformData(uniform_manager_->GetDirectionalLightUniform(), directional_light_data_list.data(), sizeof(DirectionalLightData) * directional_light_data_list.size(), 0);
    rhi->UpdateUniformData(uniform_manager_->GetPointLightUniform(), point_light_data_list.data(), sizeof(PointLightData) * point_light_data_list.size(), 0);
    rhi->UpdateUniformData(uniform_manager_->GetSpotLightUniform(), spot_light_data_list.data(), sizeof(SpotLightData) * spot_light_data_list.size(), 0);
}

void RenderingPipeline::UpdateShadowMapUniform(IRenderView* render_view, IRenderHardware* rhi)
{
    ShadowMapInformation shadow_map_information{render_view->GetCascadedShadowMap().GetTextureMatrices(),
                                                render_view->GetCascadedShadowMap().GetViewFarBounds()};
    rhi->UpdateUniformData(uniform_manager_->GetShadowMapUniform(), &shadow_map_information, sizeof(ShadowMapInformation), 0);
}

void RenderingPipeline::LoadPrimitiveMeshes(IRenderHardware* rhi)
{
    std::unique_ptr<MeshData> box_mesh_data = MeshGenerator::GenerateBox();
    std::unique_ptr<MeshData> cone_mesh_data = MeshGenerator::GenerateCone(Cone{});
    std::unique_ptr<MeshData> cylinder_mesh_data = MeshGenerator::GenerateCylinder(Cylinder{});
    std::unique_ptr<MeshData> plane_mesh_data = MeshGenerator::GeneratePlane(Plane{});
    std::unique_ptr<MeshData> sphere_mesh_data = MeshGenerator::GenerateSphere(Sphere{});
    std::unique_ptr<MeshData> torus_mesh_data = MeshGenerator::GenerateTorus(Torus{});

    std::shared_ptr<IMesh> box_mesh = rhi->CreateMesh(box_mesh_data.get());
    std::shared_ptr<IMesh> cone_mesh = rhi->CreateMesh(cone_mesh_data.get());
    std::shared_ptr<IMesh> cylinder_mesh = rhi->CreateMesh(cylinder_mesh_data.get());
    std::shared_ptr<IMesh> plane_mesh = rhi->CreateMesh(plane_mesh_data.get());
    std::shared_ptr<IMesh> sphere_mesh = rhi->CreateMesh(sphere_mesh_data.get());
    std::shared_ptr<IMesh> torus_mesh = rhi->CreateMesh(torus_mesh_data.get());

    uint32 box_mesh_id = GenerateNewUniqueIdentifier();
    mesh_cache_.emplace(box_mesh_id, box_mesh);
    primitive_mesh_id_cache_[kBoxMeshIdIndex] = box_mesh_id;

    uint32 cone_mesh_id = GenerateNewUniqueIdentifier();
    mesh_cache_.emplace(cone_mesh_id, cone_mesh);
    primitive_mesh_id_cache_[kConeMeshIdIndex] = cone_mesh_id;

    uint32 cylinder_mesh_id = GenerateNewUniqueIdentifier();
    mesh_cache_.emplace(cylinder_mesh_id, cylinder_mesh);
    primitive_mesh_id_cache_[kCylinderMeshIdIndex] = cylinder_mesh_id;

    uint32 plane_mesh_id = GenerateNewUniqueIdentifier();
    mesh_cache_.emplace(plane_mesh_id, plane_mesh);
    primitive_mesh_id_cache_[kPlaneMeshIdIndex] = plane_mesh_id;

    uint32 sphere_mesh_id = GenerateNewUniqueIdentifier();
    mesh_cache_.emplace(sphere_mesh_id, sphere_mesh);
    primitive_mesh_id_cache_[kSphereMeshIdIndex] = sphere_mesh_id;

    uint32 torus_mesh_id = GenerateNewUniqueIdentifier();
    mesh_cache_.emplace(torus_mesh_id, torus_mesh);
    primitive_mesh_id_cache_[kTorusMeshIdIndex] = torus_mesh_id;
}

void RenderingPipeline::LoadTextures(IRenderHardware* rhi, AssetManager& asset_manager)
{
    const std::vector<std::string>& texture_files = asset_manager.GetTextureFiles();
    LOG_DEBUG(kTitle, "Pre-loading textures. Texture count: " + std::to_string(texture_files.size()));

    for (const auto& texture_file : texture_files)
    {
        std::string texture_file_path = asset_manager.GetTextureFilePath(texture_file);
        std::unique_ptr<Image> texture_image = std::make_unique<Image>(texture_file_path);
        texture_image->Load();
        std::shared_ptr<ITexture> texture = rhi->CreateTexture(std::move(texture_image));

        if (texture)
        {
            texture_cache_.emplace(texture_file, texture);
        }
        else
        {
            LOG_ERROR(kTitle, "Failed to load texture: " + texture_file_path);
        }
    }
}

void RenderingPipeline::LoadShaders(IRenderHardware* rhi, AssetManager& asset_manager)
{
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

        std::shared_ptr<IShader> shader = rhi->CreateShader(stage);
        if (shader)
        {
            shader_cache_.emplace(stage.name_, shader);
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

        std::shared_ptr<IShader> shader = rhi->CreateShader(stage);
        if (shader)
        {
            shader_cache_.emplace(stage.name_, shader);
        }
        else
        {
            LOG_ERROR(kTitle, "Failed to load shader: " + fully_qualified_file);
        }
    }
}

std::shared_ptr<IProgram> RenderingPipeline::GenerateShaderProgram(IRenderHardware* rhi, uint32 shader_id, const std::vector<std::string>& shader_name_list)
{
    auto program_search = program_cache_.find(shader_id);
    if (program_search != program_cache_.end())
    {
        return program_search->second;
    }
    std::vector<std::shared_ptr<IShader>> shader_list{};
    for (const std::string& shader_name: shader_name_list)
    {
        auto search = shader_cache_.find(shader_name);
        if (search != shader_cache_.end())
        {
            shader_list.push_back(search->second);
        }
    }
    std::shared_ptr<IProgram> program = rhi->CreateShaderProgram(shader_list);
    program_cache_.emplace(shader_id, program);
    return program;
}

uint32 RenderingPipeline::GenerateNewUniqueIdentifier()
{
    return unique_identifier_++;
}

void RenderingPipeline::ReadShaderSource(const std::string& filename, std::string& destination)
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