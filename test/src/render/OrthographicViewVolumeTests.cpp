#include "render/ViewVolumeBuilder.hpp"
#include "render/Components.hpp"
#include <gtest/gtest.h>
#include <memory>

using namespace zero;
using namespace zero::render;

class TestOrthographicViewVolume : public ::testing::Test {
protected:

    TestOrthographicViewVolume()
    : camera_(Camera::ProjectionType::ORTHOGRAPHIC)
    , volume_(nullptr)
    , near_bottom_left_()
    , near_top_right_()
    , far_bottom_left_()
    , far_top_right_()
    {}

    void SetUp() override {
        std::shared_ptr<IViewVolume> view_volume = ViewVolumeBuilder::create(camera_);
        volume_ = std::static_pointer_cast<OrthographicViewVolume>(view_volume);
        camera_.GetNearClipCoordinates(near_bottom_left_, near_top_right_);
        camera_.GetFarClipCoordinates(far_bottom_left_, far_top_right_);
    }

    Camera camera_;
    std::shared_ptr<OrthographicViewVolume> volume_;
    math::Vec3f near_bottom_left_;
    math::Vec3f near_top_right_;
    math::Vec3f far_bottom_left_;
    math::Vec3f far_top_right_;
};


TEST_F(TestOrthographicViewVolume, IsCulled_Point_InVolume) {
    EXPECT_FALSE(volume_->IsCulled(near_bottom_left_));
    EXPECT_FALSE(volume_->IsCulled(near_top_right_));
    EXPECT_FALSE(volume_->IsCulled(volume_->GetViewBox().Center()));
    EXPECT_FALSE(volume_->IsCulled(math::Vec3f(0.0F, 0.0F, (far_top_right_.z_ - near_top_right_.z_) * 0.5F)));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Point_OutOfVolume) {
    EXPECT_TRUE(volume_->IsCulled(camera_.position_));
    EXPECT_TRUE(volume_->IsCulled(far_bottom_left_));
    EXPECT_TRUE(volume_->IsCulled(far_top_right_));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_InVolume) {
    math::Sphere sphere(1000.0F);
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.radius_ = 10.0F;
    sphere.center_ = volume_->GetViewBox().Center();
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.center_ = near_bottom_left_;
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.center_ = near_top_right_;
    EXPECT_FALSE(volume_->IsCulled(sphere));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_OutOfVolume) {
    math::Sphere sphere(1.0F);
    EXPECT_TRUE(volume_->IsCulled(sphere));

    sphere.center_ = camera_.position_;
    EXPECT_TRUE(volume_->IsCulled(sphere));

    sphere.center_ = near_bottom_left_ + math::Vec3f(0.0F, 0.0F, 1.0F);
    EXPECT_TRUE(volume_->IsCulled(sphere));

    sphere.center_ = near_top_right_ + math::Vec3f(0.0F, 0.0F, 1.0F);
    EXPECT_TRUE(volume_->IsCulled(sphere));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_InVolume) {
    math::Box box = volume_->GetViewBox();
    EXPECT_FALSE(volume_->IsCulled(box));

    box.min_ = volume_->GetViewBox().Center();
    box.max_ = math::Vec3f(1000.0F, 1000.0F, 1000.0F);
    EXPECT_FALSE(volume_->IsCulled(box));

    box.min_ = near_bottom_left_;
    EXPECT_FALSE(volume_->IsCulled(box));

    box.min_ = near_top_right_;
    EXPECT_FALSE(volume_->IsCulled(box));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_OutOfVolume) {
    math::Box box = math::Box::Unit();
    EXPECT_TRUE(volume_->IsCulled(box));

    box.min_ = camera_.position_;
    box.max_ = math::Vec3f(100.0F, 100.0F, 100.0F);
    EXPECT_TRUE(volume_->IsCulled(box));

    box.min_ = near_bottom_left_ + math::Vec3f(0.0F, 0.0F, 1.0F);
    EXPECT_TRUE(volume_->IsCulled(box));

    box.min_ = near_top_right_ + math::Vec3f(0.0F, 0.0F, 1.0F);
    EXPECT_TRUE(volume_->IsCulled(box));
}