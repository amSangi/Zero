#include "components/Camera.hpp"
#include <gtest/gtest.h>

using namespace zero;

TEST(TestCamera, LookAt)
{
    Camera camera;
    EXPECT_EQ(camera.GetViewDirection(), math::Vec3f::Back());
    camera.LookAt(math::Vec3f::Up());
    EXPECT_EQ(camera.GetViewDirection(), math::Vec3f::Up());
}

TEST(TestCamera, LookAt_MoveAndLookLeft)
{
    Camera camera;
    camera.position_ = math::Vec3f::Forward();
    camera.LookAt(math::Vec3f::Left());
    EXPECT_EQ(camera.GetViewDirection(), math::Vec3f::Left());
    EXPECT_EQ(camera.position_, math::Vec3f::Forward());
}

TEST(TestCamera, GetVerticalFieldOfView)
{
    Camera camera;
    camera.viewport_.width_ = 1920;
    camera.viewport_.height_ = 1080;
    camera.horizontal_field_of_view_ = math::Degree(90.0F);
    auto degrees = camera.GetVerticalFieldOfView().ToDegree().deg_;
    EXPECT_TRUE(math::Equal(degrees, 59.0F, 0.5F));
}