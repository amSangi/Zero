#include <random>
#include <gtest/gtest.h>
#include "math/Sphere.hpp"
#include "math/Box.hpp"

using namespace zero::math;

TEST(TestSphere, ContainsBox) {
	Sphere sphere(250.0F);

	EXPECT_TRUE(sphere.Contains(Box::Unit()));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f::Zero(), Vec3f(0.0F, 0.0F, 175.0F))));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f::Zero(), Vec3f(0.0F, 0.0F, 250.0F))));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f::Zero(), Vec3f(0.0F, 0.0F, 150.0F))));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f(0.0F, 0.0F, 120.0F), Vec3f(0.0F, 0.0F, 150.0F))));
	EXPECT_TRUE(sphere.Contains(Box(Vec3f(0.0F, 0.0F, -120.0F), Vec3f(0.0F, 0.0F, 150.0F))));

	EXPECT_FALSE(sphere.Contains(Box(Vec3f::Zero(), Vec3f(11350.0F))));
	EXPECT_FALSE(sphere.Contains(Box(Vec3f(-125.0F), Vec3f(1100.0F))));
	EXPECT_FALSE(sphere.Contains(Box(Vec3f(-125.0F), Vec3f(1100.0F))));
}


TEST(TestSphere, ContainsSphere) {
    Sphere big_sphere(100.0F);
    Sphere small_sphere(Vec3f(0.0F, 0.0F, 100.0F - 30.125F), 30.125F);

    EXPECT_TRUE(big_sphere.Contains(small_sphere));

    EXPECT_FALSE(small_sphere.Contains(big_sphere));
}

TEST(TestSphere, ContainsItself) {
    Sphere sphere{};

    sphere = Sphere::Zero();
    EXPECT_TRUE(sphere.Contains(sphere));
    sphere = Sphere(Vec3f(25.5F, 25.34F, 23.1F), 2341.5F);
    EXPECT_TRUE(sphere.Contains(sphere));
    sphere = Sphere(Vec3f(-251.34F, 234.1452F, -25134.13F), 0.1F);
    EXPECT_TRUE(sphere.Contains(sphere));
    sphere = Sphere::Zero();
    EXPECT_TRUE(sphere.Contains(sphere));
}

TEST(TestSphere, ContainsPoint) {
    Sphere sphere(Vec3f::Zero(), 100.0F);

    EXPECT_TRUE(sphere.Contains(Vec3f(0.F, 0.0F, 25.0F)));
    EXPECT_TRUE(sphere.Contains(Vec3f(0.0F, 0.0F, 99.9F)));
    EXPECT_TRUE(sphere.Contains(Vec3f(0.0F, 0.0F, 99.9F)));
    EXPECT_TRUE(sphere.Contains(Vec3f(0.0F, 0.0F, 100.0F)));

	EXPECT_FALSE(sphere.Contains(Vec3f(0.0F, 0.0F, 125.0F)));
	EXPECT_FALSE(sphere.Contains(Vec3f(0.0F, 0.0F, 100.00001F)));
	EXPECT_FALSE(sphere.Contains(Vec3f(-3420.234F, 10.0F, 234.0F)));
}

TEST(TestSphere, IntersectsSphere) {
	Sphere big_sphere(Vec3f::Zero(), 1100.0F);

	EXPECT_TRUE(big_sphere.Intersects(big_sphere));
	EXPECT_TRUE(big_sphere.Intersects(Sphere(Vec3f(100.0F, 0.0F, 0.0F), 25.0F)));
	EXPECT_TRUE(big_sphere.Intersects(Sphere(Vec3f(1100.1F, 0.0F, 0.0F), 0.1F)));

	EXPECT_FALSE(big_sphere.Intersects(Sphere(Vec3f(1100.1F, 0.0F, 0.0F), 0.05F)));
	EXPECT_FALSE(big_sphere.Intersects(Sphere(Vec3f(-10000.0F, 0.0F, 0.0F), 10.0F)));
}

