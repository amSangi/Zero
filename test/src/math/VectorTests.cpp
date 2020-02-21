#include <gtest/gtest.h>
#include "math/Vector.hpp"

using namespace zero::math;

TEST(TestVector, UsefulConstructors) {
	EXPECT_EQ(Vec2f::Up(), Vec2f(0.0F, 1.0F));
	EXPECT_EQ(Vec3f::Up(), Vec3f(0.0F, 1.0F, 0.0F));
	EXPECT_EQ(Vec4f::Up(), Vec4f(0.0F, 1.0F, 0.0F, 0.0F));

	EXPECT_EQ(Vec2f::Down(), Vec2f(0.0F, -1.0F));
	EXPECT_EQ(Vec3f::Down(), Vec3f(0.0F, -1.0F, 0.0F));
	EXPECT_EQ(Vec4f::Down(), Vec4f(0.0F, -1.0F, 0.0F, 0.0F));

	EXPECT_EQ(Vec2f::Right(), Vec2f(1.0F, 0.0F));
	EXPECT_EQ(Vec3f::Right(), Vec3f(1.0F, 0.0F, 0.0F));
	EXPECT_EQ(Vec4f::Right(), Vec4f(1.0F, 0.0F, 0.0F, 0.0F));

	EXPECT_EQ(Vec2f::Left(), Vec2f(-1.0F, 0.0F));
	EXPECT_EQ(Vec3f::Left(), Vec3f(-1.0F, 0.0F, 0.0F));
	EXPECT_EQ(Vec4f::Left(), Vec4f(-1.0F, 0.0F, 0.0F, 0.0F));

	EXPECT_EQ(Vec3f::Forward(), Vec3f(0.0F, 0.0F, 1.0F));
	EXPECT_EQ(Vec4f::Forward(), Vec4f(0.0F, 0.0F, 1.0F, 0.0F));

	EXPECT_EQ(Vec3f::Back(), Vec3f(0.0F, 0.0F, -1.0F));
	EXPECT_EQ(Vec4f::Back(), Vec4f(0.0F, 0.0F, -1.0F, 0.0F));
}

TEST(TestVector, ScalarOperations) {
	EXPECT_EQ(Vec4f::One(), Vec4f(1.0F));


	EXPECT_EQ(Vec4f::One() * 2.0F, Vec4f(2.0F));
	EXPECT_EQ(Vec4f::One() * 1.5F, Vec4f(1.5F));
	EXPECT_EQ(Vec4f::One() * 3.0F, Vec4f(3.0F));
	EXPECT_EQ(Vec4f::One() / 1.0F, Vec4f(1.0F));

	Vec4f v = Vec4f::One();
    v += 1.0F;
	EXPECT_EQ(v, Vec4f(2.0F));
	v -= 0.5F;
	EXPECT_EQ(v, Vec4f(1.5F));
	v *= 2.0F;
	EXPECT_EQ(v, Vec4f(3.0F));
	v /= 3.0F;
	EXPECT_EQ(v, Vec4f(1.0F));
}

TEST(TestVector, VectorVectorOperations) {
	Vec4f v = Vec4f::One();
	Vec4f v2 = Vec4f::One();

	EXPECT_EQ(v + v2, Vec4f(2.0F));
	EXPECT_EQ(v - v2, Vec4f(0.0F));
	EXPECT_EQ((v + 1.0F) * (v2 * 1.5F), Vec4f(3.0F));
	EXPECT_EQ(v / (v2 + 1.0F), Vec4f(0.5F));


	v += v2;
	EXPECT_EQ(v, Vec4f(2.0F));

	v -= Vec4f(1.0F, 2.0F, 3.0F, 4.0F);
	EXPECT_EQ(v, Vec4f(1.0F, 0.0F, -1.0F, -2.0F));

	v *= Vec4f(1.0F, 2.0F, 3.0F, 4.0F);
	EXPECT_EQ(v, Vec4f(1.0F, 0.0F, -3.0F, -8.0F));

	v /= Vec4f(-1.0F, -2.0F, -3.0F, -4.0F);
	EXPECT_EQ(v, Vec4f(-1.0F, 0.0F, 1.0F, 2.0F));
}

TEST(TestVector, Magnitude) {
	Vec4f simple_vector(1.0F, 2.0F, 3.0F, 4.0F);
	Vec4f negative_vector(-1.0F, -2.0F, -3.0F, -4.0F);
	Vec4f zero_vector(0.0F);

	float magnitude = Sqrt(1.0F + 4.0F + 9.0F + 16.0F);
	EXPECT_EQ(simple_vector.Magnitude(), magnitude);
	EXPECT_EQ(negative_vector.Magnitude(), magnitude);
	EXPECT_EQ(zero_vector.Magnitude(), 0.0F);
}

