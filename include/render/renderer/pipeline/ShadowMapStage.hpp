#pragma once

#include "render/renderer/CascadedShadowMap.hpp"
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

        void Execute(IRenderView* render_view) override;

    private:
        /**
         * @brief Set the shadow casting directional light
         * @param render_view the render view containing all the renderables, lights, and sky dome for the frame
         * @param out_directional_light the directional light to set
         * @return True if a shadow casting directional light exists in the game. Otherwise false.
         */
        bool GetShadowCastingDirectionalLight(IRenderView* render_view, DirectionalLight& out_directional_light);

        void RenderEntities(const math::Matrix4x4& light_view_matrix,
                            const std::vector<Entity>& viewable_entities,
                            const entt::registry& registry,
                            const TimeDelta& time_delta);

        static const std::string kShadowMapFragmentShaderName;
        IRenderingManager* rendering_manager_;
        CascadedShadowMap cascaded_shadow_map_;
        std::shared_ptr<ISampler> diffuse_map_sampler_;

    }; // class ShadowMapStage

} // namespace zero::render