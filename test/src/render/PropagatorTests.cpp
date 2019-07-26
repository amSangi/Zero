#include "TestPropagator.hpp"

using namespace zero;

/* ********** Propagate Transform Tests ********** */

TEST_F(TestPropagator, PropagateTransform_ParentVolume_SmallerThanChild) {
    auto body_entity = GenerateHumanoid(math::Vec3f::Zero(), 2.0f);
    EXPECT_FALSE(ParentContainsChildVolumes(body_entity));
    propagator_.PropagateTransform(registry_);
    EXPECT_FALSE(ParentContainsChildVolumes(body_entity));
}

TEST_F(TestPropagator, PropagateTransform_ParentVolume_ContainsChildren) {
    for (auto& body_entity : humanoids_) {
        EXPECT_TRUE(ParentContainsChildVolumes(body_entity));
    }
}

TEST_F(TestPropagator, PropagateTransform_ChangeParent_WorldPosition) {
    FAIL();
}

TEST_F(TestPropagator, PropagateTransform_ChangeParent_WorldScale) {
    FAIL();
}

TEST_F(TestPropagator, PropagateTransform_ChangeParent_WorldOrientation) {
    FAIL();
}

TEST_F(TestPropagator, PropagateTransform_ChangeChild_WorldPosition) {
    FAIL();
}

TEST_F(TestPropagator, PropagateTransform_ChangeChild_WorldScale) {
    FAIL();
}

TEST_F(TestPropagator, PropagateTransform_ChangeChild_WorldOrientation) {
    FAIL();
}

/* ********** Propagate Volume Tests ********** */

TEST_F(TestPropagator, PropagateVolume_ParentVolume_SmallerThanChild) {
    FAIL();
}

TEST_F(TestPropagator, PropagateVolume_ParentVolume_ContainsChildren) {
    FAIL();
}

TEST_F(TestPropagator, PropagateVolume_ChangeParent_WorldPosition) {
    FAIL();
}

TEST_F(TestPropagator, PropagateVolume_ChangeParent_WorldScale) {
    FAIL();
}

TEST_F(TestPropagator, PropagateVolume_ChangeParent_WorldOrientation) {
    FAIL();
}

TEST_F(TestPropagator, PropagateVolume_ChangeChild_WorldPosition) {
    FAIL();
}

TEST_F(TestPropagator, PropagateVolume_ChangeChild_WorldScale) {
    FAIL();
}

TEST_F(TestPropagator, PropagateVolume_ChangeChild_WorldOrientation) {
    FAIL();
}