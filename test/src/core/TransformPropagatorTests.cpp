#include <algorithm>
#include "components/Transform.hpp"
#include "TestTransformPropagator.hpp"

using namespace zero;

/* ********** Remove Child Tests ********** */

TEST_F(TestTransformPropagator, RemoveChild_NullEntity)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    EXPECT_EQ(transform.children_.size(), 5);

    TransformPropagator::RemoveChild(registry_, body_entity, NullEntity);
    TransformPropagator::RemoveChild(registry_, NullEntity, body_entity);

    EXPECT_EQ(transform.children_.size(), 5);
}

TEST_F(TestTransformPropagator, RemoveChild_AlreadyRemoved)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    ASSERT_EQ(transform.children_.size(), 5);

    Entity child_entity_to_remove = transform.children_[0];

    TransformPropagator::RemoveChild(registry_, body_entity, child_entity_to_remove);
    EXPECT_EQ(transform.children_.size(), 4);

    TransformPropagator::RemoveChild(registry_, body_entity, child_entity_to_remove);
    EXPECT_EQ(transform.children_.size(), 4);
}

TEST_F(TestTransformPropagator, RemoveChild_ChildDeleted)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    ASSERT_EQ(transform.children_.size(), 5);

    Entity child_entity_to_delete = transform.children_[0];
    registry_.destroy(child_entity_to_delete);
    EXPECT_EQ(transform.children_.size(), 5);

    TransformPropagator::RemoveChild(registry_, body_entity, child_entity_to_delete);
    EXPECT_EQ(transform.children_.size(), 4);
}

TEST_F(TestTransformPropagator, RemoveChild_ValidChild)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    ASSERT_EQ(transform.children_.size(), 5);

    Entity child_entity_to_remove = transform.children_[0];
    auto& removed_child_transform = registry_.get<Transform>(child_entity_to_remove);
    EXPECT_TRUE(removed_child_transform.parent_ == body_entity);

    TransformPropagator::RemoveChild(registry_, body_entity, child_entity_to_remove);
    EXPECT_EQ(transform.children_.size(), 4);
    EXPECT_EQ(std::find(transform.children_.begin(),
                        transform.children_.end(),
                        child_entity_to_remove),
              transform.children_.end());

    EXPECT_TRUE(removed_child_transform.parent_ == NullEntity);
}


/* ********** Remove Children Tests ********** */

TEST_F(TestTransformPropagator, DetachChildren_NoChildren)
{
    Entity entity = registry_.create();
    auto& transform = registry_.assign<Transform>(entity);
    EXPECT_EQ(transform.children_.size(), 0);
    TransformPropagator::RemoveChildren(registry_, entity);
    EXPECT_EQ(transform.children_.size(), 0);
}

TEST_F(TestTransformPropagator, DetachChildren_DeletedChildren)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    registry_.destroy(transform.children_.begin(), transform.children_.end());
    EXPECT_EQ(transform.children_.size(), 5);
    TransformPropagator::RemoveChildren(registry_, body_entity);
    EXPECT_EQ(transform.children_.size(), 0);
}

TEST_F(TestTransformPropagator, DetachChildren_ValidChildren)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    EXPECT_EQ(transform.children_.size(), 5);
    std::vector<Entity> children = transform.children_;

    TransformPropagator::RemoveChildren(registry_, body_entity);
    EXPECT_EQ(transform.children_.size(), 0);

    for (Entity entity : children)
    {
        auto& child_transform = registry_.get<Transform>(entity);
        EXPECT_TRUE(child_transform.parent_ == NullEntity);
    }
}

/* ********** Remove Parent Tests ********** */

TEST_F(TestTransformPropagator, RemoveParent_NullParent)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    EXPECT_EQ(transform.children_.size(), 5);
    EXPECT_TRUE(transform.parent_ == NullEntity);
    TransformPropagator::RemoveParent(registry_, body_entity);
    EXPECT_EQ(transform.children_.size(), 5);
    EXPECT_TRUE(transform.parent_ == NullEntity);
}

TEST_F(TestTransformPropagator, RemoveParent_DeletedParent)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    std::vector<Entity> children = transform.children_;
    registry_.destroy(body_entity);

    for (Entity child_entity : children)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_TRUE(child_transform.parent_ == body_entity);
        TransformPropagator::RemoveParent(registry_, child_entity);
        EXPECT_TRUE(child_transform.parent_ == NullEntity);
    }
}

