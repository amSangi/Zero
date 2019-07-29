#include "gtest/gtest.h"
#include "core/Transform.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Vector3.hpp"

using namespace zero;

TEST(TestTransform, DefaultConstructor) {
    Transform transform;
    EXPECT_EQ(transform.position_, math::Vec3f::Zero());
    EXPECT_EQ(transform.local_position_, math::Vec3f::Zero());
    EXPECT_EQ(transform.orientation_, math::Quaternion());
    EXPECT_EQ(transform.local_orientation_, math::Quaternion());
    EXPECT_EQ(transform.scale_, math::Vec3f::One());
    EXPECT_EQ(transform.local_scale_, math::Vec3f::One());
    EXPECT_TRUE(transform.parent_ == Component::NullEntity);
    EXPECT_TRUE(transform.children_.empty());
}

TEST(TestTransform, RootTransformConstructor) {
    math::Vec3f position(1.0f, 2.0f, 3.0f);
    math::Vec3f scale(1.0f, 2.0f, 1.0f);
    math::Quaternion orientation = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                               math::Radian(0.0f),
                                                               math::Degree(90.0f).ToRadian());
    Transform transform(position, scale, orientation);
    EXPECT_EQ(transform.position_, position);
    EXPECT_EQ(transform.local_position_, math::Vec3f::Zero());
    EXPECT_EQ(transform.orientation_, orientation);
    EXPECT_EQ(transform.local_orientation_, math::Quaternion());
    EXPECT_EQ(transform.scale_, scale);
    EXPECT_EQ(transform.local_scale_, math::Vec3f::One());
    EXPECT_TRUE(transform.parent_ == Component::NullEntity);
    EXPECT_TRUE(transform.children_.empty());
}

TEST(TestTransform, ChildTransformConstructor) {
    math::Vec3f parent_position(1.0f, 2.0f, 3.0f);
    math::Vec3f parent_scale(1.0f, 2.0f, 1.0f);
    math::Quaternion parent_orientation = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                                      math::Radian(0.0f),
                                                                      math::Degree(90.0f).ToRadian());
    math::Vec3f local_child_position(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_scale(1.0f, 2.0f, 1.0f);
    math::Quaternion local_child_orientation = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                                           math::Radian(0.0f),
                                                                           math::Degree(90.0f).ToRadian());

    Transform parent_transform(parent_position, parent_scale, parent_orientation);

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              local_child_position,
                              local_child_scale,
                              local_child_orientation);

    EXPECT_EQ(child_transform.position_, parent_position + local_child_position);
    EXPECT_EQ(child_transform.local_position_, local_child_position);
    EXPECT_EQ(child_transform.orientation_, (parent_orientation * local_child_orientation).Unit());
    EXPECT_EQ(child_transform.local_orientation_, local_child_orientation);
    EXPECT_EQ(child_transform.scale_, parent_scale * local_child_scale);
    EXPECT_EQ(child_transform.local_scale_, local_child_scale);
}

TEST(TestTransform, WorldToLocalMatrix) {
    math::Vec3f parent_position(1.0f, 2.0f, 3.0f);
    math::Vec3f parent_scale(1.0f, 2.0f, 1.0f);
    math::Quaternion parent_orientation;

    math::Vec3f local_child_position(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_scale(1.0f, 2.0f, 1.0f);
    math::Quaternion local_child_orientation;

    Transform parent_transform(parent_position, parent_scale, parent_orientation);

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              local_child_position,
                              local_child_scale,
                              local_child_orientation);

    EXPECT_EQ(parent_transform.GetWorldToLocalMatrix(), math::Matrix4x4(1.0f, 0.0f, 0.0f, 1.0f,
                                                                        0.0f, 2.0f, 0.0f, 2.0f,
                                                                        0.0f, 0.0f, 1.0f, 3.0f,
                                                                        0.0f, 0.0f, 0.0f, 1.0f).Inverse());
    EXPECT_EQ(child_transform.GetWorldToLocalMatrix(), math::Matrix4x4(1.0f, 0.0f, 0.0f, 2.0f,
                                                                       0.0f, 4.0f, 0.0f, 4.0f,
                                                                       0.0f, 0.0f, 1.0f, 6.0f,
                                                                       0.0f, 0.0f, 0.0f, 1.0f).Inverse());
}

