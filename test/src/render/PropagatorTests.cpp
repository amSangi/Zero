#include <algorithm>
#include <tuple>
#include "core/Transform.hpp"
#include "render/Components.hpp"
#include "TestPropagator.hpp"

using namespace zero;

/* ********** Remove Child Tests ********** */

TEST_F(TestPropagator, RemoveChild_NullEntity) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    EXPECT_EQ(transform.children_.size(), 5);

    propagator_.RemoveChild(registry_, body_entity, Component::NullEntity);
    propagator_.RemoveChild(registry_, Component::NullEntity, body_entity);

    EXPECT_EQ(transform.children_.size(), 5);
}

TEST_F(TestPropagator, RemoveChild_AlreadyRemoved) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    ASSERT_EQ(transform.children_.size(), 5);

    auto child_entity_to_remove = transform.children_[0];

    propagator_.RemoveChild(registry_, body_entity, child_entity_to_remove);
    EXPECT_EQ(transform.children_.size(), 4);

    propagator_.RemoveChild(registry_, body_entity, child_entity_to_remove);
    EXPECT_EQ(transform.children_.size(), 4);
}

TEST_F(TestPropagator, RemoveChild_ChildDeleted) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    ASSERT_EQ(transform.children_.size(), 5);

    auto child_entity_to_delete = transform.children_[0];
    registry_.destroy(child_entity_to_delete);
    EXPECT_EQ(transform.children_.size(), 5);

    propagator_.RemoveChild(registry_, body_entity, child_entity_to_delete);
    EXPECT_EQ(transform.children_.size(), 4);
}

TEST_F(TestPropagator, RemoveChild_ValidChild) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    ASSERT_EQ(transform.children_.size(), 5);

    auto child_entity_to_remove = transform.children_[0];
    auto& removed_child_transform = registry_.get<Transform>(child_entity_to_remove);
    EXPECT_TRUE(removed_child_transform.parent_ == body_entity);

    propagator_.RemoveChild(registry_, body_entity, child_entity_to_remove);
    EXPECT_EQ(transform.children_.size(), 4);
    EXPECT_EQ(std::find(transform.children_.begin(),
                        transform.children_.end(),
                        child_entity_to_remove),
              transform.children_.end());

    EXPECT_TRUE(removed_child_transform.parent_ == Component::NullEntity);
}


/* ********** Detach Children Tests ********** */

TEST_F(TestPropagator, DetachChildren_NoChildren) {
    auto entity = registry_.create();
    auto& transform = registry_.assign<Transform>(entity);
    EXPECT_EQ(transform.children_.size(), 0);
    propagator_.DetachChildrenTransform(registry_, entity);
    EXPECT_EQ(transform.children_.size(), 0);
}

TEST_F(TestPropagator, DetachChildren_DeletedChildren) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    registry_.destroy(transform.children_.begin(), transform.children_.end());
    EXPECT_EQ(transform.children_.size(), 5);
    propagator_.DetachChildrenTransform(registry_, body_entity);
    EXPECT_EQ(transform.children_.size(), 0);
}

TEST_F(TestPropagator, DetachChildren_ValidChildren) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    EXPECT_EQ(transform.children_.size(), 5);
    std::vector<Component::Entity> children = transform.children_;

    propagator_.DetachChildrenTransform(registry_, body_entity);
    EXPECT_EQ(transform.children_.size(), 0);

    for (auto entity : children) {
        auto& child_transform = registry_.get<Transform>(entity);
        EXPECT_TRUE(child_transform.parent_ == Component::NullEntity);
    }
}

/* ********** Detach Parent Tests ********** */

TEST_F(TestPropagator, DetachFromParent_NullParent) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    EXPECT_EQ(transform.children_.size(), 5);
    EXPECT_TRUE(transform.parent_ == Component::NullEntity);
    propagator_.DetachFromParentTransform(registry_, body_entity);
    EXPECT_EQ(transform.children_.size(), 5);
    EXPECT_TRUE(transform.parent_ == Component::NullEntity);
}

