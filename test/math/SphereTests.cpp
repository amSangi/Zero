#include "gtest/gtest.h"
#include "Sphere.hpp"
#include "Box.hpp"

using namespace zero;

TEST(TestSphere, ContainsBox) {
	Sphere sphere(250.0f);

	EXPECT_TRUE(sphere.Contains(Box::Unit()));
	EXPECT_TRUE(sphere.Contains(Box(Vector3::Zero(), Vector3(0.0f, 0.0f, 175.0f))));
	EXPECT_TRUE(sphere.Contains(Box(Vector3::Zero(), Vector3(0.0f, 0.0f, 250.0f))));
	EXPECT_TRUE(sphere.Contains(Box(Vector3::Zero(), Vector3(0.0f, 0.0f, 150.0f))));
	EXPECT_TRUE(sphere.Contains(Box(Vector3(0.0f, 0.0f, 120.0f), Vector3(0.0f, 0.0f, 150.0f))));
	EXPECT_TRUE(sphere.Contains(Box(Vector3(0.0f, 0.0f, -120.0f), Vector3(0.0f, 0.0f, 150.0f))));

	EXPECT_FALSE(sphere.Contains(Box(Vector3::Zero(), Vector3(11350.0f))));
	EXPECT_FALSE(sphere.Contains(Box(Vector3(-125.0f), Vector3(1100.0f))));
	EXPECT_FALSE(sphere.Contains(Box(Vector3(-125.0f), Vector3(1100.0f))));
}


TEST(TestSphere, ContainsSphere) {
    Sphere big_sphere(100.0f);
    Sphere small_sphere(Vector3(0.0f, 0.0f, 100.0f - 30.125f), 30.125f);

    EXPECT_TRUE(big_sphere.Contains(small_sphere));

    EXPECT_FALSE(small_sphere.Contains(big_sphere));
}

TEST(TestSphere, ContainsItself) {
    Sphere sphere{};

    sphere = Sphere::Zero();
    EXPECT_TRUE(sphere.Contains(sphere));
    sphere = Sphere(Vector3(25.5f, 25.34f, 23.1f), 2341.5f);
    EXPECT_TRUE(sphere.Contains(sphere));
    sphere = Sphere(Vector3(-251.34f, 234.1452f, -25134.13f), 0.1f);
    EXPECT_TRUE(sphere.Contains(sphere));
}

TEST(TestSphere, ContainsPoint) {
    Sphere sphere(Vector3::Zero(), 100.0f);

    EXPECT_TRUE(sphere.Contains(Vector3(0.f, 0.0f, 25.0f)));
    EXPECT_TRUE(sphere.Contains(Vector3(0.0f, 0.0f, 99.9f)));
    EXPECT_TRUE(sphere.Contains(Vector3(0.0f, 0.0f, 99.9f)));
    EXPECT_TRUE(sphere.Contains(Vector3(0.0f, 0.0f, 100.0f)));

	EXPECT_FALSE(sphere.Contains(Vector3(0.0f, 0.0f, 125.0f)));
	EXPECT_FALSE(sphere.Contains(Vector3(0.0f, 0.0f, 100.00001f)));
	EXPECT_FALSE(sphere.Contains(Vector3(-3420.234f, 10.0f, 234.0f)));
}

TEST(TestSphere, IntersectsSphere) {
	Sphere big_sphere(Vector3::Zero(), 1100.0f);

	EXPECT_TRUE(big_sphere.Intersects(big_sphere));
	EXPECT_TRUE(big_sphere.Intersects(Sphere(Vector3(100.0f, 0.0f, 0.0f), 25.0f)));
	EXPECT_TRUE(big_sphere.Intersects(Sphere(Vector3(1100.1f, 0.0f, 0.0f), 0.1f)));

	EXPECT_FALSE(big_sphere.Intersects(Sphere(Vector3(1100.1f, 0.0f, 0.0f), 0.05f)));
	EXPECT_FALSE(big_sphere.Intersects(Sphere(Vector3(-10000.0f, 0.0f, 0.0f), 10.0f)));
}

TEST(TestSphere, Merge) {
	Sphere s1(Vector3::Zero(), 10.0f);
	Sphere s2(Vector3(25.0f, 25.0f, 25.0f), 10.0f);

	Sphere merged_sphere = Sphere::Merge(s1, s2);

	EXPECT_FALSE(s1.Contains(s2));
	EXPECT_FALSE(s2.Contains(s1));
	EXPECT_TRUE(merged_sphere.Contains(s1));
	EXPECT_TRUE(merged_sphere.Contains(s2));
	EXPECT_FALSE(s1.Contains(merged_sphere));
	EXPECT_FALSE(s2.Contains(merged_sphere));
	EXPECT_TRUE(merged_sphere.radius_ >= s1.radius_);
	EXPECT_TRUE(merged_sphere.radius_ >= s2.radius_);

	s1.Merge(s2);
	EXPECT_TRUE(s1.Contains(s2));
	EXPECT_FALSE(s2.Contains(s1));
	EXPECT_TRUE(s1.radius_ >= s2.radius_);
	EXPECT_EQ(merged_sphere, s1);
}

TEST(TestSphere, MergeItself) {
    Sphere s1(Vector3::Zero(), 25.5f);

    Sphere merged_itself = Sphere::Merge(s1, s1);

    EXPECT_TRUE(s1.Contains(s1));
    EXPECT_TRUE(s1.Contains(merged_itself));
    EXPECT_TRUE(merged_itself.Contains(s1));
    EXPECT_EQ(s1, merged_itself);

    s1.Merge(s1);
    EXPECT_EQ(merged_itself, s1);
}

TEST(TestSphere, MergeContainingSphere) {
    Sphere s1(Vector3::Zero(), 100.0f);
    Sphere s2(Vector3::One(), 25.0f);

    Sphere merged_sphere = Sphere::Merge(s1, s2);

    EXPECT_TRUE(s1.Contains(s2));
    EXPECT_FALSE(s2.Contains(s1));
    EXPECT_TRUE(merged_sphere.Contains(s1));
    EXPECT_TRUE(merged_sphere.Contains(s2));
    EXPECT_EQ(s1, merged_sphere);

    s1.Merge(s2);
    EXPECT_EQ(merged_sphere, s1);
}

TEST(TestSphere, MergeInsideSphere) {
    Sphere s1(Vector3::Zero(), 100.0f);
    Sphere s2(Vector3::One(), 25.0f);

    EXPECT_TRUE(s1.Contains(s2));
    EXPECT_FALSE(s2.Contains(s1));

    s2.Merge(s1);
    EXPECT_TRUE(s2.Contains(s1));
    EXPECT_EQ(s1, s2);
}