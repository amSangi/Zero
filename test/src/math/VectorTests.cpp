#include "gtest/gtest.h"
#include "math/Vector.hpp"

using namespace zero::math;

TEST(TestVector, UsefulConstructors) {
	EXPECT_EQ(Vec2f::Up(), Vec2f(0.0f, 1.0f));
	EXPECT_EQ(Vec3f::Up(), Vec3f(0.0f, 1.0f, 0.0f));
	EXPECT_EQ(Vec4f::Up(), Vec4f(0.0f, 1.0f, 0.0f, 0.0f));

	EXPECT_EQ(Vec2f::Down(), Vec2f(0.0f, -1.0f));
	EXPECT_EQ(Vec3f::Down(), Vec3f(0.0f, -1.0f, 0.0f));
	EXPECT_EQ(Vec4f::Down(), Vec4f(0.0f, -1.0f, 0.0f, 0.0f));

	EXPECT_EQ(Vec2f::Right(), Vec2f(1.0f, 0.0f));
	EXPECT_EQ(Vec3f::Right(), Vec3f(1.0f, 0.0f, 0.0f));
	EXPECT_EQ(Vec4f::Right(), Vec4f(1.0f, 0.0f, 0.0f, 0.0f));

	EXPECT_EQ(Vec2f::Left(), Vec2f(-1.0f, 0.0f));
	EXPECT_EQ(Vec3f::Left(), Vec3f(-1.0f, 0.0f, 0.0f));
	EXPECT_EQ(Vec4f::Left(), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f));

	EXPECT_EQ(Vec3f::Forward(), Vec3f(0.0f, 0.0f, 1.0f));
	EXPECT_EQ(Vec4f::Forward(), Vec4f(0.0f, 0.0f, 1.0f, 0.0f));

	EXPECT_EQ(Vec3f::Back(), Vec3f(0.0f, 0.0f, -1.0f));
	EXPECT_EQ(Vec4f::Back(), Vec4f(0.0f, 0.0f, -1.0f, 0.0f));
}

TEST(TestVector, ScalarOperations) {
	EXPECT_EQ(Vec4f::One(), Vec4f(1.0f));


	EXPECT_EQ(Vec4f::One() * 2.0f, Vec4f(2.0f));
	EXPECT_EQ(Vec4f::One() * 1.5f, Vec4f(1.5f));
	EXPECT_EQ(Vec4f::One() * 3.0f, Vec4f(3.0f));
	EXPECT_EQ(Vec4f::One() / 1.0f, Vec4f(1.0f));

	Vec4f v = Vec4f::One();
    v += 1.0f;
	EXPECT_EQ(v, Vec4f(2.0f));
	v -= 0.5f;
	EXPECT_EQ(v, Vec4f(1.5f));
	v *= 2.0f;
	EXPECT_EQ(v, Vec4f(3.0f));
	v /= 3.0f;
	EXPECT_EQ(v, Vec4f(1.0f));
}

TEST(TestVector, VectorVectorOperations) {
	Vec4f v = Vec4f::One();
	Vec4f v2 = Vec4f::One();

	EXPECT_EQ(v + v2, Vec4f(2.0f));
	EXPECT_EQ(v - v2, Vec4f(0.0f));
	EXPECT_EQ((v + 1.0f) * (v2 * 1.5f), Vec4f(3.0f));
	EXPECT_EQ(v / (v2 + 1.0f), Vec4f(0.5f));


	v += v2;
	EXPECT_EQ(v, Vec4f(2.0f));

	v -= Vec4f(1.0f, 2.0f, 3.0f, 4.0f);
	EXPECT_EQ(v, Vec4f(1.0f, 0.0f, -1.0f, -2.0f));

	v *= Vec4f(1.0f, 2.0f, 3.0f, 4.0f);
	EXPECT_EQ(v, Vec4f(1.0f, 0.0f, -3.0f, -8.0f));

	v /= Vec4f(-1.0f, -2.0f, -3.0f, -4.0f);
	EXPECT_EQ(v, Vec4f(-1.0f, 0.0f, 1.0f, 2.0f));
}