TEST_F(TestPropagator, DetachFromParent_DeletedParent) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    std::vector<Component::Entity> children = transform.children_;
    registry_.destroy(body_entity);

    for (auto child_entity : children) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_TRUE(child_transform.parent_ == body_entity);
        propagator_.DetachFromParentTransform(registry_, child_entity);
        EXPECT_TRUE(child_transform.parent_ == Component::NullEntity);
    }
}

TEST_F(TestPropagator, DetachFromParent_ValidParent) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    std::vector<Component::Entity> children = transform.children_;
    for (auto child_entity : children) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_TRUE(child_transform.parent_ == body_entity);
        propagator_.DetachFromParentTransform(registry_, child_entity);
        EXPECT_TRUE(child_transform.parent_ == Component::NullEntity);
    }
    EXPECT_TRUE(transform.children_.empty());
}


/* ********** Propagate Mark For Destruction Tests ********** */

TEST_F(TestPropagator, PropagateMarkForDestruction_NoneDestroyed) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    EXPECT_NE(transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    propagator_.PropagateMarkForDestruction(registry_);

    EXPECT_NE(transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestPropagator, PropagateMarkForDestruction_ParentMarkedForDestruction) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    transform.state_ = Transform::State::MARKED_FOR_DELETE;
    propagator_.PropagateMarkForDestruction(registry_);

    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestPropagator, PropagateMarkForDestruction_ParentDestroyed) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    transform.state_ = Transform::State::MARKED_FOR_DELETE;
    std::vector<Component::Entity> children = transform.children_;
    registry_.destroy(body_entity);
    propagator_.PropagateMarkForDestruction(registry_);

    for (auto child_entity : children) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestPropagator, PropagateMarkForDestruction_ChildrenMarkedForDestruction) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        child_transform.state_ = Transform::State::MARKED_FOR_DELETE;
    }

    EXPECT_NE(transform.state_, Transform::State::MARKED_FOR_DELETE);
    propagator_.PropagateMarkForDestruction(registry_);

    EXPECT_NE(transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestPropagator, PropagateMarkForDestruction_ChildrenDestroyed) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    transform.state_ = Transform::State::MARKED_FOR_DELETE;
    registry_.destroy(transform.children_[0]);
    propagator_.PropagateMarkForDestruction(registry_);

    for (auto child_entity : transform.children_) {
        // Skip destroyed child
        if (child_entity == transform.children_[0]) continue;

        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestPropagator, PropagateMarkForDestruction_KeepChildrenAlive) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    transform.keep_children_alive_ = true;
    transform.state_ = Transform::State::MARKED_FOR_DELETE;
    propagator_.PropagateMarkForDestruction(registry_);

    for (auto child_entity : transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestPropagator, PropagateMarkForDestruction_DeepHierarchy) {
    auto root_body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto level_one = GenerateHumanoid(math::Vec3f::Zero());
    auto level_two = GenerateHumanoid(math::Vec3f::Zero());

    auto& root_body_transform = registry_.get<Transform>(root_body_entity);
    auto& level_one_transform = registry_.get<Transform>(level_one);
    auto& level_two_transform = registry_.get<Transform>(level_two);

    root_body_transform.children_.push_back(level_one);
    level_one_transform.parent_ = root_body_entity;

    level_one_transform.children_.push_back(level_two);
    level_two_transform.parent_ = level_one;

    root_body_transform.state_ = Transform::State::MARKED_FOR_DELETE;
    propagator_.PropagateMarkForDestruction(registry_);

    EXPECT_EQ(root_body_transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (auto child_entity : root_body_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
    for (auto child_entity : level_one_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
    for (auto child_entity : level_two_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestPropagator, PropagateMarkForDestruction_KeepDeepHierarchyAlive) {
    auto root_body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto level_one = GenerateHumanoid(math::Vec3f::Zero());
    auto level_two = GenerateHumanoid(math::Vec3f::Zero());

    auto& root_body_transform = registry_.get<Transform>(root_body_entity);
    auto& level_one_transform = registry_.get<Transform>(level_one);
    auto& level_two_transform = registry_.get<Transform>(level_two);

    root_body_transform.children_.push_back(level_one);
    level_one_transform.parent_ = root_body_entity;

    level_one_transform.children_.push_back(level_two);
    level_two_transform.parent_ = level_one;

    level_one_transform.keep_children_alive_ = true;
    root_body_transform.state_ = Transform::State::MARKED_FOR_DELETE;
    propagator_.PropagateMarkForDestruction(registry_);

    EXPECT_EQ(root_body_transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (auto child_entity : root_body_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
    for (auto child_entity : level_one_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
    for (auto child_entity : level_two_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}


/* ********** Propagate Transform Tests ********** */

TEST_F(TestPropagator, PropagateTransform_RootParentDeleted) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    std::vector<Component::Entity> children = transform.children_;

    registry_.destroy(body_entity);
    propagator_.PropagateTransform(registry_);

    for (auto child_entity : children) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_TRUE(child_transform.parent_ == body_entity);
    }
}

TEST_F(TestPropagator, PropagateTransform_ChildDeleted) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 2.0F);
    auto& transform = registry_.get<Transform>(body_entity);
    auto& child_transform = registry_.get<Transform>(transform.children_[1]);
    const math::Vec3f prev_child_position = child_transform.position_;

    transform.Translate(math::Vec3f::One());
    registry_.destroy(transform.children_[0]);
    propagator_.PropagateTransform(registry_);

    EXPECT_NE(child_transform.position_, prev_child_position);
}

TEST_F(TestPropagator, PropagateTransform_ParentVolume_SmallerThanChild) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 2.0F);
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Component::Entity, Transform> child_map;
    for (const auto child_entity : body_transform.children_) {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply no transformation and propagate transforms
    propagator_.PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (const auto child_entity : body_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        EXPECT_EQ(child_transform, prev_child_transform);
    }
}

TEST_F(TestPropagator, PropagateTransform_ParentVolume_ContainsChildren) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Component::Entity, Transform> child_map;
    for (const auto child_entity : body_transform.children_) {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply no transformation and propagate transforms
    propagator_.PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (const auto child_entity : body_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        EXPECT_EQ(child_transform, prev_child_transform);
    }
}

TEST_F(TestPropagator, PropagateTransform_ChangeParent_WorldPosition) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Component::Entity, Transform> child_map;
    for (const auto child_entity : body_transform.children_) {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply translation to parent entity and propagate
    math::Vec3f translation(1.0F, 2.0F, 3.0F);
    body_transform.Translate(translation);
    propagator_.PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (const auto child_entity : body_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        EXPECT_EQ(child_transform.position_, prev_child_transform.position_ + translation);
    }
}

TEST_F(TestPropagator, PropagateTransform_ChangeParent_WorldScale) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Component::Entity, Transform> child_map;
    for (const auto child_entity : body_transform.children_) {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply scale to parent entity and propagate
    math::Vec3f scale(1.0F, 2.0F, 3.0F);
    body_transform.Scale(scale);
    propagator_.PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (const auto child_entity : body_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        EXPECT_EQ(child_transform.scale_, prev_child_transform.scale_ * scale);
    }
}

TEST_F(TestPropagator, PropagateTransform_ChangeParent_WorldOrientation) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Component::Entity, Transform> child_map;
    for (const auto child_entity : body_transform.children_) {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply rotation to parent entity and propagate
    math::Quaternion rotation = math::Quaternion::FromEuler(math::Radian(0.0F),
                                                            math::Radian(0.0F),
                                                            math::Degree(90.0F).ToRadian());
    body_transform.Rotate(rotation);
    propagator_.PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (const auto child_entity : body_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        EXPECT_EQ(child_transform.orientation_, prev_child_transform.orientation_ * rotation);
    }
}

TEST_F(TestPropagator, PropagateTransform_ChangeParent_WorldTRS) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Component::Entity, Transform> child_map;
    for (const auto child_entity : body_transform.children_) {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply TRS to parent entity and propagate
    math::Vec3f translation(1.0F, 2.0F, 3.0F);
    math::Vec3f scale(1.0F, 2.0F, 3.0F);
    math::Quaternion rotation = math::Quaternion::FromEuler(math::Radian(0.0F),
                                                            math::Radian(0.0F),
                                                            math::Degree(195.0F).ToRadian());
    body_transform.Scale(scale)
                  .Rotate(rotation)
                  .Translate(translation);
    propagator_.PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (const auto child_entity : body_transform.children_) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        prev_child_transform.Scale(scale)
                            .Rotate(rotation)
                            .Translate(translation);
        auto transform_matrix = prev_child_transform.GetLocalToWorldMatrix();
        auto expected_position = transform_matrix.GetTranslation();
        auto expected_scale = transform_matrix.GetScale();
        auto expected_orientation = transform_matrix.GetRotation();
        EXPECT_EQ(child_transform.position_, expected_position);
        EXPECT_EQ(child_transform.scale_, expected_scale);
        EXPECT_EQ(child_transform.orientation_, expected_orientation);
    }
}


/* ********** Propagate Volume Tests ********** */

TEST_F(TestPropagator, PropagateVolume_RootParentDeleted) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    std::vector<Component::Entity> children = transform.children_;

    registry_.destroy(body_entity);
    propagator_.PropagateVolume(registry_);

    for (auto child_entity : children) {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_TRUE(child_transform.parent_ == body_entity);
    }
}

TEST_F(TestPropagator, PropagateVolume_ChildDeleted) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 2.0F);
    auto& transform = registry_.get<Transform>(body_entity);
    auto& volume = registry_.get<render::Volume>(body_entity);
    registry_.destroy(transform.children_[0]);
    propagator_.PropagateVolume(registry_);

    EXPECT_GT(volume.bounding_volume_.radius_, 2.0F);
}

