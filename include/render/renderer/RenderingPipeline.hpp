#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "core/NonCopyable.hpp"
#include "core/AssetManager.hpp"
#include "component/Material.hpp"
#include "component/Mesh.hpp"
#include "component/PrimitiveInstance.hpp"
#include "render/IRenderView.hpp"
#include "render/renderer/IRenderPass.hpp"
#include "render/renderer/UniformManager.hpp"

namespace zero::render
{
    /**
     * @brief Manage, sort, and execute a list of render calls
     */
    class RenderingPipeline : public NonCopyable
    {
    public:
        RenderingPipeline();
        ~RenderingPipeline() = default;

        uint32 LoadMesh(IRenderHardware* rhi, MeshData* mesh_data);
        void Initialize(IRenderHardware* rhi, AssetManager& asset_manager);
        void Shutdown();

        uint32 GetPrimitiveMeshId(IRenderHardware* rhi, PrimitiveInstance primitive_instance);
        void GenerateSkyDomeDrawCall(IRenderHardware* rhi, const Camera& camera, const SkyDome& sky_dome);
        void GenerateDrawCall(IRenderHardware* rhi,
                              const Mesh& mesh,
                              const Material& material,
                              math::Matrix4x4 model_matrix,
                              math::Matrix4x4 view_matrix);
        void GenerateShadowDrawCall(IRenderHardware* rhi,
                                    uint32 cascade_index,
                                    const Mesh& mesh,
                                    const Material& material,
                                    math::Matrix4x4 model_matrix);

        /**
         * @brief Sort the render passes
         */
        void Sort();

        /**
         * @brief Render the render calls
         * @param rhi the render hardware interface to use for rendering
         */
        void Render(IRenderView* render_view, IRenderHardware* rhi);

        /**
         * @brief Clear the rendering pipeline of all render calls
         */
        void ClearRenderCalls();

    private:
        void UpdateLightUniforms(IRenderView* render_view, IRenderHardware* rhi);
        void UpdateShadowMapUniform(IRenderView* render_view, IRenderHardware* rhi);
        void LoadPrimitiveMeshes(IRenderHardware* rhi);
        void LoadTextures(IRenderHardware* rhi, AssetManager& asset_manager);
        void LoadShaders(IRenderHardware* rhi, AssetManager& asset_manager);
        std::shared_ptr<IProgram> GenerateShaderProgram(IRenderHardware* rhi, uint32 shader_id, const std::vector<std::string>& shader_name_list);
        [[nodiscard]] uint32 GenerateNewUniqueIdentifier();
        static void ReadShaderSource(const std::string& filename, std::string& destination);

        /**
         * @brief The log title
         */
        static const char* kTitle;

        /**
         * @brief GPU Resource Cache
         */
        std::unordered_map<uint32, std::shared_ptr<IMesh>> mesh_cache_;
        std::unordered_map<uint32, std::shared_ptr<IProgram>> program_cache_;
        std::unordered_map<std::string, std::shared_ptr<IShader>> shader_cache_;
        std::unordered_map<std::string, std::shared_ptr<ITexture>> texture_cache_;
        uint32 unique_identifier_;
        std::array<uint32, 6> primitive_mesh_id_cache_;

        /**
         * @brief Active render passes, which maintain their own set of draw calls
         */
        std::vector<std::unique_ptr<IRenderPass>> render_passes_;
        const uint32 entity_render_pass_index_;
        Material shadow_map_material_{};
        std::shared_ptr<ITexture> empty_texture_;
        std::shared_ptr<UniformManager> uniform_manager_;

    }; // class RenderingPipeline

} // namespace zero::render