#include <gtest/gtest.h>
#include "math/Plane.hpp"
#include "math/ZMath.hpp"

using namespace zero::math;

TEST(TestPlane, PlaneFromThreePoints) {
    Vec3f origin = Vec3f::Zero();
	Vec3f left = Vec3f::Left();
	Vec3f right = Vec3f::Right();
	Vec3f forward = Vec3f::Forward();
	Vec3f back = Vec3f::Back();
	Vec3f up = Vec3f::Up();
	Vec3f down = Vec3f::Down();

	// Points are in counter clockwise order around the normal
	EXPECT_EQ(Plane(origin, forward, right).normal_, up);
	EXPECT_EQ(Plane(origin, right, forward).normal_, down);
    EXPECT_EQ(Plane(origin, forward, left).normal_, down);
    EXPECT_EQ(Plane(origin, left, forward).normal_, up);

	EXPECT_EQ(Plane(origin, forward, up).normal_, left);
	EXPECT_EQ(Plane(origin, up, forward).normal_, right);
    EXPECT_EQ(Plane(origin, back, up).normal_, right);
    EXPECT_EQ(Plane(origin, up, back).normal_, left);

	EXPECT_EQ(Plane(origin, right, up).normal_, forward);
	EXPECT_EQ(Plane(origin, up, right).normal_, back);
	EXPECT_EQ(Plane(origin, left, up).normal_, back);
	EXPECT_EQ(Plane(origin, up, left).normal_, forward);
}

TEST(TestPlane, Intersects) {
	Plane up_plane = Plane::Up();
	Plane right_plane = Plane::Right();
	Plane forward_plane = Plane::Forward();

	// Up plane
	EXPECT_TRUE(up_plane.Intersects(right_plane));
	EXPECT_TRUE(up_plane.Intersects(right_plane.Flip()));
	EXPECT_TRUE(up_plane.Intersects(forward_plane));
	EXPECT_TRUE(up_plane.Intersects(forward_plane.Flip()));

	EXPECT_FALSE(up_plane.Intersects(up_plane));
	EXPECT_FALSE(up_plane.Intersects(up_plane.Flip()));

	// Right plane
	EXPECT_TRUE(right_plane.Intersects(up_plane));
	EXPECT_TRUE(right_plane.Intersects(up_plane.Flip()));
	EXPECT_TRUE(right_plane.Intersects(forward_plane));
	EXPECT_TRUE(right_plane.Intersects(forward_plane.Flip()));

	EXPECT_FALSE(right_plane.Intersects(right_plane));
	EXPECT_FALSE(right_plane.Intersects(right_plane.Flip()));

	// Forward plane
	EXPECT_TRUE(forward_plane.Intersects(up_plane));
	EXPECT_TRUE(forward_plane.Intersects(up_plane.Flip()));
	EXPECT_TRUE(forward_plane.Intersects(right_plane));
	EXPECT_TRUE(forward_plane.Intersects(right_plane.Flip()));

	EXPECT_FALSE(forward_plane.Intersects(forward_plane));
	EXPECT_FALSE(forward_plane.Intersects(forward_plane.Flip()));
}

TEST(TestPlane, Reflect) {
	Plane plane = Plane::Up();
	EXPECT_EQ(plane.Reflect(Vec3f(1.0F, 1.0F)), Vec3f(1.0F, -1.0F));
	EXPECT_EQ(plane.Reflect(Vec3f(-1.0F, 1.0F)), Vec3f(-1.0F, -1.0F));

	plane = Plane::Right();
	EXPECT_EQ(plane.Reflect(Vec3f(-1.0F, -1.0F)), Vec3f(1.0F, -1.0F));
	EXPECT_EQ(plane.Reflect(Vec3f(1.0F, 1.0F)), Vec3f(-1.0F, 1.0F));

	plane = Plane::Forward();
	EXPECT_EQ(plane.Reflect(Vec3f(0.0F, -1.0F, -1.0F)), Vec3f(0.0F, -1.0F, 1.0F));
	EXPECT_EQ(plane.Reflect(Vec3f(0.0F, 1.0F, -1.0F)), Vec3f(0.0F, 1.0F, 1.0F));
}

TEST(TestPlane, Distance) {
	Plane plane = Plane::Up();

	EXPECT_EQ(plane.Distance(Vec3f::Zero()), 0.0F);
	EXPECT_EQ(plane.Distance(Vec3f(1.0F, 0.0F)), 0.0F);
	EXPECT_EQ(plane.Distance(Vec3f::One()), 1.0F);
	EXPECT_EQ(plane.Distance(Vec3f(25.0F, 50.0F, 75.0F)), 50.0F);

	plane = Plane(Vec3f::One(), 15.0F);
	EXPECT_EQ(plane.Distance(Vec3f::Zero()), 15.0F);
}
