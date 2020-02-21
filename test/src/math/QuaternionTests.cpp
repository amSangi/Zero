#include <gtest/gtest.h>
#include "math/Angle.hpp"
#include "math/Quaternion.hpp"

using namespace zero::math;

TEST(TestQuaternion, ScalarOperations) {
	Quaternion quat = Quaternion::Identity();

	EXPECT_EQ(quat + 1.0F, Quaternion(2.0F, 1.0F, 1.0F, 1.0F));
	EXPECT_EQ(quat - 1.0F, Quaternion(0.0F, -1.0F, -1.0F, -1.0F));
	EXPECT_EQ((quat + 1.0F) * 2.5F, Quaternion(5.0F, 2.5F, 2.5F, 2.5F));
	EXPECT_EQ((quat + 1.0F) / 2.0F, Quaternion(1.0F, 0.5F, 0.5F, 0.5F));

	quat += 1.0F;
	EXPECT_EQ(quat, Quaternion(2.0F, 1.0F, 1.0F, 1.0F));
	quat -= 2.0F;
	EXPECT_EQ(quat, Quaternion(0.0F, -1.0F, -1.0F, -1.0F));
	quat *= 2.5F;
	EXPECT_EQ(quat, Quaternion(0.0F, -2.5F, -2.5F, -2.5F));
	quat /= 2.5F;
	EXPECT_EQ(quat, Quaternion(0.0F, -1.0F, -1.0F, -1.0F));
}

TEST(TestQuaternion, QuaternionQuaternionOperations) {
	Quaternion quat1 = Quaternion::Identity();
	Quaternion quat2(2.5F, 3.5F, 1.0F, 5.0F);

	EXPECT_EQ(quat1 + quat2, Quaternion(3.5F, 3.5F, 1.0F, 5.0F));
	EXPECT_EQ(quat1 - quat2, Quaternion(-1.5F, -3.5F, -1.0F, -5.0F));

    EXPECT_EQ(quat1 * quat2, quat2 *quat1);
	EXPECT_EQ(quat1 * quat2, Quaternion(2.5F, 3.5F, 1.0F, 5.0F));

	Quaternion q1(1.0F, 2.0F, 3.0F, 4.0F);
	Quaternion q2(5.0F, 6.0F, 7.0F, 8.0F);
	EXPECT_NE(q1 * q2, q2 * q1);
	EXPECT_EQ(q1 * q2, Quaternion(-60.0F, 12.0F, 30.0F, 24.0F));
	EXPECT_EQ(q2 * q1, Quaternion(-60.0F, 20.0F, 14.0F, 32.0F));



	quat1 += quat2;
	EXPECT_EQ(quat1, Quaternion(3.5F, 3.5F, 1.0F, 5.0F));

	quat1 -= (quat2 * 2.0F);
	EXPECT_EQ(quat1, Quaternion(-1.5F, -3.5F, -1.0F, -5.0F));

	quat1 = Quaternion::Identity();
	quat1 *= quat2;
	EXPECT_EQ(quat1, quat2);


	Quaternion temp = q1;
	q1 *= q2;
	q2 *= temp;
	EXPECT_NE(q1, q2);
	EXPECT_EQ(q1, Quaternion(-60.0F, 12.0F, 30.0F, 24.0F));
	EXPECT_EQ(q2, Quaternion(-60.0F, 20.0F, 14.0F, 32.0F));
}

TEST(TestQuaternion, QuaternionVectorOperations) {
	Quaternion identity = Quaternion::Identity();
	Vec3f expected, actual;

	Vec3f vector(1.0F, 2.0F, 3.0F);
	expected = vector;
	actual = identity * vector;
    EXPECT_EQ(actual, expected);


	expected = Vec3f(25.0F, 2.0F, -9.0F);
	actual = Quaternion(1.0F, 2.0F, 3.0F, 4.0F) * Vec3f(1.0F, 2.0F, 3.0F);
    EXPECT_EQ(actual, expected);

	expected = Vec3f(261.0F, -1514.0F, -419.0F);
	actual = Quaternion(13.0F, 11.0F, 3.0F, -4.0F) * vector;
    EXPECT_EQ(actual, expected);

	expected = Vec3f(-1078029.0F, 6407.0F, -2368179.0F);
	actual = Quaternion(11.0F, -3.0F, 234.0F, 2.0F) * Vec3f(11.0F, -13.0F, 21.0F);
    EXPECT_EQ(actual, expected);

}

TEST(TestQuaternion, Norm) {
	EXPECT_EQ(Quaternion::Zero().Norm(), 0.0F);
	EXPECT_EQ(Quaternion::Identity().Norm(), 1.0F);
	EXPECT_EQ(Quaternion(1.0F, 23.0F, 44.0F, 33.0F).Norm(), 3.0F * Sqrt(395.0F));
	EXPECT_EQ(Quaternion(1.0F, -3.0F, 44.0F, -33.0F).Norm(), Sqrt(3035.0F));
}

