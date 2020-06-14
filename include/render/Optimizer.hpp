#pragma once

#include "components/Component.hpp"

namespace zero
{
    // Forward declarations
    class Camera;

namespace render
{

    /**
     * @brief Optimize the render call by retrieving all visible entities.
     */
    class Optimizer
    {
    public:
        Optimizer() = delete;

        /**
         * @brief Return an optimized list of renderable entities
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
