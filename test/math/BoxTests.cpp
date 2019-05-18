#include "gtest/gtest.h"
#include "math/Box.hpp"

using namespace zero::math;

TEST(TestBox, ContainsBox) {
	Box box(Vec3f::Zero(), Vec3f(100.0f));

	EXPECT_TRUE(box.Contains(box));
	EXPECT_TRUE(box.Contains(Box::Unit()));
	EXPECT_TRUE(box.Contains(Box(Vec3f(25.0f), Vec3f(100.0f))));

	EXPECT_FALSE(box.Contains(Box(Vec3f(-100.0f), Vec3f(100.0f))));
	EXPECT_FALSE(box.Contains(Box(Vec3f(0.0f, 0.0f, -1.0f), Vec3f(100.0f))));
	EXPECT_FALSE(box.Contains(Box(Vec3f::Zero(), Vec3f(100.1f))));
}

TEST(TestBox, ContainsPoint) {
	Box box(Vec3f::Zero(), Vec3f(100.0f));

	EXPECT_TRUE(box.Contains(Vec3f::Zero()));
	EXPECT_TRUE(box.Contains(Vec3f(100.0f)));

	EXPECT_FALSE(box.Contains(Vec3f(100.0f, 100.0f, 100.0001f)));
	EXPECT_FALSE(box.Contains(Vec3f(0.0f, 0.0f, -0.0001f)));
}

TEST(TestBox, IntersectsBox) {
	Box box(Vec3f::Zero(), Vec3f(100.0f));

	EXPECT_TRUE(box.Intersects(box));
	EXPECT_TRUE(box.Intersects(Box::Unit()));
	EXPECT_TRUE(box.Intersects(Box(Vec3f(99.9999f), Vec3f(1000.0f))));
	EXPECT_TRUE(box.Intersects(Box(Vec3f(-1000.0f), Vec3f(0.0f, 0.0f, 0.1f))));

	EXPECT_FALSE(box.Intersects(Box(Vec3f(125.0f), Vec3f(250.0f))));
	EXPECT_FALSE(box.Intersects(Box(Vec3f(-125.0f), Vec3f(-10.0f))));
	EXPECT_FALSE(box.Intersects(Box(Vec3f(-125.0f), Vec3f(-0.001f))));
}

TEST(TestBox, Merge) {
	Box box(Vec3f::Zero(), Vec3f(100.0f));
	Box other_box(Vec3f(100.0f), Vec3f(250.0f));

	EXPECT_EQ(box, Box::Merge(box, box));
	EXPECT_EQ(Box(Vec3f::Zero(), Vec3f(250.0f)), Box::Merge(box, other_box));
}

TEST(TestBox, MergeContainingBox) {
	Box box(Vec3f(-1000.0f), Vec3f(1000.0f));
	Box other_box(Vec3f::Zero(), Vec3f(250.0f));

	EXPECT_EQ(box, Box::Merge(box, other_box));
	other_box = Box(Vec3f(-100.0f), Vec3f(100.1f));
	EXPECT_EQ(box, Box::Merge(box, other_box));
}

TEST(TestBox, MergeInsideBox) {
	Box box(Vec3f::Zero(), Vec3f(100.0f));
	Box other_box(Vec3f::Zero(), Vec3f(250.0f));

	EXPECT_EQ(other_box, Box::Merge(box, other_box));
	other_box = Box(Vec3f(-100.0f), Vec3f(100.1f));
	EXPECT_EQ(other_box, Box::Merge(box, other_box));
}

TEST(TestBox, Size) {
	EXPECT_EQ(Vec3f::One(), Box::Unit().Size());
	EXPECT_EQ(Vec3f(100.0f), Box(Vec3f::Zero(), Vec3f(100.0f)).Size());
	EXPECT_EQ(Vec3f(50.0f), Box(Vec3f(-25.0f), Vec3f(25.0f)).Size());
}

TEST(TestBox, Center) {
	EXPECT_EQ(Vec3f(0.5f), Box::Unit().Center());
	EXPECT_EQ(Vec3f(50.0f), Box(Vec3f::Zero(), Vec3f(100.0f)).Center());
	EXPECT_EQ(Vec3f(0.0f), Box(Vec3f(-25.0f), Vec3f(25.0f)).Center());
}