TEST(TestVector, SquareMagnitude) {
	Vec4f simple_vector(1.0F, 2.0F, 3.0F, 4.0F);
	Vec4f negative_vector(-1.0F, -2.0F, -3.0F, -4.0F);
	Vec4f zero_vector(0.0F);

	float square_magnitude = 1.0F + 4.0F + 9.0F + 16.0F;
	EXPECT_EQ(simple_vector.SquareMagnitude(), square_magnitude);
	EXPECT_EQ(negative_vector.SquareMagnitude(), square_magnitude);
	EXPECT_EQ(zero_vector.SquareMagnitude(), 0.0F);
}

TEST(TestVector, Abs) {
	Vec4f simple_vector(1.0F, 2.0F, 3.0F, 4.0F);
	Vec4f negative_vector(-1.0F, -2.0F, -3.0F, -4.0F);

	Vec4f positive(1.0F, 2.0F, 3.0F, 4.0F);
	EXPECT_EQ(simple_vector.Abs(), positive);
	EXPECT_EQ(negative_vector.Abs(), positive);
	EXPECT_EQ(Vec4f::Zero().Abs(), Vec4f(0.0F));
}

TEST(TestVector, Normalize) {
	Vec3f expected(1.0F / Sqrt(3.0F));

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
	Vec4f expected(-0.135205F, -0.605484F, 0.764204F, 0.176355F);
	Vec4f actual(-2.3F, -10.3F, 13.0F, 3.0F);

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
	EXPECT_FALSE(Vec3f(0.00001F).IsEpsilon());
	EXPECT_FALSE(Vec3f(-1.0F).IsEpsilon());
}

TEST(TestVector, Dot) {
	EXPECT_EQ(Vec4f::Dot(Vec4f::Zero(), Vec4f::Zero()), 0.0F);
	EXPECT_EQ(Vec4f::Dot(Vec4f::One(), Vec4f::One()), 4.0F);

	float dot = Vec4f::Dot(Vec4f(-2.3F, 43.104F, -253.1345F, 23.0F),
                             Vec4f(1.0F, 323.4F, 234.3F, -34.0F));
	EXPECT_TRUE(Equal(-46153.9F, dot, 1e-01));
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
	Vec3f v1(2.3F, 3.1F, 1.5F);
	Vec3f v2(1.9F, 2.3F, 3.15F);

	EXPECT_EQ(Vec3f::Cross(v1, v2), Vec3f(6.315F, -4.395F, -0.6F));
}

TEST(TestVector, Reflect) {
	Vec3f normal = Vec3f::Up();
	Vec3f incident, reflect;

	incident = Vec3f(1.0F, -1.0F);
	reflect = Vec3f::Reflect(incident, normal);
	EXPECT_EQ(reflect, Vec3f(1.0F, 1.0F));

	incident = -1.0F * normal;
	reflect = Vec3f::Reflect(incident, normal);
	EXPECT_EQ(reflect, normal);

	incident = Vec3f(-1.0F, -1.0F);
	reflect = Vec3f::Reflect(incident, normal);
	EXPECT_EQ(reflect, Vec3f(-1.0F, 1.0F));
}

TEST(TestVector, ReflectComplex) {
	Vec3f normal = Vec3f::Normalize(Vec3f::One());
	Vec3f incident, reflect;

	incident = -1.0F * normal;
	reflect = Vec3f::Reflect(incident, normal);
	EXPECT_EQ(reflect, normal);
}

TEST(TestVector, Distance) {
	EXPECT_EQ(Vec3f::Distance(Vec3f::Up(), Vec3f::Zero()), 1.0F);
	EXPECT_EQ(Vec3f::Distance(Vec3f::Up(), Vec3f::Right()), Sqrt(2.0F));
	EXPECT_EQ(Vec3f::Distance(Vec3f::Up(), Vec3f::Down()), 2.0F);

	float expected = 46.8359F;
	float actual = Vec3f::Distance(Vec3f(2.34F, 19.0F, 15.2F), Vec3f(14.13F, -3.234F, -24.3F));
	EXPECT_TRUE(Equal(expected, actual, 1e-04));
}

