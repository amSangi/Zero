#pragma once

#include "render/CascadedShadowMap.hpp"
#include "render/IRenderPass.hpp"
#include "render/IRenderingManager.hpp"
#include "component/Light.hpp"

namespace zero::render
{
    class ShadowMapPass : public RenderPass
    {
    public:
        explicit ShadowMapPass(IRenderingManager* rendering_manager);
        ~ShadowMapPass() override = default;

        void Execute(const Camera& camera, const entt::registry& registry, const TimeDelta& time_delta) override;

    private:
        /**
         * @brief Set the shadow casting directional light
         * @param registry the registry containing all the game entities and their components
         * @param out_directional_light the directional light to set
         * @return True if a shadow casting directional light exists in the game. Otherwise false.
         */
        bool GetShadowCastingDirectionalLight(const entt::registry& registry, DirectionalLight& out_directional_light);

        void RenderEntities(const math::Matrix4x4& light_view_matrix,
                            const std::vector<Entity>& viewable_entities,
                            const entt::registry& registry,
                            const TimeDelta& time_delta);

        static const std::string kShadowMapFragmentShaderName;
        IRenderingManager* rendering_manager_;
        CascadedShadowMap cascaded_shadow_map_;

    }; // class ShadowMapPass

} // namespace zero::render