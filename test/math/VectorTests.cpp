#include "gtest/gtest.h"
#include "Vector.h"
#include "ZeroMath.h"

using namespace Zero;

TEST(VectorTests, UsefulConstructors) {
	Vector2 v2 = Vector2::Up();
	Vector3 v3 = Vector3::Up();
	Vector4 v4 = Vector4::Up();

	EXPECT_EQ(Vector2(0.0f, 1.0f),             v2);
	EXPECT_EQ(Vector3(0.0f, 1.0f, 0.0f),       v3);
	EXPECT_EQ(Vector4(0.0f, 1.0f, 0.0f, 0.0f), v4);

	v2 = Vector2::Down();
	v3 = Vector3::Down();
	v4 = Vector4::Down();

	EXPECT_EQ(Vector2(0.0f, -1.0f),             v2);
	EXPECT_EQ(Vector3(0.0f, -1.0f, 0.0f),       v3);
	EXPECT_EQ(Vector4(0.0f, -1.0f, 0.0f, 0.0f), v4);

	v2 = Vector2::Right();
	v3 = Vector3::Right();
	v4 = Vector4::Right();

	EXPECT_EQ(Vector2(1.0f, 0.0f),             v2);
	EXPECT_EQ(Vector3(1.0f, 0.0f, 0.0f),       v3);
	EXPECT_EQ(Vector4(1.0f, 0.0f, 0.0f, 0.0f), v4);

	v2 = Vector2::Left();
	v3 = Vector3::Left();
	v4 = Vector4::Left();

	EXPECT_EQ(Vector2(-1.0f, 0.0f),             v2);
	EXPECT_EQ(Vector3(-1.0f, 0.0f, 0.0f),       v3);
	EXPECT_EQ(Vector4(-1.0f, 0.0f, 0.0f, 0.0f), v4);

	v3 = Vector3::Front();
	v4 = Vector4::Front();

	EXPECT_EQ(Vector3(0.0f, 0.0f, 1.0f),       v3);
	EXPECT_EQ(Vector4(0.0f, 0.0f, 1.0f, 0.0f), v4);

	v3 = Vector3::Back();
	v4 = Vector4::Back();

	EXPECT_EQ(Vector3(0.0f, 0.0f, -1.0f),       v3);
	EXPECT_EQ(Vector4(0.0f, 0.0f, -1.0f, 0.0f), v4);
}

TEST(VectorTests, ScalarOperations) {
	Vector4 v = Vector4::One();
	EXPECT_EQ(Vector4(1.0f, 1.0f, 1.0f, 1.0f), v);

	v += 1.0f;
	EXPECT_EQ(Vector4(2.0f, 2.0f, 2.0f, 2.0f), v);
	v -= 0.5f;
	EXPECT_EQ(Vector4(1.5f, 1.5f, 1.5f, 1.5f), v);
	v *= 2.0f;
	EXPECT_EQ(Vector4(3.0f, 3.0f, 3.0f, 3.0f), v);
	v /= 3.0f;
	EXPECT_EQ(Vector4(1.0f, 1.0f, 1.0f, 1.0f), v);

	EXPECT_EQ(Vector4(2.0f, 2.0f, 2.0f, 2.0f), v * 2.0f);
	EXPECT_EQ(Vector4(1.5f, 1.5f, 1.5f, 1.5f), v * 1.5f);
	EXPECT_EQ(Vector4(3.0f, 3.0f, 3.0f, 3.0f), v * 3.0f);
	EXPECT_EQ(Vector4(1.0f, 1.0f, 1.0f, 1.0f), v / 1.0f);
}