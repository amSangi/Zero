#include "gtest/gtest.h"
#include "math/Angle.hpp"
#include "math/Quaternion.hpp"

using namespace zero::math;

TEST(TestQuaternion, ScalarOperations) {
	Quaternion quat = Quaternion::Identity();

	EXPECT_EQ(quat + 1.0f, Quaternion(2.0f, 1.0f, 1.0f, 1.0f));
	EXPECT_EQ(quat - 1.0f, Quaternion(0.0f, -1.0f, -1.0f, -1.0f));
	EXPECT_EQ((quat + 1.0f) * 2.5f, Quaternion(5.0f, 2.5f, 2.5f, 2.5f));
	EXPECT_EQ((quat + 1.0f) / 2.0f, Quaternion(1.0f, 0.5f, 0.5f, 0.5f));

	quat += 1.0f;
	EXPECT_EQ(quat, Quaternion(2.0f, 1.0f, 1.0f, 1.0f));
	quat -= 2.0f;
	EXPECT_EQ(quat, Quaternion(0.0f, -1.0f, -1.0f, -1.0f));
	quat *= 2.5f;
	EXPECT_EQ(quat, Quaternion(0.0f, -2.5f, -2.5f, -2.5f));
	quat /= 2.5f;
	EXPECT_EQ(quat, Quaternion(0.0f, -1.0f, -1.0f, -1.0f));
}

TEST(TestQuaternion, QuaternionQuaternionOperations) {
	Quaternion quat1 = Quaternion::Identity();
	Quaternion quat2(2.5f, 3.5f, 1.0f, 5.0f);

	EXPECT_EQ(quat1 + quat2, Quaternion(3.5f, 3.5f, 1.0f, 5.0f));
	EXPECT_EQ(quat1 - quat2, Quaternion(-1.5f, -3.5f, -1.0f, -5.0f));

    EXPECT_EQ(quat1 * quat2, quat2 *quat1);
	EXPECT_EQ(quat1 * quat2, Quaternion(2.5f, 3.5f, 1.0f, 5.0f));

	Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion q2(5.0f, 6.0f, 7.0f, 8.0f);
	EXPECT_NE(q1 * q2, q2 * q1);
	EXPECT_EQ(q1 * q2, Quaternion(-60.0f, 12.0f, 30.0f, 24.0f));
	EXPECT_EQ(q2 * q1, Quaternion(-60.0f, 20.0f, 14.0f, 32.0f));



	quat1 += quat2;
	EXPECT_EQ(quat1, Quaternion(3.5f, 3.5f, 1.0f, 5.0f));

	quat1 -= (quat2 * 2.0f);
	EXPECT_EQ(quat1, Quaternion(-1.5f, -3.5f, -1.0f, -5.0f));

	quat1 = Quaternion::Identity();
	quat1 *= quat2;
	EXPECT_EQ(quat1, quat2);


	Quaternion temp = q1;
	q1 *= q2;
	q2 *= temp;
	EXPECT_NE(q1, q2);
	EXPECT_EQ(q1, Quaternion(-60.0f, 12.0f, 30.0f, 24.0f));
	EXPECT_EQ(q2, Quaternion(-60.0f, 20.0f, 14.0f, 32.0f));
}