TEST(TestQuaternion, UnitNorm) {
	Quaternion expected, actual, quat;

	EXPECT_EQ(Quaternion::Identity().Unit(), Quaternion::Identity());

	quat = Quaternion(0.0F, 1.0F, 1.0F, 1.0F);
	expected = quat / Sqrt(3.0F);
	actual = quat.Unit();
    EXPECT_EQ(actual, expected);

	quat = Quaternion(25.0F, -13.0F, -3.0F, 231.0F);
	expected = quat / (2.0F * Sqrt(13541.0F));
	actual = quat.Unit();
    EXPECT_EQ(actual, expected);
}

TEST(TestQuaternion, Conjugate) {
	EXPECT_EQ(Quaternion::Identity().Conjugate(), Quaternion::Identity());
	EXPECT_EQ(Quaternion(1.0F, 3.0F, -4.5F, -13.3F).Conjugate(), Quaternion(1.0F, -3.0F, 4.5F, 13.3F));
}

TEST(TestQuaternion, Inverse) {
	Quaternion expected, actual, quat;

	quat = Quaternion::Identity();
	expected = quat;
	actual = quat.Inverse();
    EXPECT_EQ(actual, expected);

	quat = Quaternion(25.0F, -13.0F, -3.0F, 231.0F);
	expected = Quaternion(25.0F, 13.0F, 3.0F, -231.0F) / (2.0F * Sqrt(13541.0F));
	actual = quat.Inverse();
    EXPECT_EQ(actual, expected);

	quat = Quaternion(35.0F, 10.0F, 11.0F, 12.0F);
	expected = Quaternion(35.0F, -10.0F, -11.0F, -12.0F) / Sqrt(1590.0F);
	actual = quat.Inverse();
	EXPECT_EQ(actual, expected);

}

TEST(TestQuaternion, Negate) {
	EXPECT_EQ(Quaternion::Identity().Negate(), Quaternion::Identity() * -1.0F);
	EXPECT_EQ(Quaternion(-1.0F, 3.0F, 4.5F, -5.0F).Negate(), Quaternion(1.0F, -3.0F, -4.5F, 5.0F));
}

TEST(TestQuaternion, Dot) {
	EXPECT_EQ(Quaternion::Dot(Quaternion::Identity(), Quaternion::Identity()), 1.0F);
	float dot = Quaternion::Dot(Quaternion(1.0F, 3.0F, 23.0F, 113.0F), Quaternion(32.0F, 13.0F, 2.0F, 3.0F));
	EXPECT_EQ(dot, 456.0F);
}

TEST(TestQuaternion, FromAngleAxis) {
	Quaternion expected, actual;

	// Round to nearest tenth and check
	auto CHECK_FLT_EQ = [] (Quaternion a, Quaternion e) {
		EXPECT_EQ(Floor(a.w_ * 10.0F + 0.50F) / 10.0F, e.w_);
		EXPECT_EQ(Floor(a.x_ * 10.0F + 0.50F) / 10.0F, e.x_);
		EXPECT_EQ(Floor(a.y_ * 10.0F + 0.50F) / 10.0F, e.y_);
		EXPECT_EQ(Floor(a.z_ * 10.0F + 0.50F) / 10.0F, e.z_);
	};

	expected = Quaternion(0.9F, 0.4F, 0.0F, 0.0F);
	actual = Quaternion::FromAngleAxis(Vec3f::Right(), Degree(45.0F));
	CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(0.7F, -0.7F, 0.0F, 0.0F);
	actual = Quaternion::FromAngleAxis(Vec3f::Left(), Degree(95.0F));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(1.0F, 0.0F, 0.1F, 0.0F);
	actual = Quaternion::FromAngleAxis(Vec3f::Up(), Degree(11.3F));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(1.0F, 0.0F, -0.2F, 0.0F);
	actual = Quaternion::FromAngleAxis(Vec3f::Down(), Degree(23.3F));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(-1.0F, 0.0F, 0.0F, 0.2F);
	actual = Quaternion::FromAngleAxis(Vec3f::Back(), Degree(385.2F));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(0.7F, 0.0F, 0.0F, -0.7F);
	actual = Quaternion::FromAngleAxis(Vec3f::Forward(), Degree(-96.3F));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(-1.0F, -0.1F, -0.1F, -0.1F);
	actual = Quaternion::FromAngleAxis(Vec3f::One(), Degree(-334.1F));
    CHECK_FLT_EQ(actual, expected);

	expected = Quaternion(1.0F, 0.0F, 0.0F, 0.0F);
	actual = Quaternion::FromAngleAxis(Vec3f::Zero(), Degree(-0.1F));
    CHECK_FLT_EQ(actual, expected);
}

TEST(TestQuaternion, LookRotation) {
	EXPECT_EQ(Quaternion::LookRotation(Vec3f::Right()).GetEulerAngles(),
              Vec3f(0.0F, 90.0F * kDegreeToRadian, 0.0F));
}

TEST(TestQuaternion, FromToRotation) {
	EXPECT_EQ(Quaternion::FromToRotation(Vec3f::Right(), Vec3f::Up()).GetEulerAngles(),
              Vec3f(0.0F, 90.0F * kDegreeToRadian, 0.0F));
}