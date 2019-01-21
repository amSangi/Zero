#include "gtest/gtest.h"
#include "Vector.hpp"

using namespace zero;

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

	v3 = Vector3::Forward();
	v4 = Vector4::Forward();

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

	float magnitude = zero::Sqrt(1.0f + 4.0f + 9.0f + 16.0f);
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
	Vector3 expected(1.0f / zero::Sqrt(3.0f));

	// Static Method
	EXPECT_EQ(expected, Vector3::Normalize(Vector3::One()));

	// Instance method
	Vector3 actual = Vector3::One();
	EXPECT_TRUE(actual.Normalize());
	EXPECT_EQ(expected, actual);

	EXPECT_TRUE(zero::Equal(expected.x_, actual.x_));
	EXPECT_TRUE(zero::Equal(expected.y_, actual.y_));
	EXPECT_TRUE(zero::Equal(expected.z_, actual.z_));
}

TEST(TestVector, NormalizeComplex) {
	Vector4 expected(-0.135205f, -0.605484f, 0.764204f, 0.176355f);
	Vector4 actual(-2.3f, -10.3f, 13.0f, 3.0f);

	// Static method
	EXPECT_EQ(expected, Vector4::Normalize(actual));

	// Instance method
	EXPECT_TRUE(actual.Normalize());
	EXPECT_EQ(expected, actual);
	EXPECT_TRUE(zero::Equal(expected.x_, actual.x_));
	EXPECT_TRUE(zero::Equal(expected.y_, actual.y_));
	EXPECT_TRUE(zero::Equal(expected.z_, actual.z_));
	EXPECT_TRUE(zero::Equal(expected.w_, actual.w_));
}

TEST(TestVector, IsEpsilon) {
	EXPECT_TRUE(Vector3::Zero().IsEpsilon());
	EXPECT_TRUE(Vector3(EPSILON).IsEpsilon());

	EXPECT_FALSE(Vector3::One().IsEpsilon());
	EXPECT_FALSE(Vector3(0.00001f).IsEpsilon());
	EXPECT_FALSE(Vector3(-1.0f).IsEpsilon());
}

TEST(TestVector, Dot) {
	EXPECT_EQ(0.0f, Vector4::Dot(Vector4::Zero(), Vector4::Zero()));
	EXPECT_EQ(4.0f, Vector4::Dot(Vector4::One(), Vector4::One()));

	float dot = Vector4::Dot(Vector4(-2.3f, 43.104f, -253.1345f, 23.0f),
                             Vector4(1.0f, 323.4f, 234.3f, -34.0f));
	EXPECT_TRUE(zero::Equal(-46153.9f, dot, 1e-01));
}

TEST(TestVector, Cross) {
	Vector3 front = Vector3::Forward();
	Vector3 back = Vector3::Back();
	Vector3 right = Vector3::Right();
	Vector3 left = Vector3::Left();
	Vector3 up = Vector3::Up();
	Vector3 down = Vector3::Down();

	EXPECT_EQ(front, Vector3::Cross(right, up));
	EXPECT_EQ(back, Vector3::Cross(up, right));
	EXPECT_EQ(up, Vector3::Cross(front, right));
	EXPECT_EQ(down, Vector3::Cross(right, front));
	EXPECT_EQ(right, Vector3::Cross(up, front));
	EXPECT_EQ(left, Vector3::Cross(front, up));
}

TEST(TestVector, CrossComplex) {
	Vector3 v1(2.3f, 3.1f, 1.5f);
	Vector3 v2(1.9f, 2.3f, 3.15f);

	EXPECT_EQ(Vector3(6.315f, -4.395f, -0.6f), Vector3::Cross(v1, v2));
}

TEST(TestVector, Reflect) {
	Vector3 normal = Vector3::Up();
	Vector3 incident, reflect;

	incident = Vector3(1.0f, -1.0f);
	reflect = Vector3::Reflect(incident, normal);
	EXPECT_EQ(Vector3(1.0f, 1.0f), reflect);

	incident = -1.0f * normal;
	reflect = Vector3::Reflect(incident, normal);
	EXPECT_EQ(normal, reflect);

	incident = Vector3(-1.0f, -1.0f);
	reflect = Vector3::Reflect(incident, normal);
	EXPECT_EQ(Vector3(-1.0f, 1.0f), reflect);
}