TEST(TestTransform, LocalToWorldMatrix) {
    math::Vec3f parent_position(1.0f, 2.0f, 3.0f);
    math::Vec3f parent_scale(1.0f, 2.0f, 1.0f);
    math::Quaternion parent_orientation;

    math::Vec3f local_child_position(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_scale(1.0f, 2.0f, 1.0f);
    math::Quaternion local_child_orientation;

    Transform parent_transform(parent_position, parent_scale, parent_orientation);

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              local_child_position,
                              local_child_scale,
                              local_child_orientation);

    EXPECT_EQ(parent_transform.GetLocalToWorldMatrix(), math::Matrix4x4(1.0f, 0.0f, 0.0f, 1.0f,
                                                                        0.0f, 2.0f, 0.0f, 2.0f,
                                                                        0.0f, 0.0f, 1.0f, 3.0f,
                                                                        0.0f, 0.0f, 0.0f, 1.0f));
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), math::Matrix4x4(1.0f, 0.0f, 0.0f, 2.0f,
                                                                       0.0f, 4.0f, 0.0f, 4.0f,
                                                                       0.0f, 0.0f, 1.0f, 6.0f,
                                                                       0.0f, 0.0f, 0.0f, 1.0f));
}

TEST(TestTransform, LocalToParentMatrix) {
    math::Vec3f parent_position(1.0f, 2.0f, 3.0f);
    math::Vec3f parent_scale(1.0f, 2.0f, 1.0f);
    math::Quaternion parent_orientation;

    math::Vec3f local_child_position(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_scale(1.0f, 2.0f, 1.0f);
    math::Quaternion local_child_orientation;

    Transform parent_transform(parent_position, parent_scale, parent_orientation);

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              local_child_position,
                              local_child_scale,
                              local_child_orientation);

    EXPECT_EQ(parent_transform.GetLocalToParentMatrix(), math::Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
                                                                         0.0f, 1.0f, 0.0f, 0.0f,
                                                                         0.0f, 0.0f, 1.0f, 0.0f,
                                                                         0.0f, 0.0f, 0.0f, 1.0f));
    EXPECT_EQ(child_transform.GetLocalToParentMatrix(), math::Matrix4x4(1.0f, 0.0f, 0.0f, 1.0f,
                                                                        0.0f, 2.0f, 0.0f, 2.0f,
                                                                        0.0f, 0.0f, 1.0f, 3.0f,
                                                                        0.0f, 0.0f, 0.0f, 1.0f));
}

TEST(TestTransform, Translate) {
    math::Vec3f parent_position(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_position(1.0f, 2.0f, 3.0f);
    math::Vec3f translation(10.0f, -15.0f, 5.0f);

    Transform parent_transform(parent_position, math::Vec3f::One(), math::Quaternion());

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              local_child_position,
                              math::Vec3f::One(),
                              math::Quaternion());

    child_transform.Translate(translation);

    math::Vec3f expected_child_world_pos = parent_position + local_child_position + translation;

    EXPECT_EQ(child_transform.position_, expected_child_world_pos);
    EXPECT_EQ(child_transform.local_position_, local_child_position);
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), math::Matrix4x4(1.0f, 0.0f, 0.0f, expected_child_world_pos.x_,
                                                                       0.0f, 1.0f, 0.0f, expected_child_world_pos.y_,
                                                                       0.0f, 0.0f, 1.0f, expected_child_world_pos.z_,
                                                                       0.0f, 0.0f, 0.0f, 1.0f));
}

TEST(TestTransform, LocalTranslate) {
    math::Vec3f parent_position(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_position(1.0f, 2.0f, 3.0f);
    math::Vec3f translation(10.0f, -15.0f, 5.0f);

    Transform parent_transform(parent_position, math::Vec3f::One(), math::Quaternion());

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              local_child_position,
                              math::Vec3f::One(),
                              math::Quaternion());

    child_transform.LocalTranslate(parent_transform, translation);

    math::Vec3f expected_child_world_pos = parent_position + local_child_position + translation;
    math::Vec3f expected_child_local_pos = local_child_position + translation;

    EXPECT_EQ(child_transform.position_, expected_child_world_pos);
    EXPECT_EQ(child_transform.local_position_, expected_child_local_pos);
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), math::Matrix4x4(1.0f, 0.0f, 0.0f, expected_child_world_pos.x_,
                                                                       0.0f, 1.0f, 0.0f, expected_child_world_pos.y_,
                                                                       0.0f, 0.0f, 1.0f, expected_child_world_pos.z_,
                                                                       0.0f, 0.0f, 0.0f, 1.0f));
}

