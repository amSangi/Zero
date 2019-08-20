#pragma once

#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <random>
#include <vector>
#include "core/Component.hpp"
#include "render/Components.hpp"

class TestRegistry : public ::testing::Test {
public:

    void SetUp() override;

    /**
     * @brief Generate a random position in 3D space given a range
     * @param min the min vector range
     * @param max the max vector range
     * @return a random vector within the specified range
     */
    zero::math::Vec3f GenerateRandomPosition(zero::math::Vec3f min, zero::math::Vec3f max);

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
    std::mt19937 random_generator_;
    entt::registry registry_;

}; // class TestRegistry