TEST(TestVector, ReflectComplex) {
	Vector3 normal = Vector3::Normalize(Vector3::One());
	Vector3 incident, reflect;

	incident = -1.0f * normal;
	reflect = Vector3::Reflect(incident, normal);
	EXPECT_EQ(normal, reflect);
}

TEST(TestVector, Distance) {
	EXPECT_EQ(1.0f, Vector3::Distance(Vector3::Up(), Vector3::Zero()));
	EXPECT_EQ(zero::Sqrt(2.0f), Vector3::Distance(Vector3::Up(), Vector3::Right()));
	EXPECT_EQ(2.0f, Vector3::Distance(Vector3::Up(), Vector3::Down()));

	float expected = 46.8359f;
	float actual = Vector3::Distance(Vector3(2.34f, 19.0f, 15.2f), Vector3(14.13f, -3.234f, -24.3f));
	EXPECT_TRUE(zero::Equal(expected, actual, 1e-04));
}

TEST(TestVector, SquareDistance) {
	EXPECT_EQ(1.0f, Vector3::SquareDistance(Vector3::Up(), Vector3::Zero()));
	EXPECT_EQ(2.0f, Vector3::SquareDistance(Vector3::Up(), Vector3::Right()));
	EXPECT_EQ(4.0f, Vector3::SquareDistance(Vector3::Up(), Vector3::Down()));

	float expected = 2193.6;
	float actual = Vector3::SquareDistance(Vector3(2.34f, 19.0f, 15.2f), Vector3(14.13f, -3.234f, -24.3f));
	EXPECT_TRUE(zero::Equal(expected, actual, 1e-01));
}

TEST(TestVector, Lerp) {
	Vector3 start(0.0f);
	Vector3 end(0.0f, 10.0f);
	EXPECT_EQ(start, Vector3::Lerp(start, end, 0.0f));
	EXPECT_EQ(Vector3(0.0f, 5.0f), Vector3::Lerp(start, end, 0.5f));
	EXPECT_EQ(end, Vector3::Lerp(start, end, 1.0f));
}

TEST(TestVector, LerpComplex) {
	Vector3 start(2.3f, 5.0f, -13.3f);
	Vector3 end(32.0f, -23.1f, 304.1f);

	EXPECT_EQ(start, Vector3::Lerp(start, end, 0.0f));

	Vector3 expected_mid(17.2f, -9.1f, 145.4f);
	Vector3 actual_mid = Vector3::Lerp(start, end, 0.5f);
	EXPECT_TRUE(zero::Equal(expected_mid.x_, actual_mid.x_, 1e-01));
	EXPECT_TRUE(zero::Equal(expected_mid.y_, actual_mid.y_, 1e-01));
	EXPECT_TRUE(zero::Equal(expected_mid.z_, actual_mid.z_, 1e-01));

	EXPECT_EQ(end, Vector3::Lerp(start, end, 1.0f));
}

TEST(TestVector, Angle) {
	Radian expected, actual;

	expected = Radian(PI / 2.0f);
	actual = Vector3::Angle(Vector3::Right(), Vector3::Up());
	EXPECT_EQ(expected, actual);

	expected = Radian(PI);
	actual = Vector3::Angle(Vector3::Right(), Vector3::Left());
	EXPECT_EQ(expected, actual);

	expected = Radian(PI / 2.0f);
	actual = Vector3::Angle(Vector3::Right(), Vector3::Down());
	EXPECT_EQ(expected, actual);

	expected = Radian(PI / 2.0f);
	actual = Vector3::Angle(Vector3(1.0f, 1.0f), Vector3(-1.0f, 1.0f));
	EXPECT_EQ(expected, actual);

	expected = Radian(zero::Acos(1.0f / 3.0f));
	actual = Vector3::Angle(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, -1.0f));
	EXPECT_EQ(expected, actual);

	expected = Radian(0.0f);
	actual = Vector3::Angle(Vector3(5.0f, 13.3f, -13.32f), Vector3(5.0f, 13.3f, -13.32f));
	EXPECT_EQ(expected, actual);
}