TEST(TestTransform, Rotate) {
    math::Quaternion parent_orientation = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                                      math::Radian(0.0f),
                                                                      math::Radian::FromDegree(60.0f));
    math::Quaternion local_child_orientation = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                                           math::Radian(0.0f),
                                                                           math::Radian::FromDegree(60.0f));
    math::Quaternion rotation  = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                             math::Radian(0.0f),
                                                             math::Radian::FromDegree(-60.0f));

    Transform parent_transform(math::Vec3f::Zero(), math::Vec3f::One(), parent_orientation);

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              math::Vec3f::Zero(),
                              math::Vec3f::One(),
                              local_child_orientation);

    child_transform.Rotate(rotation);

    math::Quaternion expected_child_world_orientation = (parent_orientation * local_child_orientation * rotation);

    EXPECT_EQ(child_transform.orientation_, expected_child_world_orientation);
    EXPECT_EQ(child_transform.local_orientation_, local_child_orientation);
    auto rotation_matrix = expected_child_world_orientation.GetRotationMatrix();
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), math::Matrix4x4(rotation_matrix));
}

TEST(TestTransform, LocalRotate) {
    math::Quaternion parent_orientation = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                                      math::Radian(0.0f),
                                                                      math::Radian::FromDegree(60.0f));
    math::Quaternion local_child_orientation = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                                           math::Radian(0.0f),
                                                                           math::Radian::FromDegree(60.0f));
    math::Quaternion rotation  = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                             math::Radian(0.0f),
                                                             math::Radian::FromDegree(-60.0f));

    Transform parent_transform(math::Vec3f::Zero(), math::Vec3f::One(), parent_orientation);

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              math::Vec3f::Zero(),
                              math::Vec3f::One(),
                              local_child_orientation);

    child_transform.LocalRotate(parent_transform, rotation);

    math::Quaternion expected_child_world_orientation = (parent_orientation * local_child_orientation * rotation);
    math::Quaternion expected_child_local_orientation = (local_child_orientation * rotation);

    EXPECT_EQ(child_transform.orientation_, expected_child_world_orientation);
    EXPECT_EQ(child_transform.local_orientation_, expected_child_local_orientation);
    auto rotation_matrix = expected_child_world_orientation.GetRotationMatrix();
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), math::Matrix4x4(rotation_matrix));
}

TEST(TestTransform, Scale) {
    math::Vec3f parent_scale(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_scale(1.0f, 2.0f, 3.0f);
    math::Vec3f scale(10.0f, 15.0f, 5.0f);

    Transform parent_transform(math::Vec3f::Zero(), parent_scale, math::Quaternion());

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              math::Vec3f::Zero(),
                              local_child_scale,
                              math::Quaternion());

    child_transform.Scale(scale);

    math::Vec3f expected_child_world_scale = parent_scale * local_child_scale * scale;

    EXPECT_EQ(child_transform.scale_, expected_child_world_scale);
    EXPECT_EQ(child_transform.local_scale_, local_child_scale);
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), math::Matrix4x4(expected_child_world_scale.x_, 0.0f, 0.0f, 0.0f,
                                                                       0.0f, expected_child_world_scale.y_, 0.0f, 0.0f,
                                                                       0.0f, 0.0f, expected_child_world_scale.z_, 0.0f,
                                                                       0.0f, 0.0f, 0.0f, 1.0f));
}

TEST(TestTransform, LocalScale) {
    math::Vec3f parent_scale(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_scale(1.0f, 2.0f, 3.0f);
    math::Vec3f scale(10.0f, 15.0f, 5.0f);

    Transform parent_transform(math::Vec3f::Zero(), parent_scale, math::Quaternion());

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              math::Vec3f::Zero(),
                              local_child_scale,
                              math::Quaternion());

    child_transform.LocalScale(parent_transform, scale);

    math::Vec3f expected_child_world_scale = parent_scale * local_child_scale * scale;
    math::Vec3f expected_child_local_scale = local_child_scale * scale;

    EXPECT_EQ(child_transform.scale_, expected_child_world_scale);
    EXPECT_EQ(child_transform.local_scale_, expected_child_local_scale);
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), math::Matrix4x4(expected_child_world_scale.x_, 0.0f, 0.0f, 0.0f,
                                                                       0.0f, expected_child_world_scale.y_, 0.0f, 0.0f,
                                                                       0.0f, 0.0f, expected_child_world_scale.z_, 0.0f,
                                                                       0.0f, 0.0f, 0.0f, 1.0f));
}