TEST_F(TestPropagator, PropagateVolume_ParentVolume_SmallerThanChild) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 2.0F);
    EXPECT_FALSE(ParentContainsChildVolumes(body_entity));
    propagator_.PropagateVolume(registry_);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestPropagator, PropagateVolume_ParentVolume_ContainsChildren) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 6.0F);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
    propagator_.PropagateVolume(registry_);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestPropagator, PropagateVolume_ChangeParent_WorldPosition) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));

    // Apply translation to parent entity and propagate
    math::Vec3f translation(1.0F, 2.0F, 3.0F);
    body_transform.Translate(translation);
    propagator_.PropagateTransform(registry_);
    propagator_.PropagateVolume(registry_);

    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestPropagator, PropagateVolume_ChangeParent_WorldScale) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));

    // Apply scale to parent entity and propagate
    math::Vec3f scale(1.0F, 2.0F, 3.0F);
    body_transform.Scale(scale);
    propagator_.PropagateTransform(registry_);
    propagator_.PropagateVolume(registry_);

    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestPropagator, PropagateVolume_ChangeParent_WorldOrientation) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);
    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));

    // Apply rotation to parent entity and propagate
    math::Quaternion rotation = math::Quaternion::FromEuler(math::Radian(0.0F),
                                                            math::Radian(0.0F),
                                                            math::Degree(90.0F).ToRadian());
    body_transform.Rotate(rotation);
    propagator_.PropagateTransform(registry_);
    propagator_.PropagateVolume(registry_);

    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestPropagator, PropagateVolume_ChangeParent_WorldTRS) {
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
    propagator_.PropagateTransform(registry_);
    propagator_.PropagateVolume(registry_);

    EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
}