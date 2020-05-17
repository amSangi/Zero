#include <gtest/gtest.h>
#include "math/Intersection.hpp"
#include "math/Box.hpp"
#include "math/Sphere.hpp"

using namespace zero::math;

TEST(TestIntersection, BoxSphereIntersect)
{
	Sphere sphere(Vec3f::Zero(), 100.0F);
	Box box(Vec3f::Zero(), Vec3f(25.0F));

	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
	box = Box(Vec3f(0.0F, 0.0F, 99.9F), Vec3f(125.0F));
	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
	box = Box(Vec3f(-25.0F), Vec3f(-0.001F, 0.001F, -0.001F));
	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
}