TEST(TestQuaternion, QuaternionVectorOperations) {
	Quaternion identity = Quaternion::Identity();
	Vec3f expected, actual;

	Vec3f vector(1.0f, 2.0f, 3.0f);
	expected = vector;
	actual = identity * vector;
    EXPECT_EQ(actual, expected);


	expected = Vec3f(25.0f, 2.0f, -9.0f);
	actual = Quaternion(1.0f, 2.0f, 3.0f, 4.0f) * Vec3f(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(actual, expected);

	expected = Vec3f(261.0f, -1514.0f, -419.0f);
	actual = Quaternion(13.0f, 11.0f, 3.0f, -4.0f) * vector;
    EXPECT_EQ(actual, expected);

	expected = Vec3f(-1078029.0f, 6407.0f, -2368179.0f);
	actual = Quaternion(11.0f, -3.0f, 234.0f, 2.0f) * Vec3f(11.0f, -13.0f, 21.0f);
    EXPECT_EQ(actual, expected);

}

TEST(TestQuaternion, Norm) {
	EXPECT_EQ(Quaternion::Zero().Norm(), 0.0f);
	EXPECT_EQ(Quaternion::Identity().Norm(), 1.0f);
	EXPECT_EQ(Quaternion(1.0f, 23.0f, 44.0f, 33.0f).Norm(), 3.0f * Sqrt(395.0f));
	EXPECT_EQ(Quaternion(1.0f, -3.0f, 44.0f, -33.0f).Norm(), Sqrt(3035.0f));
}

TEST(TestQuaternion, UnitNorm) {
	Quaternion expected, actual, quat;

	EXPECT_EQ(Quaternion::Identity().Unit(), Quaternion::Identity());

	quat = Quaternion(0.0f, 1.0f, 1.0f, 1.0f);
	expected = quat / Sqrt(3.0f);
	actual = quat.Unit();
    EXPECT_EQ(actual, expected);

	quat = Quaternion(25.0f, -13.0f, -3.0f, 231.0f);
	expected = quat / (2.0f * Sqrt(13541.0f));
	actual = quat.Unit();
    EXPECT_EQ(actual, expected);
}

TEST(TestQuaternion, Conjugate) {
	EXPECT_EQ(Quaternion::Identity().Conjugate(), Quaternion::Identity());
	EXPECT_EQ(Quaternion(1.0f, 3.0f, -4.5f, -13.3f).Conjugate(), Quaternion(1.0f, -3.0f, 4.5f, 13.3f));
}

TEST(TestQuaternion, Inverse) {
	Quaternion expected, actual, quat;

	quat = Quaternion::Identity();
	expected = quat;
	actual = quat.Inverse();
    EXPECT_EQ(actual, expected);

	quat = Quaternion(25.0f, -13.0f, -3.0f, 231.0f);
	expected = Quaternion(25.0f, 13.0f, 3.0f, -231.0f) / (2.0f * Sqrt(13541.0f));
	actual = quat.Inverse();
    EXPECT_EQ(actual, expected);

	quat = Quaternion(35.0f, 10.0f, 11.0f, 12.0f);
	expected = Quaternion(35.0f, -10.0f, -11.0f, -12.0f) / Sqrt(1590.0f);
	actual = quat.Inverse();
	EXPECT_EQ(actual, expected);

}

TEST(TestQuaternion, Negate) {
	EXPECT_EQ(Quaternion::Identity().Negate(), Quaternion::Identity() * -1.0f);
	EXPECT_EQ(Quaternion(-1.0f, 3.0f, 4.5f, -5.0f).Negate(), Quaternion(1.0f, -3.0f, -4.5f, 5.0f));
}

TEST(TestQuaternion, Dot) {
	EXPECT_EQ(Quaternion::Dot(Quaternion::Identity(), Quaternion::Identity()), 1.0f);
	float dot = Quaternion::Dot(Quaternion(1.0f, 3.0f, 23.0f, 113.0f), Quaternion(32.0f, 13.0f, 2.0f, 3.0f));
	EXPECT_EQ(dot, 456.0f);
}

TEST(TestQuaternion, FromAngleAxis) {
	Quaternion expected, actual;

	// Round to nearest tenth and check
	auto CHECK_FLT_EQ = [] (Quaternion a, Quaternion e) {
		EXPECT_EQ(Floor(a.w_ * 10.0f + 0.50f) / 10.0f, e.w_);
		EXPECT_EQ(Floor(a.x_ * 10.0f + 0.50f) / 10.0f, e.x_);
		EXPECT_EQ(Floor(a.y_ * 10.0f + 0.50f) / 10.0f, e.y_);
		EXPECT_EQ(Floor(a.z_ * 10.0f + 0.50f) / 10.0f, e.z_);
	};

	expected = Quaternion(0.9f, 0.4f, 0.0f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vec3f::Right(), Degree(45.0f));
	CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(0.7f, -0.7f, 0.0f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vec3f::Left(), Degree(95.0f));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(1.0f, 0.0f, 0.1f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vec3f::Up(), Degree(11.3f));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(1.0f, 0.0f, -0.2f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vec3f::Down(), Degree(23.3f));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(-1.0f, 0.0f, 0.0f, 0.2f);
	actual = Quaternion::FromAngleAxis(Vec3f::Back(), Degree(385.2f));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(0.7f, 0.0f, 0.0f, -0.7f);
	actual = Quaternion::FromAngleAxis(Vec3f::Forward(), Degree(-96.3f));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(-1.0f, -0.1f, -0.1f, -0.1f);
	actual = Quaternion::FromAngleAxis(Vec3f::One(), Degree(-334.1f));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vec3f::Zero(), Degree(-0.1f));
    CHECK_FLT_EQ(actual, expected);
}

TEST(TestQuaternion, LookRotation) {
	EXPECT_EQ(Quaternion::LookRotation(Vec3f::Right()).GetEulerAngles(),
              Vec3f(0.0f, 90.0f * DEGREE_TO_RADIAN, 0.0f));
}

TEST(TestQuaternion, FromToRotation) {
	EXPECT_EQ(Quaternion::FromToRotation(Vec3f::Right(), Vec3f::Up()).GetEulerAngles(),
              Vec3f(0.0f, 90.0f * DEGREE_TO_RADIAN, 0.0f));
}