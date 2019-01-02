#include "gtest/gtest.h"
#include "Box.h"

using namespace Zero;

TEST(TestBox, ContainsBox) {
	Box box(Vector3::Zero(), Vector3(100.0f));

	EXPECT_TRUE(box.Contains(box));
	EXPECT_TRUE(box.Contains(Box::Unit()));
	EXPECT_TRUE(box.Contains(Box(Vector3(25.0f), Vector3(100.0f))));

	EXPECT_FALSE(box.Contains(Box(Vector3(-100.0f), Vector3(100.0f))));
	EXPECT_FALSE(box.Contains(Box(Vector3(0.0f, 0.0f, -1.0f), Vector3(100.0f))));
	EXPECT_FALSE(box.Contains(Box(Vector3::Zero(), Vector3(100.1f))));
}

TEST(TestBox, ContainsPoint) {
	Box box(Vector3::Zero(), Vector3(100.0f));

	EXPECT_TRUE(box.Contains(Vector3::Zero()));
	EXPECT_TRUE(box.Contains(Vector3(100.0f)));

	EXPECT_FALSE(box.Contains(Vector3(100.0f, 100.0f, 100.0001f)));
	EXPECT_FALSE(box.Contains(Vector3(0.0f, 0.0f, -0.0001f)));
}

TEST(TestBox, IntersectsBox) {
	Box box(Vector3::Zero(), Vector3(100.0f));

	EXPECT_TRUE(box.Intersects(box));
	EXPECT_TRUE(box.Intersects(Box::Unit()));
	EXPECT_TRUE(box.Intersects(Box(Vector3(99.9999f), Vector3(1000.0f))));
	EXPECT_TRUE(box.Intersects(Box(Vector3(-1000.0f), Vector3(0.0f, 0.0f, 0.1f))));

	EXPECT_FALSE(box.Intersects(Box(Vector3(125.0f), Vector3(250.0f))));
	EXPECT_FALSE(box.Intersects(Box(Vector3(-125.0f), Vector3(-10.0f))));
	EXPECT_FALSE(box.Intersects(Box(Vector3(-125.0f), Vector3(-0.001f))));
}

TEST(TestBox, Merge) {
	Box box(Vector3::Zero(), Vector3(100.0f));
	Box other_box(Vector3(100.0f), Vector3(250.0f));

	EXPECT_EQ(box, Box::Merge(box, box));
	EXPECT_EQ(Box(Vector3::Zero(), Vector3(250.0f)), Box::Merge(box, other_box));
}

TEST(TestBox, MergeContainingBox) {
	Box box(Vector3(-1000.0f), Vector3(1000.0f));
	Box other_box(Vector3::Zero(), Vector3(250.0f));

	EXPECT_EQ(box, Box::Merge(box, other_box));
	other_box = Box(Vector3(-100.0f), Vector3(100.1f));
	EXPECT_EQ(box, Box::Merge(box, other_box));
}

TEST(TestBox, MergeInsideBox) {
	Box box(Vector3::Zero(), Vector3(100.0f));
	Box other_box(Vector3::Zero(), Vector3(250.0f));

	EXPECT_EQ(other_box, Box::Merge(box, other_box));
	other_box = Box(Vector3(-100.0f), Vector3(100.1f));
	EXPECT_EQ(other_box, Box::Merge(box, other_box));
}

TEST(TestBox, Size) {
	EXPECT_EQ(Vector3::One(), Box::Unit().Size());
	EXPECT_EQ(Vector3(100.0f), Box(Vector3::Zero(), Vector3(100.0f)).Size());
	EXPECT_EQ(Vector3(50.0f), Box(Vector3(-25.0f), Vector3(25.0f)).Size());
}

TEST(TestBox, Center) {
	EXPECT_EQ(Vector3(0.5f), Box::Unit().Center());
	EXPECT_EQ(Vector3(50.0f), Box(Vector3::Zero(), Vector3(100.0f)).Center());
	EXPECT_EQ(Vector3(0.0f), Box(Vector3(-25.0f), Vector3(25.0f)).Center());
}