TEST(TestTransform, World_TRS_Transformation) {
    math::Vec3f parent_position(1.0f, 2.0f, 3.0f);
    math::Vec3f parent_scale(1.0f, 2.0f, 3.0f);
    math::Quaternion parent_orientation  = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                                       math::Radian(0.0f),
                                                                       math::Radian::FromDegree(60.0f));

    // Transformations
    math::Vec3f translation(10.0f, -15.0f, 5.0f);
    math::Vec3f scale(10.0f, 15.0f, 5.0f);
    math::Quaternion rotation  = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                             math::Radian(0.0f),
                                                             math::Radian::FromDegree(-60.0f));

    Transform parent_transform(parent_position, parent_scale, parent_orientation);

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              math::Vec3f::Zero(),
                              math::Vec3f::One(),
                              math::Quaternion());

    child_transform.Scale(scale)
                   .Rotate(rotation)
                   .Translate(translation);

    auto expected_position = translation + parent_position;
    auto expected_scale = scale * parent_scale;
    auto expected_orientation = math::Quaternion::Identity();

    auto expected_local_to_world_matrix = math::Matrix4x4::Identity();
    expected_local_to_world_matrix.Scale(expected_scale)
                                  .Rotate(expected_orientation)
                                  .Translate(expected_position);

    EXPECT_EQ(child_transform.position_, expected_position);
    EXPECT_EQ(child_transform.scale_, expected_scale);
    EXPECT_EQ(child_transform.orientation_, expected_orientation);
    EXPECT_EQ(child_transform.local_position_, math::Vec3f::Zero());
    EXPECT_EQ(child_transform.local_scale_, math::Vec3f::One());
    EXPECT_EQ(child_transform.local_orientation_, math::Quaternion());

    EXPECT_EQ(child_transform.GetLocalToParentMatrix(), math::Matrix4x4::Identity());
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), expected_local_to_world_matrix);
    EXPECT_EQ(child_transform.GetWorldToLocalMatrix(), expected_local_to_world_matrix.Inverse());
}

TEST(TestTransform, Local_TRS_Transformation) {
    math::Vec3f parent_position = math::Vec3f::Zero();
    math::Vec3f parent_scale = math::Vec3f::One();
    math::Quaternion parent_orientation;

    math::Vec3f local_child_position(1.0f, 2.0f, 3.0f);
    math::Vec3f local_child_scale(1.0f, 2.0f, 3.0f);
    math::Quaternion local_child_orientation;

    // Transformations
    math::Vec3f translation(10.0f, -15.0f, 5.0f);
    math::Vec3f scale(1.0f, 2.0f, 3.0f);
    math::Quaternion rotation  = math::Quaternion::FromEuler(math::Radian(0.0f),
                                                             math::Radian(0.0f),
                                                             math::Radian::FromDegree(-60.0f));

    Transform parent_transform(parent_position, parent_scale, parent_orientation);

    auto dummy_entity = Component::NullEntity;
    Transform child_transform(dummy_entity,
                              parent_transform,
                              local_child_position,
                              local_child_scale,
                              local_child_orientation);

    child_transform.LocalScale(parent_transform, scale)
                   .LocalRotate(parent_transform, rotation)
                   .LocalTranslate(parent_transform, translation);

    auto expected_local_position = local_child_position + translation;
    auto expected_local_scale = local_child_scale * scale;
    auto expected_local_orientation = local_child_orientation * rotation;

    auto expected_position = expected_local_position + parent_position;
    auto expected_scale = expected_local_scale * parent_scale;
    auto expected_orientation = expected_local_orientation * parent_orientation;

    EXPECT_EQ(child_transform.position_, expected_position);
    EXPECT_EQ(child_transform.scale_, expected_scale);
    EXPECT_EQ(child_transform.orientation_, expected_orientation);

    EXPECT_EQ(child_transform.local_position_, expected_local_position);
    EXPECT_EQ(child_transform.local_scale_, expected_scale);
    EXPECT_EQ(child_transform.local_orientation_, expected_local_orientation);

    auto expected_local_to_parent_matrix = math::Matrix4x4::Identity()
            .Scale(expected_local_scale)
            .Rotate(expected_local_orientation)
            .Translate(expected_local_position);

    auto expected_local_to_world_matrix = math::Matrix4x4::Identity()
            .Scale(expected_scale)
            .Rotate(expected_orientation)
            .Translate(expected_position);

    EXPECT_EQ(child_transform.GetLocalToParentMatrix(), expected_local_to_parent_matrix);
    EXPECT_EQ(child_transform.GetLocalToWorldMatrix(), expected_local_to_world_matrix);
    EXPECT_EQ(child_transform.GetWorldToLocalMatrix(), expected_local_to_world_matrix.Inverse());
}