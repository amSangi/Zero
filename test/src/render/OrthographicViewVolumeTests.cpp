#include "render/scene/ViewVolumeBuilder.hpp"
#include "render/scene/OrthographicViewVolume.hpp"
#include <gtest/gtest.h>
#include <memory>

using namespace zero;
using namespace zero::render;

class TestOrthographicViewVolume : public ::testing::Test
{
protected:

    TestOrthographicViewVolume()
    : camera_(Camera::ProjectionType::ORTHOGRAPHIC)
    , volume_(nullptr)
    , min_()
    , max_()
    , difference_()
    {
    }

    void SetUp() override
    {
        std::shared_ptr<IViewVolume> view_volume = ViewVolumeBuilder::create(camera_);
        volume_ = std::static_pointer_cast<OrthographicViewVolume>(view_volume);
        min_ = volume_->GetViewBox().min_;
        max_ = volume_->GetViewBox().max_;
        difference_ = max_ - min_;
    }

    Camera camera_;
    std::shared_ptr<OrthographicViewVolume> volume_;
    math::Vec3f min_;
    math::Vec3f max_;
    math::Vec3f difference_;
};


TEST_F(TestOrthographicViewVolume, IsCulled_Point_InVolume)
{
    EXPECT_FALSE(volume_->IsCulled(min_));
    EXPECT_FALSE(volume_->IsCulled(max_));
    EXPECT_FALSE(volume_->IsCulled(volume_->GetViewBox().Center()));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Point_OutOfVolume)
{
    EXPECT_TRUE(volume_->IsCulled(min_ - difference_));
    EXPECT_TRUE(volume_->IsCulled(max_ + difference_));
    EXPECT_TRUE(volume_->IsCulled(camera_.position_));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_InVolume)
{
    math::Sphere sphere(1000.0F);
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.radius_ = 10.0F;
    sphere.center_ = volume_->GetViewBox().Center();
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.center_ = min_;
    EXPECT_FALSE(volume_->IsCulled(sphere));

    sphere.center_ = max_;
    EXPECT_FALSE(volume_->IsCulled(sphere));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_OutOfVolume)
{
    math::Sphere sphere(1.0F);
    sphere.center_ = volume_->GetViewBox().Center() - difference_;
    EXPECT_TRUE(volume_->IsCulled(sphere));

    sphere.center_ = min_ - difference_;
    EXPECT_TRUE(volume_->IsCulled(sphere));

    sphere.center_ = max_ + difference_;
    EXPECT_TRUE(volume_->IsCulled(sphere));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_InVolume)
{
    math::Box box = volume_->GetViewBox();
    EXPECT_FALSE(volume_->IsCulled(box));

    box.min_ = volume_->GetViewBox().Center();
    box.max_ = math::Vec3f(10000.0F, 10000.0F, 10000.0F);
    EXPECT_FALSE(volume_->IsCulled(box));

    box.min_ = min_;
    EXPECT_FALSE(volume_->IsCulled(box));

    box.min_ = max_;
    EXPECT_FALSE(volume_->IsCulled(box));
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_OutOfVolume)
{
    math::Box box{};
    box.min_ = max_ + math::kEpsilon;
    box.max_ = math::Vec3f(10000.0F, 10000.0F, 10000.0F);
    EXPECT_TRUE(volume_->IsCulled(box));

    box.min_ = min_ - difference_;
    box.max_ = min_ - math::kEpsilon;
    EXPECT_TRUE(volume_->IsCulled(box));

    box.min_ = max_ + math::kEpsilon;
    box.max_ = max_ + difference_;
    EXPECT_TRUE(volume_->IsCulled(box));
}