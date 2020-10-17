#pragma once

#include "component/Component.hpp"

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
     * Current techniques employed:
     * - View frustrum culling
     *
     * Techniques that need to be implemented:
     * - Occlusion culling - Ignore entities that are completely occluded by other entities
     *
     */
    class Optimizer
    {
    public:
        Optimizer() = delete;

        /**
         * @brief Retrieve all entities that are renderable by the camera
         *
         * Culling Strategies Performed:
         * - View frustrum culling
         *
         * Techniques that need to be implemented:
         * - Occlusion culling
         *
         * @param camera the camera to render to
         * @param registry the registry containing all the entities and their components
         * @return a list of renderable entities
         */
        static std::vector<Entity> ExtractRenderableEntities(const Camera& camera, const entt::registry& registry);

    private:
        /**
         * @brief Get all non-culled entities that are renderable.
         *
         * An entity is viewable if its volume is not culled by the camera and if it contains either a Model mesh or Primitive
         * mesh.
         * Required components: Transform, Material, Volume, (ModelInstance or PrimitiveInstance)
         *
         * @param registry the registry containing all the entities and their components
         * @param camera the camera render to
         * @return all viewable entities
         */
        static std::vector<Entity> GetViewableEntities(const Camera& camera, const entt::registry& registry);

    }; // class Optimizer

} // namespace render
} // namespace zero
