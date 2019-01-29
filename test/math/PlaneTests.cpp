#include "gtest/gtest.h"
#include "Plane.hpp"
#include "ZMath.hpp"

using namespace zero::math;

TEST(TestPlane, Constructor) {
	Vector3 left = Vector3::Left();
	Vector3 right = Vector3::Right();
	Vector3 forward = Vector3::Forward();
	Vector3 up = Vector3::Up();
	Vector3 down = Vector3::Down();

	Plane plane(right, left, forward);
	EXPECT_EQ(Plane::Up(), plane);
	EXPECT_EQ(Plane::Up().Flip(), Plane(left, right, forward));


	EXPECT_EQ(Plane::Right(), Plane(forward, down, up));
	EXPECT_EQ(Plane::Right().Flip(), Plane(forward, up, down));
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
	EXPECT_EQ(Vector3(1.0f, -1.0f), plane.Reflect(Vector3(1.0f, 1.0f)));
	EXPECT_EQ(Vector3(-1.0f, -1.0f), plane.Reflect(Vector3(-1.0f, 1.0f)));

	plane = Plane::Right();
	EXPECT_EQ(Vector3(1.0f, -1.0f), plane.Reflect(Vector3(-1.0f, -1.0f)));
	EXPECT_EQ(Vector3(-1.0f, 1.0f), plane.Reflect(Vector3(1.0f, 1.0f)));

	plane = Plane::Forward();
	EXPECT_EQ(Vector3(0.0f, -1.0f, 1.0f), plane.Reflect(Vector3(0.0f, -1.0f, -1.0f)));
	EXPECT_EQ(Vector3(0.0f, 1.0f, 1.0f), plane.Reflect(Vector3(0.0f, 1.0f, -1.0f)));
}

TEST(TestPlane, Distance) {
	Plane plane = Plane::Up();

	EXPECT_EQ(0.0f, plane.Distance(Vector3::Zero()));
	EXPECT_EQ(0.0f, plane.Distance(Vector3(1.0f, 0.0f)));
	EXPECT_EQ(1.0f, plane.Distance(Vector3::One()));
	EXPECT_EQ(50.0f, plane.Distance(Vector3(25.0f, 50.0f, 75.0f)));

	plane = Plane(Vector3::One(), 15.0f);
	EXPECT_EQ(15.0f, plane.Distance(Vector3::Zero()));
}
