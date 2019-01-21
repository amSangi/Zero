#include "gtest/gtest.h"
#include "Intersection.hpp"
#include "Box.hpp"
#include "Sphere.hpp"

using namespace zero;

TEST(TestIntersection, BoxSphereIntersect) {
	Sphere sphere(Vector3::Zero(), 100.0f);
	Box box(Vector3::Zero(), Vector3(25.0f));

	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
	box = Box(Vector3(0.0f, 0.0f, 99.9f), Vector3(125.0f));
	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
	box = Box(Vector3(-25.0f), Vector3(-0.001f, 0.001f, -0.001f));
	EXPECT_TRUE(Intersection::BoxSphereIntersect(box, sphere));
}