TEST(TestVector, SquareDistance) {
	EXPECT_EQ(Vec3f::SquareDistance(Vec3f::Up(), Vec3f::Zero()), 1.0F);
	EXPECT_EQ(Vec3f::SquareDistance(Vec3f::Up(), Vec3f::Right()), 2.0F);
	EXPECT_EQ(Vec3f::SquareDistance(Vec3f::Up(), Vec3f::Down()), 4.0F);

	float expected = 2193.6;
	float actual = Vec3f::SquareDistance(Vec3f(2.34F, 19.0F, 15.2F), Vec3f(14.13F, -3.234F, -24.3F));
	EXPECT_TRUE(Equal(expected, actual, 1e-01));
}

TEST(TestVector, Lerp) {
	Vec3f start(0.0F);
	Vec3f end(0.0F, 10.0F);
	EXPECT_EQ(Vec3f::Lerp(start, end, 0.0F), start);
	EXPECT_EQ(Vec3f::Lerp(start, end, 0.5F), Vec3f(0.0F, 5.0F));
	EXPECT_EQ(Vec3f::Lerp(start, end, 1.0F), end);
}

TEST(TestVector, LerpComplex) {
	Vec3f start(2.3F, 5.0F, -13.3F);
	Vec3f end(32.0F, -23.1F, 304.1F);

	EXPECT_EQ(Vec3f::Lerp(start, end, 0.0F), start);

	Vec3f expected_mid(17.2F, -9.1F, 145.4F);
	Vec3f actual_mid = Vec3f::Lerp(start, end, 0.5F);
	EXPECT_TRUE(Equal(expected_mid.x_, actual_mid.x_, 1e-01));
	EXPECT_TRUE(Equal(expected_mid.y_, actual_mid.y_, 1e-01));
	EXPECT_TRUE(Equal(expected_mid.z_, actual_mid.z_, 1e-01));

	EXPECT_EQ(Vec3f::Lerp(start, end, 1.0F), end);
}

TEST(TestVector, Angle) {
	Radian expected, actual;

	expected = Radian(kPi / 2.0F);
	actual = Vec3f::Angle(Vec3f::Right(), Vec3f::Up());
	EXPECT_EQ(actual, expected);

	expected = Radian(kPi);
	actual = Vec3f::Angle(Vec3f::Right(), Vec3f::Left());
    EXPECT_EQ(actual, expected);

	expected = Radian(kPi / 2.0F);
	actual = Vec3f::Angle(Vec3f::Right(), Vec3f::Down());
    EXPECT_EQ(actual, expected);

	expected = Radian(kPi / 2.0F);
	actual = Vec3f::Angle(Vec3f(1.0F, 1.0F), Vec3f(-1.0F, 1.0F));
    EXPECT_EQ(actual, expected);

	expected = Radian(Acos(1.0F / 3.0F));
	actual = Vec3f::Angle(Vec3f(1.0F, 1.0F, 1.0F), Vec3f(1.0F, 1.0F, -1.0F));
    EXPECT_EQ(actual, expected);

	expected = Radian(0.0F);
	actual = Vec3f::Angle(Vec3f(5.0F, 13.3F, -13.32F), Vec3f(5.0F, 13.3F, -13.32F));
    EXPECT_EQ(actual, expected);
}

TEST(TestVector, XY) {
    Vector<1, float> single_dimension(1.0F);
    Vec2f two_dimension(1.0F, 2.0F);
    Vec3f three_dimension(1.0F, 2.0F, 3.0F);
    Vec4f four_dimension(1.0F, 2.0F, 3.0F, 4.0F);

    EXPECT_EQ(single_dimension.XY(), Vec2f(1.0F, 0.0F));
    EXPECT_EQ(two_dimension.XY(), two_dimension);
    EXPECT_EQ(three_dimension.XY(), Vec2f(1.0F, 2.0F));
    EXPECT_EQ(four_dimension.XY(), Vec2f(1.0F, 2.0F));
}

TEST(TestVector, XYZ) {
    Vector<1, float> single_dimension(1.0F);
    Vec2f two_dimension(1.0F, 2.0F);
    Vec3f three_dimension(1.0F, 2.0F, 3.0F);
    Vec4f four_dimension(1.0F, 2.0F, 3.0F, 4.0F);

    EXPECT_EQ(single_dimension.XYZ(), Vec3f(1.0F, 0.0F, 0.0F));
    EXPECT_EQ(two_dimension.XYZ(), Vec3f(1.0F, 2.0F, 0.0F));
    EXPECT_EQ(three_dimension.XYZ(), three_dimension);
    EXPECT_EQ(four_dimension.XYZ(), Vec3f(1.0F, 2.0F, 3.0F));
}