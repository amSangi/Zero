 #pragma once

#include <vector>
#include <entt/entt.hpp>

namespace zero
{

    using Entity = entt::entity;
    static constexpr auto NullEntity = entt::null;

    /**
     * @brief The Component base in the Entity-Component-System
     */
    struct Component {}; // struct Component

} // namespace zero