TEST(TestSphere, Merge) {
	Sphere s1(Vec3f::Zero(), 10.0F);
	Sphere s2(Vec3f(25.0F, 25.0F, 25.0F), 10.0F);

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
    Sphere s1(Vec3f::Zero(), 25.5F);

    Sphere merged_itself = Sphere::Merge(s1, s1);

    EXPECT_TRUE(s1.Contains(s1));
    EXPECT_TRUE(s1.Contains(merged_itself));
    EXPECT_TRUE(merged_itself.Contains(s1));
    EXPECT_EQ(merged_itself, s1);

    s1.Merge(s1);
    EXPECT_EQ(s1, merged_itself);
}

TEST(TestSphere, MergeContainingSphere) {
    Sphere s1(Vec3f::Zero(), 100.0F);
    Sphere s2(Vec3f::One(), 25.0F);

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
    Sphere s1(Vec3f::Zero(), 100.0F);
    Sphere s2(Vec3f::One(), 25.0F);

    EXPECT_TRUE(s1.Contains(s2));
    EXPECT_FALSE(s2.Contains(s1));

    s2.Merge(s1);
    EXPECT_TRUE(s2.Contains(s1));
    EXPECT_EQ(s2, s1);
}

TEST(TestSphere, MergeLargerSphere) {
    Sphere s1(Vec3f::Zero(), 2.0F);
    Sphere s2(Vec3f(-1.0F, -2.0F, 0.0F), 2.0F);

    EXPECT_FALSE(s1.Contains(s2));
    EXPECT_FALSE(s2.Contains(s1));

    s1.Merge(s2);
    EXPECT_TRUE(s1.Contains(s2));
}

TEST(TestSphere, MergeSeveralSpheres) {
    std::vector<Sphere> spheres;
    std::random_device device;
    std::mt19937 generator(device());

    // Random radius generator
    float min_radius = 0.0F;
    float max_radius = 350.0F;
    std::uniform_real_distribution<float> radius_distribution(min_radius, max_radius);

    // Random 3D position generator
    float min_pos = -500.0F;
    float max_pos = 500.0F;
    std::uniform_real_distribution<float> pos_distribution(min_pos, max_pos);
    auto generate_random_pos = [&pos_distribution, &generator]() {
        return Vec3f(pos_distribution(generator),
                     pos_distribution(generator),
                     pos_distribution(generator));
    };

    // Generate random spheres
    int sphere_count = 500;
    for (int i = 0; i < sphere_count; ++i) {
        spheres.emplace_back(generate_random_pos(), radius_distribution(generator));
    }

    // Main sphere to encapsulate all the spheres
    Sphere main_sphere(Vec3f::Zero(), 1.0F);

    // Merge all the spheres with the main sphere
    for (int i = 0; i < sphere_count; ++i) {
        auto merged_sphere = Sphere::Merge(main_sphere, spheres[i]);
        EXPECT_TRUE(merged_sphere.Contains(main_sphere));
        EXPECT_TRUE(merged_sphere.Contains(spheres[i]));
        main_sphere = merged_sphere;
    }

    // Main sphere should contain all of the spheres
    for (int i = 0; i < sphere_count; ++i) {
        EXPECT_TRUE(main_sphere.Contains(spheres[i]));
    }
}

TEST(TestSphere, AABB) {
    Vec3f min{};
    Vec3f max{10.0F, 15.0F, 25.0F};

    Box box(min, max);
    Sphere sphere(min, max);

    EXPECT_EQ(sphere.center_, Vec3f(5.0F, 7.5F, 12.5F));
    EXPECT_GE(sphere.radius_, max.Magnitude() * 0.5F);
    EXPECT_TRUE(sphere.Contains(min));
    EXPECT_TRUE(sphere.Contains(max));
    EXPECT_TRUE(sphere.Contains(box));
    EXPECT_TRUE(sphere.Contains(sphere));
}