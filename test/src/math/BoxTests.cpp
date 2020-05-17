#include <gtest/gtest.h>
#include "math/Box.hpp"

using namespace zero::math;

TEST(TestBox, ContainsBox)
{
	Box box(Vec3f::Zero(), Vec3f(100.0F));

	EXPECT_TRUE(box.Contains(box));
	EXPECT_TRUE(box.Contains(Box::Unit()));
	EXPECT_TRUE(box.Contains(Box(Vec3f(25.0F), Vec3f(100.0F))));

	EXPECT_FALSE(box.Contains(Box(Vec3f(-100.0F), Vec3f(100.0F))));
	EXPECT_FALSE(box.Contains(Box(Vec3f(0.0F, 0.0F, -1.0F), Vec3f(100.0F))));
	EXPECT_FALSE(box.Contains(Box(Vec3f::Zero(), Vec3f(100.1F))));

    box.min_ = Vec3f(-50.0F, -50.0F, -500.0F);
    box.max_ = Vec3f(50.0F, 50.0F, -50.0F);
    EXPECT_TRUE(box.Contains(box));
}

TEST(TestBox, ContainsPoint)
{
	Box box(Vec3f::Zero(), Vec3f(100.0F));

	EXPECT_TRUE(box.Contains(Vec3f::Zero()));
	EXPECT_TRUE(box.Contains(Vec3f(100.0F)));

	EXPECT_FALSE(box.Contains(Vec3f(100.0F, 100.0F, 100.0001F)));
	EXPECT_FALSE(box.Contains(Vec3f(0.0F, 0.0F, -0.0001F)));

	box.min_ = Vec3f(-50.0F, -50.0F, -500.0F);
	box.max_ = Vec3f(50.0F, 50.0F, -50.0F);
	EXPECT_TRUE(box.Contains(box.min_));
	EXPECT_TRUE(box.Contains(box.max_));
}

TEST(TestBox, IntersectsBox)
{
	Box box(Vec3f::Zero(), Vec3f(100.0F));

	EXPECT_TRUE(box.Intersects(box));
	EXPECT_TRUE(box.Intersects(Box::Unit()));
	EXPECT_TRUE(box.Intersects(Box(Vec3f(99.9999F), Vec3f(1000.0F))));
	EXPECT_TRUE(box.Intersects(Box(Vec3f(-1000.0F), Vec3f(0.0F, 0.0F, 0.1F))));

	EXPECT_FALSE(box.Intersects(Box(Vec3f(125.0F), Vec3f(250.0F))));
	EXPECT_FALSE(box.Intersects(Box(Vec3f(-125.0F), Vec3f(-10.0F))));
	EXPECT_FALSE(box.Intersects(Box(Vec3f(-125.0F), Vec3f(-0.001F))));
}

TEST(TestBox, Merge)
{
	Box box(Vec3f::Zero(), Vec3f(100.0F));
	Box other_box(Vec3f(100.0F), Vec3f(250.0F));

	EXPECT_EQ(Box::Merge(box, box), box);
	EXPECT_EQ(Box::Merge(box, other_box), Box(Vec3f::Zero(), Vec3f(250.0F)));
}

TEST(TestBox, MergeContainingBox)
{
	Box box(Vec3f(-1000.0F), Vec3f(1000.0F));
	Box other_box(Vec3f::Zero(), Vec3f(250.0F));

	EXPECT_EQ(Box::Merge(box, other_box), box);
	other_box = Box(Vec3f(-100.0F), Vec3f(100.1F));
	EXPECT_EQ(Box::Merge(box, other_box), box);
}

TEST(TestBox, MergeInsideBox)
{
	Box box(Vec3f::Zero(), Vec3f(100.0F));
	Box other_box(Vec3f::Zero(), Vec3f(250.0F));

	EXPECT_EQ(Box::Merge(box, other_box), other_box);
	other_box = Box(Vec3f(-100.0F), Vec3f(100.1F));
	EXPECT_EQ(Box::Merge(box, other_box), other_box);
}

TEST(TestBox, Size)
{
	EXPECT_EQ(Box::Unit().Size(), Vec3f::One());
	EXPECT_EQ(Box(Vec3f::Zero(), Vec3f(100.0F)).Size(), Vec3f(100.0F));
	EXPECT_EQ(Box(Vec3f(-25.0F), Vec3f(25.0F)).Size(), Vec3f(50.0F));
}

TEST(TestBox, Center)
{
	EXPECT_EQ(Box::Unit().Center(), Vec3f(0.5F));
	EXPECT_EQ(Box(Vec3f::Zero(), Vec3f(100.0F)).Center(), Vec3f(50.0F));
	EXPECT_EQ(Box(Vec3f(-25.0F), Vec3f(25.0F)).Center(), Vec3f(0.0F));
}