TEST_F(TestTransformPropagator, RemoveParent_ValidParent)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    std::vector<Entity> children = transform.children_;
    for (Entity child_entity : children)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_TRUE(child_transform.parent_ == body_entity);
        TransformPropagator::RemoveParent(registry_, child_entity);
        EXPECT_TRUE(child_transform.parent_ == NullEntity);
    }
    EXPECT_TRUE(transform.children_.empty());
}


/* ********** Propagate Mark For Destruction Tests ********** */

TEST_F(TestTransformPropagator, PropagateMarkForDestruction_NoneDestroyed)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    EXPECT_NE(transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    TransformPropagator::PropagateMarkForDestruction(registry_);

    EXPECT_NE(transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestTransformPropagator, PropagateMarkForDestruction_ParentMarkedForDestruction)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    transform.state_ = Transform::State::MARKED_FOR_DELETE;
    TransformPropagator::PropagateMarkForDestruction(registry_);

    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestTransformPropagator, PropagateMarkForDestruction_ParentDestroyed)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    transform.state_ = Transform::State::MARKED_FOR_DELETE;
    std::vector<Entity> children = transform.children_;
    registry_.destroy(body_entity);
    TransformPropagator::PropagateMarkForDestruction(registry_);

    for (Entity child_entity : children)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestTransformPropagator, PropagateMarkForDestruction_ChildrenMarkedForDestruction)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        child_transform.state_ = Transform::State::MARKED_FOR_DELETE;
    }

    EXPECT_NE(transform.state_, Transform::State::MARKED_FOR_DELETE);
    TransformPropagator::PropagateMarkForDestruction(registry_);

    EXPECT_NE(transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestTransformPropagator, PropagateMarkForDestruction_ChildrenDestroyed)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    transform.state_ = Transform::State::MARKED_FOR_DELETE;
    registry_.destroy(transform.children_[0]);
    TransformPropagator::PropagateMarkForDestruction(registry_);

    for (Entity child_entity : transform.children_)
    {
        // Skip destroyed child
        if (child_entity == transform.children_[0])
        {
            continue;
        }

        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestTransformPropagator, PropagateMarkForDestruction_KeepChildrenAlive)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);

    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }

    transform.keep_children_alive_ = true;
    transform.state_ = Transform::State::MARKED_FOR_DELETE;
    TransformPropagator::PropagateMarkForDestruction(registry_);

    for (Entity child_entity : transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestTransformPropagator, PropagateMarkForDestruction_DeepHierarchy)
{
    Entity root_body_entity = GenerateHumanoid(math::Vec3f::Zero());
    Entity level_one = GenerateHumanoid(math::Vec3f::Zero());
    Entity level_two = GenerateHumanoid(math::Vec3f::Zero());

    auto& root_body_transform = registry_.get<Transform>(root_body_entity);
    auto& level_one_transform = registry_.get<Transform>(level_one);
    auto& level_two_transform = registry_.get<Transform>(level_two);

    root_body_transform.children_.push_back(level_one);
    level_one_transform.parent_ = root_body_entity;

    level_one_transform.children_.push_back(level_two);
    level_two_transform.parent_ = level_one;

    root_body_transform.state_ = Transform::State::MARKED_FOR_DELETE;
    TransformPropagator::PropagateMarkForDestruction(registry_);

    EXPECT_EQ(root_body_transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (Entity child_entity : root_body_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
    for (Entity child_entity : level_one_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
    for (Entity child_entity : level_two_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}

TEST_F(TestTransformPropagator, PropagateMarkForDestruction_KeepDeepHierarchyAlive)
{
    Entity root_body_entity = GenerateHumanoid(math::Vec3f::Zero());
    Entity level_one = GenerateHumanoid(math::Vec3f::Zero());
    Entity level_two = GenerateHumanoid(math::Vec3f::Zero());

    auto& root_body_transform = registry_.get<Transform>(root_body_entity);
    auto& level_one_transform = registry_.get<Transform>(level_one);
    auto& level_two_transform = registry_.get<Transform>(level_two);

    root_body_transform.children_.push_back(level_one);
    level_one_transform.parent_ = root_body_entity;

    level_one_transform.children_.push_back(level_two);
    level_two_transform.parent_ = level_one;

    level_one_transform.keep_children_alive_ = true;
    root_body_transform.state_ = Transform::State::MARKED_FOR_DELETE;
    TransformPropagator::PropagateMarkForDestruction(registry_);

    EXPECT_EQ(root_body_transform.state_, Transform::State::MARKED_FOR_DELETE);
    for (Entity child_entity : root_body_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_EQ(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
    for (Entity child_entity : level_one_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
    for (Entity child_entity : level_two_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_NE(child_transform.state_, Transform::State::MARKED_FOR_DELETE);
    }
}


/* ********** Propagate Transform Tests ********** */

TEST_F(TestTransformPropagator, PropagateTransform_RootParentDeleted)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& transform = registry_.get<Transform>(body_entity);
    std::vector<Entity> children = transform.children_;

    registry_.destroy(body_entity);
    TransformPropagator::PropagateTransform(registry_);

    for (Entity child_entity : children)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        EXPECT_TRUE(child_transform.parent_ == body_entity);
    }
}

TEST_F(TestTransformPropagator, PropagateTransform_ChildDeleted)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero(), 2.0F);
    auto& transform = registry_.get<Transform>(body_entity);
    auto& child_transform = registry_.get<Transform>(transform.children_[1]);
    const math::Vec3f prev_child_position = child_transform.GetPosition();

    transform.Translate(math::Vec3f::One());
    registry_.destroy(transform.children_[0]);
    TransformPropagator::PropagateTransform(registry_);

    EXPECT_NE(child_transform.GetPosition(), prev_child_position);
}

TEST_F(TestTransformPropagator, PropagateTransform_ChangeParent_WorldPosition)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Entity, Transform> child_map;
    for (Entity child_entity : body_transform.children_)
    {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply translation to parent entity and propagate
    math::Vec3f translation(1.0F, 2.0F, 3.0F);
    body_transform.Translate(translation);
    EXPECT_TRUE(body_transform.IsModified());
    TransformPropagator::PropagateTransform(registry_);
    TransformPropagator::ClearCachedTransformations(registry_);
    EXPECT_FALSE(body_transform.IsModified());

    // Verify child entities have updated transforms
    for (Entity child_entity : body_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        EXPECT_EQ(child_transform.GetPosition(), prev_child_transform.GetPosition() + translation);
    }
}

TEST_F(TestTransformPropagator, PropagateTransform_ChangeParent_WorldScale)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Entity, Transform> child_map;
    for (Entity child_entity : body_transform.children_)
    {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply scale to parent entity and propagate
    math::Vec3f scale(1.0F, 2.0F, 3.0F);
    body_transform.Scale(scale);
    TransformPropagator::PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (Entity child_entity : body_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        EXPECT_EQ(child_transform.GetScale(), prev_child_transform.GetScale() * scale);
    }
}

TEST_F(TestTransformPropagator, PropagateTransform_ChangeParent_WorldOrientation)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Entity, Transform> child_map;
    for (Entity child_entity : body_transform.children_)
    {
        child_map[child_entity] = registry_.get<Transform>(child_entity);
    }

    // Apply rotation to parent entity and propagate
    math::Quaternion rotation = math::Quaternion::FromEuler(math::Radian(0.0F),
                                                            math::Radian(0.0F),
                                                            math::Degree(90.0F).ToRadian());
    body_transform.Rotate(rotation);
    TransformPropagator::PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (Entity child_entity : body_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        EXPECT_EQ(child_transform.GetOrientation(), prev_child_transform.GetOrientation() * rotation);
    }
}

TEST_F(TestTransformPropagator, PropagateTransform_ChangeParent_WorldTRS)
{
    Entity body_entity = GenerateHumanoid(math::Vec3f::Zero());
    auto& body_transform = registry_.get<Transform>(body_entity);

    // Copy previous child transforms
    std::unordered_map<Entity, Transform> child_map;
    for (Entity child_entity : body_transform.children_)
    {
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
    TransformPropagator::PropagateTransform(registry_);

    // Verify child entities have updated transforms
    for (Entity child_entity : body_transform.children_)
    {
        auto& child_transform = registry_.get<Transform>(child_entity);
        auto& prev_child_transform = child_map[child_entity];
        prev_child_transform.Scale(scale)
                .Rotate(rotation)
                .Translate(translation);
        auto transform_matrix = prev_child_transform.GetLocalToWorldMatrix();
        math::Vec3f expected_position = transform_matrix.GetTranslation();
        math::Vec3f expected_scale = transform_matrix.GetScale();
        math::Quaternion expected_orientation = transform_matrix.GetRotation();
        EXPECT_EQ(child_transform.GetPosition(), expected_position);
        EXPECT_EQ(child_transform.GetScale(), expected_scale);
        EXPECT_EQ(child_transform.GetOrientation(), expected_orientation);
    }
}