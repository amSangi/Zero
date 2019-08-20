#include <gtest/gtest.h>
#include "math/Intersection.hpp"
#include "math/Box.hpp"
#include "math/Sphere.hpp"

using namespace zero::math;

TEST(TestIntersection, BoxSphereIntersect) {
	Sphere sphere(Vec3f::Zero(), 100.0f);
	Box box(Vec3f::Zero(), Vec3f(25.0f));

	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
	box = Box(Vec3f(0.0f, 0.0f, 99.9f), Vec3f(125.0f));
	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
	box = Box(Vec3f(-25.0f), Vec3f(-0.001f, 0.001f, -0.001f));
	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
}