#pragma once

#include "render/CascadedShadowMap.hpp"
#include "render/renderer/IRenderStage.hpp"
#include "render/renderer/IRenderingManager.hpp"
#include "component/Light.hpp"

namespace zero::render
{
    class ShadowMapStage : public RenderStage
    {
    public:
        explicit ShadowMapStage(IRenderingManager* rendering_manager);
        ~ShadowMapStage() override = default;

        void Initialize() override;
        void Execute(IRenderView* render_view) override;

    private:
        /**
         * @brief Set the shadow casting directional light
         * @param directional_lights the list of directional lights
         * @param out_directional_light the directional light to set
         * @return True if a shadow casting directional light exists in the game. Otherwise false.
         */
        bool GetShadowCastingDirectionalLight(const std::vector<DirectionalLight>& directional_lights, DirectionalLight& out_directional_light);

        void RenderEntities(const math::Matrix4x4& light_view_matrix,
                            const std::vector<std::shared_ptr<IRenderable>>& renderables,
                            const TimeDelta& time_delta);

        static const std::string kShadowMapFragmentShaderName;
        IRenderingManager* rendering_manager_;
        std::shared_ptr<ISampler> diffuse_map_sampler_;

    }; // class ShadowMapStage

} // namespace zero::render