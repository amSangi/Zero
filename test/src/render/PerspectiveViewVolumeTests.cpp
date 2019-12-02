#include "render/ViewVolumeBuilder.hpp"
#include "render/Components.hpp"
#include <gtest/gtest.h>
#include <memory>

using namespace zero;
using namespace zero::render;

class TestPerspectiveViewVolume : public ::testing::Test {
protected:

    TestPerspectiveViewVolume()
    : camera_(Camera::ProjectionType::PERSPECTIVE)
    , volume_(nullptr)
    {}

    void SetUp() override {
        std::shared_ptr<IViewVolume> view_volume = ViewVolumeBuilder::create(camera_);
        volume_ = std::static_pointer_cast<PerspectiveViewVolume>(view_volume);
    }

    Camera camera_;
    std::shared_ptr<PerspectiveViewVolume> volume_;
};


TEST_F(TestPerspectiveViewVolume, PlaneNormals) {
    EXPECT_EQ(volume_->GetLeftPlane().normal_, -1.0F * volume_->GetRightPlane().normal_);
    EXPECT_EQ(volume_->GetBottomPlane().normal_, -1.0F * volume_->GetTopPlane().normal_);
    EXPECT_EQ(volume_->GetNearPlane().normal_, -1.0F * volume_->GetFarPlane().normal_);
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Point_InFrustrum) {
    math::Vec3f near_bottom_left;
    math::Vec3f near_top_right;
    math::Vec3f far_bottom_left;
    math::Vec3f far_top_right;
    camera_.GetNearClipCoordinates(near_bottom_left, near_top_right);
    camera_.GetFarClipCoordinates(far_bottom_left, far_top_right);

    EXPECT_FALSE(volume_->IsCulled(near_bottom_left));
    EXPECT_FALSE(volume_->IsCulled(near_top_right));
    EXPECT_FALSE(volume_->IsCulled(far_bottom_left));
    EXPECT_FALSE(volume_->IsCulled(far_top_right));
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Point_OutOfFrustrum) {
    math::Vec3f near_bottom_left;
    math::Vec3f near_top_right;
    math::Vec3f far_bottom_left;
    math::Vec3f far_top_right;
    camera_.GetNearClipCoordinates(near_bottom_left, near_top_right);
    camera_.GetFarClipCoordinates(far_bottom_left, far_top_right);

    EXPECT_TRUE(volume_->IsCulled(camera_.position_));
    EXPECT_TRUE(volume_->IsCulled(math::Vec3f(camera_.position_.x_,
                                                       camera_.position_.y_,
                                                       camera_.far_clip_ - 1.0F)));
    EXPECT_TRUE(volume_->IsCulled(near_bottom_left - 1.0F));
    EXPECT_TRUE(volume_->IsCulled(near_top_right + 1.0F));
    EXPECT_TRUE(volume_->IsCulled(far_bottom_left - 1.0F));
    EXPECT_TRUE(volume_->IsCulled(far_top_right + 1.0F));
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Sphere_InFrustrum) {
    FAIL();
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Sphere_OutOfFrustrum) {
    FAIL();
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Sphere_IntersectPlane) {
    FAIL();
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Box_InFrustrum) {
    FAIL();
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Box_OutOfFrustrum) {
    FAIL();
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Box_IntersectPlane) {
    FAIL();
}