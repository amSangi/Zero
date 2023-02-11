#pragma once

#include <array>
#include <memory>
#include "component/Camera.hpp"
#include "component/SkyDome.hpp"
#include "component/Light.hpp"
#include "component/Transform.hpp"
#include "core/TimeDelta.hpp"
#include "render/scene/RenderView.hpp"
#include "render/CascadedShadowMap.hpp"

namespace zero::render
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager() = default;
		void UpdateView(entt::registry& registry, const TimeDelta& time_delta);
		std::unique_ptr<RenderView> GetLatestView();
	private:
		[[nodiscard]] static const Camera& GetPrimaryCamera(const entt::registry& registry);
		[[nodiscard]] static SkyDome GetSkyDome(const entt::registry& registry);
		[[nodiscard]] static std::vector<DirectionalLight> GetDirectionalLights(const entt::registry& registry);
		[[nodiscard]] static std::vector<std::pair<PointLight, Transform>> GetPointLights(const entt::registry& registry);
		[[nodiscard]] static std::vector<std::pair<SpotLight, Transform>> GetSpotLights(const entt::registry& registry);
		std::vector<Entity> GetRenderableEntities(const Camera& camera, const entt::registry& registry);
		std::array<std::vector<Entity>, Constants::kShadowCascadeCount> GetShadowCastingEntities(const entt::registry& registry);

		std::unique_ptr<RenderView> render_view_;
		std::shared_ptr<CascadedShadowMap> cascaded_shadow_map_;
		std::set<Entity> renderable_cache_;
	}; // class SceneManager

} // namespace zero