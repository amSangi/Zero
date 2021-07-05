#pragma once

#include <array>
#include <memory>
#include "component/Animator.hpp"
#include "component/Component.hpp"
#include "core/TimeDelta.hpp"
#include "render/Constants.hpp"
#include "render/IRenderView.hpp"

namespace zero::render
{

    class SceneManager
    {
    public:
        SceneManager();
        ~SceneManager();
        void UpdateView(entt::registry& registry, const TimeDelta& time_delta);
        std::unique_ptr<IRenderView> GetLatestView();

    private:
        const Camera& GetPrimaryCamera(const entt::registry& registry) const;
        SkyDome GetSkyDome(const entt::registry& registry) const;
        std::vector<DirectionalLight> GetDirectionalLights(const entt::registry& registry) const;
        std::vector<std::pair<PointLight, Transform>> GetPointLights(const entt::registry& registry) const;
        std::vector<std::pair<SpotLight, Transform>> GetSpotLights(const entt::registry& registry) const;

        std::vector<std::shared_ptr<IRenderable>> GetRenderables(const Camera& camera, const entt::registry& registry);
        std::array<std::vector<std::shared_ptr<IRenderable>>, Constants::kShadowCascadeCount> GetShadowCastingRenderables(const entt::registry& registry);
        std::shared_ptr<std::vector<math::Matrix4x4>> GetBoneMatrices(const entt::registry& registry, const Animated& animated_component);

        std::unique_ptr<IRenderView> render_view_;
        std::shared_ptr<CascadedShadowMap> cascaded_shadow_map_;
        std::unordered_map<Entity, std::shared_ptr<IRenderable>> renderable_cache_;
        std::unordered_map<Entity, std::shared_ptr<std::vector<math::Matrix4x4>>> bone_matrix_cache_;

    }; // class SceneManager

} // namespace zero