#pragma once

#include <entt/entt.hpp>
#include "component/Camera.hpp"

namespace zero::render
{

    class IRenderPass
    {
    public:
        virtual ~IRenderPass() = default;
        virtual void Execute(const Camera& camera,
                             entt::registry& registry,
                             const std::vector<Entity>& viewable_entities) = 0;
    }; // interface IRenderPass

} // namespace zero::render