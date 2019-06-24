#include "gtest/gtest.h"
#include "math/Sphere.hpp"
#include "math/Box.hpp"

using namespace zero::math;

TEST(TestSphere, ContainsBox) {
	Sphere sphere(250.0f);

	EXPECT_TRUE(sphere.Contains(Box::Unit()));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f::Zero(), Vec3f(0.0f, 0.0f, 175.0f))));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f::Zero(), Vec3f(0.0f, 0.0f, 250.0f))));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f::Zero(), Vec3f(0.0f, 0.0f, 150.0f))));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f(0.0f, 0.0f, 120.0f), Vec3f(0.0f, 0.0f, 150.0f))));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f(0.0f, 0.0f, -120.0f), Vec3f(0.0f, 0.0f, 150.0f))));

	EXPECT_FALSE(sphere.Contains(Box(Vec3f::Zero(), Vec3f(11350.0f))));
	EXPECT_FALSE(sphere.Contains(Box(Vec3f(-125.0f), Vec3f(1100.0f))));
	EXPECT_FALSE(sphere.Contains(Box(Vec3f(-125.0f), Vec3f(1100.0f))));
}


TEST(TestSphere, ContainsSphere) {
    Sphere big_sphere(100.0f);
    Sphere small_sphere(Vec3f(0.0f, 0.0f, 100.0f - 30.125f), 30.125f);

    EXPECT_TRUE(big_sphere.Contains(small_sphere));

    EXPECT_FALSE(small_sphere.Contains(big_sphere));
}

TEST(TestSphere, ContainsItself) {
    Sphere sphere{};

    sphere = Sphere::Zero();
    EXPECT_TRUE(sphere.Contains(sphere));
    sphere = Sphere(Vec3f(25.5f, 25.34f, 23.1f), 2341.5f);
    EXPECT_TRUE(sphere.Contains(sphere));
    sphere = Sphere(Vec3f(-251.34f, 234.1452f, -25134.13f), 0.1f);
    EXPECT_TRUE(sphere.Contains(sphere));
}

TEST(TestSphere, ContainsPoint) {
    Sphere sphere(Vec3f::Zero(), 100.0f);

    EXPECT_TRUE(sphere.Contains(Vec3f(0.f, 0.0f, 25.0f)));
    EXPECT_TRUE(sphere.Contains(Vec3f(0.0f, 0.0f, 99.9f)));
    EXPECT_TRUE(sphere.Contains(Vec3f(0.0f, 0.0f, 99.9f)));
    EXPECT_TRUE(sphere.Contains(Vec3f(0.0f, 0.0f, 100.0f)));

	EXPECT_FALSE(sphere.Contains(Vec3f(0.0f, 0.0f, 125.0f)));
	EXPECT_FALSE(sphere.Contains(Vec3f(0.0f, 0.0f, 100.00001f)));
	EXPECT_FALSE(sphere.Contains(Vec3f(-3420.234f, 10.0f, 234.0f)));
}

TEST(TestSphere, IntersectsSphere) {
	Sphere big_sphere(Vec3f::Zero(), 1100.0f);

	EXPECT_TRUE(big_sphere.Intersects(big_sphere));
	EXPECT_TRUE(big_sphere.Intersects(Sphere(Vec3f(100.0f, 0.0f, 0.0f), 25.0f)));
	EXPECT_TRUE(big_sphere.Intersects(Sphere(Vec3f(1100.1f, 0.0f, 0.0f), 0.1f)));

	EXPECT_FALSE(big_sphere.Intersects(Sphere(Vec3f(1100.1f, 0.0f, 0.0f), 0.05f)));
	EXPECT_FALSE(big_sphere.Intersects(Sphere(Vec3f(-10000.0f, 0.0f, 0.0f), 10.0f)));
}

TEST(TestSphere, Merge) {
	Sphere s1(Vec3f::Zero(), 10.0f);
	Sphere s2(Vec3f(25.0f, 25.0f, 25.0f), 10.0f);

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
	EXPECT_EQ(s1, merged_sphere);
}

TEST(TestSphere, MergeItself) {
    Sphere s1(Vec3f::Zero(), 25.5f);

    Sphere merged_itself = Sphere::Merge(s1, s1);

    EXPECT_TRUE(s1.Contains(s1));
    EXPECT_TRUE(s1.Contains(merged_itself));
    EXPECT_TRUE(merged_itself.Contains(s1));
    EXPECT_EQ(merged_itself, s1);

    s1.Merge(s1);
    EXPECT_EQ(s1, merged_itself);
}

TEST(TestSphere, MergeContainingSphere) {
    Sphere s1(Vec3f::Zero(), 100.0f);
    Sphere s2(Vec3f::One(), 25.0f);

    Sphere merged_sphere = Sphere::Merge(s1, s2);

    EXPECT_TRUE(s1.Contains(s2));
    EXPECT_FALSE(s2.Contains(s1));
    EXPECT_TRUE(merged_sphere.Contains(s1));
    EXPECT_TRUE(merged_sphere.Contains(s2));
    EXPECT_EQ(merged_sphere, s1);

    s1.Merge(s2);
    EXPECT_EQ(s1, merged_sphere);
}

TEST(TestSphere, MergeInsideSphere) {
    Sphere s1(Vec3f::Zero(), 100.0f);
    Sphere s2(Vec3f::One(), 25.0f);

    EXPECT_TRUE(s1.Contains(s2));
    EXPECT_FALSE(s2.Contains(s1));

    s2.Merge(s1);
    EXPECT_TRUE(s2.Contains(s1));
    EXPECT_EQ(s2, s1);
}