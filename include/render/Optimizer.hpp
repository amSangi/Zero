#pragma once

#include "core/Component.hpp"
#include "render/Camera.hpp"

namespace zero::render
{

    class Optimizer
    {
    public:
        Optimizer() = delete;

        static std::vector<Entity> ExtractRenderableEntities(const Camera& camera, const entt::registry& registry);

    private:
        /**
         * @brief Get all non-culled entities that are renderable.
         *
         * An entity is viewable if it is not culled by the camera and if it contains either a Model mesh or Primitive
         * mesh.
         * Required components: Transform, Material, Volume, (ModelInstance or PrimitiveInstance)
         *
         * @param registry the registry containing all the entities and their components
         * @param camera the camera render to
         * @return all viewable entities
         */
        static std::vector<Entity> GetViewableEntities(const Camera& camera, const entt::registry& registry);

    }; // class Optimizer

} // namespace zero::render
