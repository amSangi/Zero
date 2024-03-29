#pragma once

#include "component/Component.hpp"
#include "math/Box.hpp"
#include "IViewVolume.hpp"

namespace zero
{
    // Forward declarations
    class Camera;

namespace render
{

    /**
     * @brief Optimize the rendering passes by only retrieving required entities.
     *
     * These functions perform different culling techniques on entities in the scene.
     *
     * Current techniques performed:
     * - View frustum culling
     *
     * Techniques that need to be implemented:
     * - Occlusion culling - Ignore entities that are completely occluded by other entities
     *
     */
    class CullingManager
    {
    public:
        CullingManager() = delete;

        /**
         * @brief Retrieve all entities that are renderable by the camera
         *
         * Culling Strategies Performed:
         * - View frustum culling
         *
         * Renderable entities without the following components will be culled:
         *     - Transform                          (Used for positioning)
         *     - Material                           (Used for rendering)
         *     - Volume                             (Used for culling)
         *     - Mesh                               (Used for mesh data)
         *
         * @param camera the camera to render to
         * @param registry the registry containing all the entities and their components
         * @return a list of renderable entities
         */
        static std::vector<Entity> GetRenderableEntities(const Camera& camera, const entt::registry& registry);

        /**
         * @brief Retrieve all entities that emit shadows and are not culled by the given box
         *
         * Renderable entities without the following components will be culled:
         *     - Transform                          (Used for positioning)
         *     - Material                           (Used for rendering)
         *     - Volume                             (Used for culling)
         *     - Mesh                               (Used for mesh data)
         *
         * @param box the boundaries of the directional light
         * @param registry the registry containing all the entities and their components
         * @return a list of renderable entities
         */
        static std::vector<Entity> GetShadowCastingEntities(const math::Box& box, const entt::registry& registry);

    private:
        static std::vector<Entity> GetViewableEntities(const Camera& camera, const entt::registry& registry);
        static std::vector<Entity> CullEntities(const IViewVolume* culler, const entt::registry& registry);

    }; // class CullingManager

} // namespace render
} // namespace zero