TEST(TestVector, Magnitude) {
	Vec4f simple_vector(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4f negative_vector(-1.0f, -2.0f, -3.0f, -4.0f);
	Vec4f zero_vector(0.0f);

	float magnitude = Sqrt(1.0f + 4.0f + 9.0f + 16.0f);
	EXPECT_EQ(simple_vector.Magnitude(), magnitude);
	EXPECT_EQ(negative_vector.Magnitude(), magnitude);
	EXPECT_EQ(zero_vector.Magnitude(), 0.0f);
}

TEST(TestVector, SquareMagnitude) {
	Vec4f simple_vector(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4f negative_vector(-1.0f, -2.0f, -3.0f, -4.0f);
	Vec4f zero_vector(0.0f);

	float square_magnitude = 1.0f + 4.0f + 9.0f + 16.0f;
	EXPECT_EQ(simple_vector.SquareMagnitude(), square_magnitude);
	EXPECT_EQ(negative_vector.SquareMagnitude(), square_magnitude);
	EXPECT_EQ(zero_vector.SquareMagnitude(), 0.0f);
}

TEST(TestVector, Abs) {
	Vec4f simple_vector(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4f negative_vector(-1.0f, -2.0f, -3.0f, -4.0f);

	Vec4f positive(1.0f, 2.0f, 3.0f, 4.0f);
	EXPECT_EQ(simple_vector.Abs(), positive);
	EXPECT_EQ(negative_vector.Abs(), positive);
	EXPECT_EQ(Vec4f::Zero().Abs(), Vec4f(0.0f));
}

TEST(TestVector, Normalize) {
	Vec3f expected(1.0f / Sqrt(3.0f));

	// Static Method
	EXPECT_EQ(Vec3f::Normalize(Vec3f::One()), expected);

	// Instance method
	Vec3f actual = Vec3f::One();
	EXPECT_TRUE(actual.Normalize());
	EXPECT_EQ(actual, expected);

	EXPECT_TRUE(Equal(expected.x_, actual.x_));
	EXPECT_TRUE(Equal(expected.y_, actual.y_));
	EXPECT_TRUE(Equal(expected.z_, actual.z_));
}

TEST(TestVector, NormalizeComplex) {
	Vec4f expected(-0.135205f, -0.605484f, 0.764204f, 0.176355f);
	Vec4f actual(-2.3f, -10.3f, 13.0f, 3.0f);

	// Static method
	EXPECT_EQ(Vec4f::Normalize(actual), expected);

	// Instance method
	EXPECT_TRUE(actual.Normalize());
	EXPECT_EQ(actual, expected);
	EXPECT_TRUE(Equal(expected.x_, actual.x_));
	EXPECT_TRUE(Equal(expected.y_, actual.y_));
	EXPECT_TRUE(Equal(expected.z_, actual.z_));
	EXPECT_TRUE(Equal(expected.w_, actual.w_));
}

TEST(TestVector, IsEpsilon) {
	EXPECT_TRUE(Vec3f::Zero().IsEpsilon());
	EXPECT_TRUE(Vec3f(EPSILON).IsEpsilon());

	EXPECT_FALSE(Vec3f::One().IsEpsilon());
	EXPECT_FALSE(Vec3f(0.00001f).IsEpsilon());
	EXPECT_FALSE(Vec3f(-1.0f).IsEpsilon());
}

TEST(TestVector, Dot) {
	EXPECT_EQ(Vec4f::Dot(Vec4f::Zero(), Vec4f::Zero()), 0.0f);
	EXPECT_EQ(Vec4f::Dot(Vec4f::One(), Vec4f::One()), 4.0f);

	float dot = Vec4f::Dot(Vec4f(-2.3f, 43.104f, -253.1345f, 23.0f),
                             Vec4f(1.0f, 323.4f, 234.3f, -34.0f));
	EXPECT_TRUE(Equal(-46153.9f, dot, 1e-01));
}

TEST(TestVector, Cross) {
	Vec3f front = Vec3f::Forward();
	Vec3f back = Vec3f::Back();
	Vec3f right = Vec3f::Right();
	Vec3f left = Vec3f::Left();
	Vec3f up = Vec3f::Up();
	Vec3f down = Vec3f::Down();

	EXPECT_EQ(Vec3f::Cross(right, up), front);
	EXPECT_EQ(Vec3f::Cross(up, right), back);
	EXPECT_EQ(Vec3f::Cross(front, right), up);
	EXPECT_EQ(Vec3f::Cross(right, front), down);
	EXPECT_EQ(Vec3f::Cross(up, front), right);
	EXPECT_EQ(Vec3f::Cross(front, up), left);
}

TEST(TestVector, CrossComplex) {
	Vec3f v1(2.3f, 3.1f, 1.5f);
	Vec3f v2(1.9f, 2.3f, 3.15f);

	EXPECT_EQ(Vec3f::Cross(v1, v2), Vec3f(6.315f, -4.395f, -0.6f));
}

TEST(TestVector, Reflect) {
	Vec3f normal = Vec3f::Up();
	Vec3f incident, reflect;

	incident = Vec3f(1.0f, -1.0f);
	reflect = Vec3f::Reflect(incident, normal);
	EXPECT_EQ(reflect, Vec3f(1.0f, 1.0f));

	incident = -1.0f * normal;
	reflect = Vec3f::Reflect(incident, normal);
	EXPECT_EQ(reflect, normal);

	incident = Vec3f(-1.0f, -1.0f);
	reflect = Vec3f::Reflect(incident, normal);
	EXPECT_EQ(reflect, Vec3f(-1.0f, 1.0f));
}

TEST(TestVector, ReflectComplex) {
	Vec3f normal = Vec3f::Normalize(Vec3f::One());
	Vec3f incident, reflect;

	incident = -1.0f * normal;
	reflect = Vec3f::Reflect(incident, normal);
	EXPECT_EQ(reflect, normal);
}

TEST(TestVector, Distance) {
	EXPECT_EQ(Vec3f::Distance(Vec3f::Up(), Vec3f::Zero()), 1.0f);
	EXPECT_EQ(Vec3f::Distance(Vec3f::Up(), Vec3f::Right()), Sqrt(2.0f));
	EXPECT_EQ(Vec3f::Distance(Vec3f::Up(), Vec3f::Down()), 2.0f);

	float expected = 46.8359f;
	float actual = Vec3f::Distance(Vec3f(2.34f, 19.0f, 15.2f), Vec3f(14.13f, -3.234f, -24.3f));
	EXPECT_TRUE(Equal(expected, actual, 1e-04));
}

TEST(TestVector, SquareDistance) {
	EXPECT_EQ(Vec3f::SquareDistance(Vec3f::Up(), Vec3f::Zero()), 1.0f);
	EXPECT_EQ(Vec3f::SquareDistance(Vec3f::Up(), Vec3f::Right()), 2.0f);
	EXPECT_EQ(Vec3f::SquareDistance(Vec3f::Up(), Vec3f::Down()), 4.0f);

	float expected = 2193.6;
	float actual = Vec3f::SquareDistance(Vec3f(2.34f, 19.0f, 15.2f), Vec3f(14.13f, -3.234f, -24.3f));
	EXPECT_TRUE(Equal(expected, actual, 1e-01));
}

TEST(TestVector, Lerp) {
	Vec3f start(0.0f);
	Vec3f end(0.0f, 10.0f);
	EXPECT_EQ(Vec3f::Lerp(start, end, 0.0f), start);
	EXPECT_EQ(Vec3f::Lerp(start, end, 0.5f), Vec3f(0.0f, 5.0f));
	EXPECT_EQ(Vec3f::Lerp(start, end, 1.0f), end);
}

TEST(TestVector, LerpComplex) {
	Vec3f start(2.3f, 5.0f, -13.3f);
	Vec3f end(32.0f, -23.1f, 304.1f);

	EXPECT_EQ(Vec3f::Lerp(start, end, 0.0f), start);

	Vec3f expected_mid(17.2f, -9.1f, 145.4f);
	Vec3f actual_mid = Vec3f::Lerp(start, end, 0.5f);
	EXPECT_TRUE(Equal(expected_mid.x_, actual_mid.x_, 1e-01));
	EXPECT_TRUE(Equal(expected_mid.y_, actual_mid.y_, 1e-01));
	EXPECT_TRUE(Equal(expected_mid.z_, actual_mid.z_, 1e-01));

	EXPECT_EQ(Vec3f::Lerp(start, end, 1.0f), end);
}

TEST(TestVector, Angle) {
	Radian expected, actual;

	expected = Radian(PI / 2.0f);
	actual = Vec3f::Angle(Vec3f::Right(), Vec3f::Up());
	EXPECT_EQ(actual, expected);

	expected = Radian(PI);
	actual = Vec3f::Angle(Vec3f::Right(), Vec3f::Left());
    EXPECT_EQ(actual, expected);

	expected = Radian(PI / 2.0f);
	actual = Vec3f::Angle(Vec3f::Right(), Vec3f::Down());
    EXPECT_EQ(actual, expected);

	expected = Radian(PI / 2.0f);
	actual = Vec3f::Angle(Vec3f(1.0f, 1.0f), Vec3f(-1.0f, 1.0f));
    EXPECT_EQ(actual, expected);

	expected = Radian(Acos(1.0f / 3.0f));
	actual = Vec3f::Angle(Vec3f(1.0f, 1.0f, 1.0f), Vec3f(1.0f, 1.0f, -1.0f));
    EXPECT_EQ(actual, expected);

	expected = Radian(0.0f);
	actual = Vec3f::Angle(Vec3f(5.0f, 13.3f, -13.32f), Vec3f(5.0f, 13.3f, -13.32f));
    EXPECT_EQ(actual, expected);
}