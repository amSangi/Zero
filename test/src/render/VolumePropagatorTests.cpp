#include "render/Components.hpp"
#include "TestVolumePropagator.hpp"

using namespace zero;

TEST_F(TestVolumePropagator, PropagateVolume_RootParentDeleted) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    std::vector<Component::Entity> children = transform.children_;

    registry_.destroy(body_entity);
    render::VolumePropagator::PropagateVolume(registry_);

    for (auto child_entity : children) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_TRUE(child_transform.parent_ == body_entity);
    }
}

TEST_F(TestVolumePropagator, PropagateVolume_ChildDeleted) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 2.0F);
    auto& transform = registry_.get<Transform>(body_entity);
    auto& volume = registry_.get<render::Volume>(body_entity);
    registry_.destroy(transform.children_[0]);
    render::VolumePropagator::PropagateVolume(registry_);

    EXPECT_GT(volume.bounding_volume_.radius_, 2.0F);
}

TEST_F(TestVolumePropagator, PropagateVolume_ParentVolume_SmallerThanChild) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 2.0F);
    EXPECT_FALSE(ParentContainsChildVolumes(body_entity));
    render::VolumePropagator::PropagateVolume(registry_);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestVolumePropagator, PropagateVolume_ParentVolume_ContainsChildren) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 6.0F);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
    render::VolumePropagator::PropagateVolume(registry_);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestVolumePropagator, PropagateVolume_ChangeParent_WorldPosition) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));

    // Apply translation to parent entity and propagate
    math::Vec3f translation(1.0F, 2.0F, 3.0F);
    body_transform.Translate(translation);
    TransformPropagator::PropagateTransform(registry_);
    render::VolumePropagator::PropagateVolume(registry_);

    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestVolumePropagator, PropagateVolume_ChangeParent_WorldScale) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));

    // Apply scale to parent entity and propagate
    math::Vec3f scale(1.0F, 2.0F, 3.0F);
    body_transform.Scale(scale);
    TransformPropagator::PropagateTransform(registry_);
    render::VolumePropagator::PropagateVolume(registry_);

    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestVolumePropagator, PropagateVolume_ChangeParent_WorldOrientation) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));

    // Apply rotation to parent entity and propagate
    math::Quaternion rotation = math::Quaternion::FromEuler(math::Radian(0.0F),
                                                            math::Radian(0.0F),
                                                            math::Degree(90.0F).ToRadian());
    body_transform.Rotate(rotation);
    TransformPropagator::PropagateTransform(registry_);
    render::VolumePropagator::PropagateVolume(registry_);

    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestVolumePropagator, PropagateVolume_ChangeParent_WorldTRS) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));

    // Apply TRS to parent entity and propagate
    math::Vec3f translation(1.0F, 2.0F, 3.0F);
    math::Vec3f scale(1.0F, 2.0F, 3.0F);
    math::Quaternion rotation = math::Quaternion::FromEuler(math::Radian(0.0F),
                                                            math::Radian(0.0F),
                                                            math::Degree(195.0F).ToRadian());
    body_transform.Scale(scale)
                  .Rotate(rotation)
                  .Translate(translation);
    TransformPropagator::PropagateTransform(registry_);
    render::VolumePropagator::PropagateVolume(registry_);

    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}
