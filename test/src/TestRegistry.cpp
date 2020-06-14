#include "components/Transform.hpp"
#include "components/Volume.hpp"
#include "TestRegistry.hpp"

using namespace zero;
using namespace entt;

void TestRegistry::SetUp()
{
    std::random_device device;
    random_generator_ = std::mt19937(device());
}

math::Vec3f TestRegistry::GenerateRandomPosition(math::Vec3f min, math::Vec3f max)
{
    std::uniform_real_distribution<float> x_distribution(min.x_, max.x_);
    std::uniform_real_distribution<float> y_distribution(min.y_, max.y_);
    std::uniform_real_distribution<float> z_distribution(min.z_, max.z_);
    return math::Vec3f(x_distribution(random_generator_), y_distribution(random_generator_), z_distribution(random_generator_));
}

Entity TestRegistry::GenerateHumanoid(math::Vec3f position, float parent_radius)
{
    // Body - located at the given position
    Entity body_entity = registry_.create();
    Transform body_transform(position, math::Vec3f::One(), math::Quaternion());
    registry_.assign<Transform>(body_entity, body_transform);
    registry_.assign<Volume>(body_entity, position, parent_radius);

    // Head - 4 units above body
    Entity head_entity = registry_.create();
    math::Vec3f head_offset(0.0F, 4.0F, 0.0F);
    registry_.assign<Transform>(head_entity,
                                body_entity,
                                body_transform,
                                head_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<Volume>(head_entity,
                                     position + head_offset,
                                     2.0F);

    // Left Arm
    Entity left_arm_entity = registry_.create();
    math::Vec3f left_arm_offset(2.0F, 1.0F, 0.0F);
    registry_.assign<Transform>(left_arm_entity,
                                body_entity,
                                body_transform,
                                left_arm_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<Volume>(left_arm_entity,
                                     position + left_arm_offset,
                                     2.0F);

    // Right Arm
    Entity right_arm_entity = registry_.create();
    math::Vec3f right_arm_offset(-2.0F, 1.0F, 0.0F);
    registry_.assign<Transform>(right_arm_entity,
                                body_entity,
                                body_transform,
                                right_arm_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<Volume>(right_arm_entity,
                                     position + right_arm_offset,
                                     2.0F);

    // Left Leg
    Entity left_leg_entity = registry_.create();
    math::Vec3f left_leg_offset(1.0F, -2.0F, 0.0F);
    registry_.assign<Transform>(left_leg_entity,
                                body_entity,
                                body_transform,
                                left_leg_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<Volume>(left_leg_entity,
                                     position + left_leg_offset,
                                     2.0F);

    // Right Leg
    Entity right_leg_entity = registry_.create();
    math::Vec3f right_leg_offset(-1.0F, -2.0F, 0.0F);
    registry_.assign<Transform>(right_leg_entity,
                                body_entity,
                                body_transform,
                                right_leg_offset,
                                math::Vec3f::One(),
                                math::Quaternion());
    registry_.assign<Volume>(right_leg_entity,
                                     position + right_leg_offset,
                                     2.0F);

    // Add child entities
    auto& parent_transform = registry_.get<Transform>(body_entity);
    parent_transform.children_.push_back(head_entity);
    parent_transform.children_.push_back(left_arm_entity);
    parent_transform.children_.push_back(right_arm_entity);
    parent_transform.children_.push_back(left_leg_entity);
    parent_transform.children_.push_back(right_leg_entity);

    return body_entity;
}

bool TestRegistry::ParentContainsChildVolumes(Entity parent) const
{
    auto &&[parent_transform, parent_volume] = registry_.get<Transform, Volume>(parent);

    for (Entity child_entity : parent_transform.children_)
    {
        const auto& child_volume = registry_.get<Volume>(child_entity);
        if (!parent_volume.bounding_volume_.Contains(child_volume.bounding_volume_)
        || !ParentContainsChildVolumes(child_entity))
        {
            return false;
        }
    }
    return true;
}