#include "gtest/gtest.h"
#include "Vector.h"
#include "ZeroMath.h"

using namespace Zero;

TEST(TestVector, UsefulConstructors) {
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

TEST(TestVector, ScalarOperations) {
	Vector4 v = Vector4::One();
	EXPECT_EQ(Vector4(1.0f), v);


	EXPECT_EQ(Vector4(2.0f), v * 2.0f);
	EXPECT_EQ(Vector4(1.5f), v * 1.5f);
	EXPECT_EQ(Vector4(3.0f), v * 3.0f);
	EXPECT_EQ(Vector4(1.0f), v / 1.0f);

	v += 1.0f;
	EXPECT_EQ(Vector4(2.0f), v);
	v -= 0.5f;
	EXPECT_EQ(Vector4(1.5f), v);
	v *= 2.0f;
	EXPECT_EQ(Vector4(3.0f), v);
	v /= 3.0f;
	EXPECT_EQ(Vector4(1.0f), v);
}

TEST(TestVector, VectorVectorOperations) {
	Vector4 v = Vector4::One();
	Vector4 v2 = Vector4::One();

	EXPECT_EQ(Vector4(2.0f), v + v2);
	EXPECT_EQ(Vector4(0.0f), v - v2);
	EXPECT_EQ(Vector4(3.0f), (v + 1.0f) * (v2 * 1.5f));
	EXPECT_EQ(Vector4(0.5f), v / (v2 + 1.0f));


	v += v2;
	EXPECT_EQ(Vector4(2.0f), v);

	v -= Vector4(1.0f, 2.0f, 3.0f, 4.0f);
	EXPECT_EQ(Vector4(1.0f, 0.0f, -1.0f, -2.0f), v);

	v *= Vector4(1.0f, 2.0f, 3.0f, 4.0f);
	EXPECT_EQ(Vector4(1.0f, 0.0f, -3.0f, -8.0f), v);

	v /= Vector4(-1.0f, -2.0f, -3.0f, -4.0f);
	EXPECT_EQ(Vector4(-1.0f, 0.0f, 1.0f, 2.0f), v);
}

TEST(TestVector, Magnitude) {
	Vector4 simple_vector(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 negative_vector(-1.0f, -2.0f, -3.0f, -4.0f);
	Vector4 zero_vector(0.0f);

	float magnitude = Zero::sqrt(1.0f + 4.0f + 9.0f + 16.0f);
	EXPECT_EQ(magnitude, simple_vector.Magnitude());
	EXPECT_EQ(magnitude, negative_vector.Magnitude());
	EXPECT_EQ(0.0f, zero_vector.Magnitude());
}

TEST(TestVector, SquareMagnitude) {
	Vector4 simple_vector(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 negative_vector(-1.0f, -2.0f, -3.0f, -4.0f);
	Vector4 zero_vector(0.0f);

	float square_magnitude = 1.0f + 4.0f + 9.0f + 16.0f;
	EXPECT_EQ(square_magnitude, simple_vector.SquareMagnitude());
	EXPECT_EQ(square_magnitude, negative_vector.SquareMagnitude());
	EXPECT_EQ(0.0f, zero_vector.SquareMagnitude());
}

TEST(TestVector, Abs) {
	Vector4 simple_vector(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 negative_vector(-1.0f, -2.0f, -3.0f, -4.0f);

	Vector4 positive(1.0f, 2.0f, 3.0f, 4.0f);
	EXPECT_EQ(positive, simple_vector.Abs());
	EXPECT_EQ(positive, negative_vector.Abs());
	EXPECT_EQ(Vector4(0.0f), Vector4::Zero().Abs());
}

TEST(TestVector, Normalize) {
	// TODO: Finish Implementation
	FAIL();
}

TEST(TestVector, Dot) {
	// TODO: Finish Implementation
	FAIL();
}

TEST(TestVector, Cross) {
	// TODO: Finish Implementation
	FAIL();
}

TEST(TestVector, Reflect) {
	// TODO: Finish Implementation
	FAIL();
}

TEST(TestVector, Distance) {
	// TODO: Finish Implementation
	FAIL();
}

TEST(TestVector, SquareDistance) {
	// TODO: Finish Implementation
	FAIL();
}

TEST(TestVector, Lerp) {
	// TODO: Finish Implementation
	FAIL();
}

TEST(TestVector, Angle) {
	// TODO: Finish Implementation
	FAIL();
}