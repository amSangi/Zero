#include <random>
#include "core/Transform.hpp"
#include "TestRegistry.hpp"

using namespace zero;
using namespace entt;

void TestRegistry::SetUp() {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<float> distribution(kMinPositionRange, kMaxPositionRange);
    for (int i = 0; i < kEntityCount; ++i) {
        math::Vec3f position(distribution(generator), distribution(generator), distribution(generator));
        humanoids_.push_back(GenerateHumanoid(position));
    }
}

Component::Entity TestRegistry::GenerateHumanoid(math::Vec3f position, float parent_radius) {
    // Body - located at the given position
    auto body_entity = registry_.create();
    Transform body_transform(position, math::Vec3f::One(), math::Quaternion());
    registry_.assign<Transform>(body_entity, body_transform);
    registry_.assign<render::Volume>(body_entity, position, parent_radius);

    // Head - 4 units above body
    auto head_entity = registry_.create();
    math::Vec3f head_offset(0.0f, 4.0f, 0.0f);
    registry_.assign<Transform>(head_entity,
                                body_entity,
                                body_transform,
                                head_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<render::Volume>(head_entity,
                                     position + head_offset,
                                     2.0f);

    // Left Arm
    auto left_arm_entity = registry_.create();
    math::Vec3f left_arm_offset(2.0f, 1.0f, 0.0f);
    registry_.assign<Transform>(left_arm_entity,
                                body_entity,
                                body_transform,
                                left_arm_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<render::Volume>(left_arm_entity,
                                     position + left_arm_offset,
                                     2.0f);

    // Right Arm
    auto right_arm_entity = registry_.create();
    math::Vec3f right_arm_offset(-2.0f, 1.0f, 0.0f);
    registry_.assign<Transform>(right_arm_entity,
                                body_entity,
                                body_transform,
                                right_arm_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<render::Volume>(right_arm_entity,
                                     position + right_arm_offset,
                                     2.0f);

    // Left Leg
    auto left_leg_entity = registry_.create();
    math::Vec3f left_leg_offset(1.0f, -2.0f, 0.0f);
    registry_.assign<Transform>(left_leg_entity,
                                body_entity,
                                body_transform,
                                left_leg_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<render::Volume>(left_leg_entity,
                                     position + left_leg_offset,
                                     2.0f);

    // Right Leg
    auto right_leg_entity = registry_.create();
    math::Vec3f right_leg_offset(-1.0f, -2.0f, 0.0f);
    registry_.assign<Transform>(right_leg_entity,
                                body_entity,
                                body_transform,
                                right_leg_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<render::Volume>(right_leg_entity,
                                     position + right_leg_offset,
                                     2.0f);

    // Add child entities
    auto& parent_transform = registry_.get<Transform>(body_entity);
    parent_transform.children_.push_back(head_entity);
    parent_transform.children_.push_back(left_arm_entity);
    parent_transform.children_.push_back(right_arm_entity);
    parent_transform.children_.push_back(left_leg_entity);
    parent_transform.children_.push_back(right_leg_entity);

    return body_entity;
}

bool TestRegistry::ParentContainsChildVolumes(Component::Entity parent) const {
    auto &&[parent_transform, parent_volume] = registry_.get<Transform, render::Volume>(parent);

    for (auto child_entity : parent_transform.children_) {
        auto& child_volume = registry_.get<render::Volume>(child_entity);
        if (!parent_volume.bounding_volume_.Contains(child_volume.bounding_volume_)
        || !ParentContainsChildVolumes(child_entity)) {
            return false;
        }
    }
    return true;
}