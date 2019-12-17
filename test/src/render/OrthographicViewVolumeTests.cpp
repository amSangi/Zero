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
}

TEST_F(TestOrthographicViewVolume, IsCulled_Point_OutOfVolume) {
    EXPECT_TRUE(volume_->IsCulled(camera_.position_));
    EXPECT_TRUE(volume_->IsCulled(far_bottom_left_));
    EXPECT_TRUE(volume_->IsCulled(far_top_right_));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_InVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_OutOfVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_InVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_OutOfVolume) {
    FAIL();
}