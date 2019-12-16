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
    , near_bottom_left_()
    , near_top_right_()
    , far_bottom_left_()
    , far_top_right_()
    {}

    void SetUp() override {
        std::shared_ptr<IViewVolume> view_volume = ViewVolumeBuilder::create(camera_);
        volume_ = std::static_pointer_cast<PerspectiveViewVolume>(view_volume);
        camera_.GetNearClipCoordinates(near_bottom_left_, near_top_right_);
        camera_.GetFarClipCoordinates(far_bottom_left_, far_top_right_);
    }

    Camera camera_;
    std::shared_ptr<PerspectiveViewVolume> volume_;
    math::Vec3f near_bottom_left_;
    math::Vec3f near_top_right_;
    math::Vec3f far_bottom_left_;
    math::Vec3f far_top_right_;
};


TEST_F(TestPerspectiveViewVolume, PlaneNormals) {
    auto left_normal = volume_->GetLeftPlane().normal_;
    auto right_normal = volume_->GetRightPlane().normal_;
    auto bottom_normal = volume_->GetBottomPlane().normal_;
    auto top_normal = volume_->GetTopPlane().normal_;
    auto near_normal = volume_->GetNearPlane().normal_;
    auto far_normal = volume_->GetFarPlane().normal_;

    // Opposite x component
    EXPECT_EQ(left_normal, math::Vec3f(-1.0F, 1.0F, 1.0F) * right_normal);
    // Point right and back
    EXPECT_GT(left_normal.x_, 0.0F);
    EXPECT_EQ(left_normal.y_, 0.0F);
    EXPECT_LT(left_normal.z_, 0.0F);

    // Opposite y component
    EXPECT_EQ(bottom_normal, math::Vec3f(1.0F, -1.0F, 1.0F) * top_normal);
    // Point up and back
    EXPECT_EQ(bottom_normal.x_, 0.0F);
    EXPECT_GT(bottom_normal.y_, 0.0F);
    EXPECT_LT(bottom_normal.z_, 0.0F);

    // Opposite z component
    EXPECT_EQ(near_normal, math::Vec3f(1.0F, 1.0F, -1.0F) * far_normal);
    // Point to the back
    EXPECT_EQ(near_normal.x_, 0.0F);
    EXPECT_EQ(near_normal.y_, 0.0F);
    EXPECT_LT(near_normal.z_, 0.0F);
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Point_InFrustrum) {
    EXPECT_FALSE(volume_->IsCulled(near_bottom_left_));
    EXPECT_FALSE(volume_->IsCulled(near_top_right_));
    EXPECT_FALSE(volume_->IsCulled(far_bottom_left_));
    EXPECT_FALSE(volume_->IsCulled(far_top_right_));

    auto near_z = near_bottom_left_.z_;
    auto far_z = far_bottom_left_.z_;
    EXPECT_FALSE(volume_->IsCulled(math::Vec3f(camera_.position_.x_,
                                                        camera_.position_.y_,
                                                        (far_z - near_z) * 0.5F)));
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Point_OutOfFrustrum) {
    EXPECT_TRUE(volume_->IsCulled(camera_.position_));
    EXPECT_TRUE(volume_->IsCulled(math::Vec3f(camera_.position_.x_,
                                                       camera_.position_.y_,
                                                       camera_.far_clip_ - 1.0F)));
    EXPECT_TRUE(volume_->IsCulled(near_bottom_left_ - 1.0F));
    EXPECT_TRUE(volume_->IsCulled(near_top_right_ + 1.0F));
    EXPECT_TRUE(volume_->IsCulled(far_bottom_left_ - 1.0F));
    EXPECT_TRUE(volume_->IsCulled(far_top_right_ + 1.0F));
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Sphere_InFrustrum) {
    auto near_z = near_bottom_left_.z_;
    auto far_z = far_bottom_left_.z_;
    math::Sphere sphere;

    sphere.center_ = math::Vec3f(0.0F, 0.0F, (far_z - near_z) * 0.5F);
    sphere.radius_ = 10.0F;
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.center_ = near_bottom_left_;
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.center_ = near_top_right_;
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.center_ = far_bottom_left_;
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.center_ = far_top_right_;
    EXPECT_FALSE(volume_->IsCulled(sphere));
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Sphere_OutOfFrustrum) {
    math::Sphere sphere;
    sphere.radius_ = 0.75F;

    math::Vec3f offset(1.0F);
    sphere.center_ = near_bottom_left_ - offset;
    EXPECT_TRUE(volume_->IsCulled(sphere));
    sphere.center_ = near_bottom_left_ + offset;
    EXPECT_TRUE(volume_->IsCulled(sphere));

    sphere.center_ = near_top_right_ - offset;
    EXPECT_TRUE(volume_->IsCulled(sphere));
    sphere.center_ = near_top_right_ + offset;
    EXPECT_TRUE(volume_->IsCulled(sphere));

    sphere.center_ = far_bottom_left_ - offset;
    EXPECT_TRUE(volume_->IsCulled(sphere));
    sphere.center_ = far_bottom_left_ + offset;
    EXPECT_TRUE(volume_->IsCulled(sphere));

    sphere.center_ = far_top_right_ - offset;
    EXPECT_TRUE(volume_->IsCulled(sphere));
    sphere.center_ = far_top_right_ + offset;
    EXPECT_TRUE(volume_->IsCulled(sphere));
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Box_InFrustrum) {
    math::Box box{near_bottom_left_, far_top_right_};
    EXPECT_FALSE(volume_->IsCulled(box));
}

TEST_F(TestPerspectiveViewVolume, IsCulled_Box_OutOfFrustrum) {
    math::Vec3f min{-100.0F, -100.0F, 100.0F};
    math::Vec3f max{-50.0F, -50.0F, 50.0F};
    math::Box box{min, max};
    EXPECT_TRUE(volume_->IsCulled(box));
}