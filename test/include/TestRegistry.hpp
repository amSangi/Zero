#pragma once

#include <gtest/gtest.h>
#include <entt.hpp>
#include <vector>
#include "core/Component.hpp"
#include "render/Components.hpp"

class TestRegistry : public ::testing::Test {
public:

    void SetUp() override;

    /**
     * @brief Create a humanoid entity at the given position facing forward (+z).
     *     Only creates Transform and Volume components.
     * @param position the central position in world coordinates
     * @param parent_radius the initial parent sphere volume radius. May not initially encapsulate all child entities
     *     for testing purposes. The default radius is the minimum radius to encapsulate all child entities.
     * @return the body entity
     */
    zero::Component::Entity GenerateHumanoid(zero::math::Vec3f position, float parent_radius = 6.0f);

    /**
     * @brief Does the parent volume contain all of its child volumes?
     * @param parent the parent entity
     * @return True if the parent entity encapsulates all child volumes
     */
    bool ParentContainsChildVolumes(zero::Component::Entity parent) const;

protected:
    static constexpr zero::uint32 kEntityCount = 100;
    static constexpr float kMinPositionRange = -500.0f;
    static constexpr float kMaxPositionRange = 500.0f;
    std::vector<zero::Component::Entity> humanoids_;
    entt::registry registry_;

}; // class TestRegistry