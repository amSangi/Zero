#include "gtest/gtest.h"
#include "Angle.hpp"
#include "Quaternion.hpp"

using namespace zero::math;

TEST(TestQuaternion, ScalarOperations) {
	Quaternion quat = Quaternion::Identity();

	EXPECT_EQ(Quaternion(2.0f, 1.0f, 1.0f, 1.0f), quat + 1.0f);
	EXPECT_EQ(Quaternion(0.0f, -1.0f, -1.0f, -1.0f), quat - 1.0f);
	EXPECT_EQ(Quaternion(5.0f, 2.5f, 2.5f, 2.5f), (quat + 1.0f) * 2.5f);
	EXPECT_EQ(Quaternion(1.0f, 0.5f, 0.5f, 0.5f), (quat + 1.0f) / 2.0f);

	quat += 1.0f;
	EXPECT_EQ(Quaternion(2.0f, 1.0f, 1.0f, 1.0f), quat);
	quat -= 2.0f;
	EXPECT_EQ(Quaternion(0.0f, -1.0f, -1.0f, -1.0f), quat);
	quat *= 2.5f;
	EXPECT_EQ(Quaternion(0.0f, -2.5f, -2.5f, -2.5f), quat);
	quat /= 2.5f;
	EXPECT_EQ(Quaternion(0.0f, -1.0f, -1.0f, -1.0f), quat);
}

TEST(TestQuaternion, QuaternionQuaternionOperations) {
	Quaternion quat1 = Quaternion::Identity();
	Quaternion quat2(2.5f, 3.5f, 1.0f, 5.0f);

	EXPECT_EQ(Quaternion(3.5f, 3.5f, 1.0f, 5.0f), quat1 + quat2);
	EXPECT_EQ(Quaternion(-1.5f, -3.5f, -1.0f, -5.0f), quat1 - quat2);

	EXPECT_TRUE(quat1 * quat2 == quat2 *quat1);
	EXPECT_EQ(Quaternion(2.5f, 3.5f, 1.0f, 5.0f), quat1 * quat2);

	Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion q2(5.0f, 6.0f, 7.0f, 8.0f);
	EXPECT_TRUE(q1 * q2 != q2 * q1);
	EXPECT_EQ(Quaternion(-60.0f, 12.0f, 30.0f, 24.0f), q1 * q2);
	EXPECT_EQ(Quaternion(-60.0f, 20.0f, 14.0f, 32.0f), q2 * q1);



	quat1 += quat2;
	EXPECT_EQ(Quaternion(3.5f, 3.5f, 1.0f, 5.0f), quat1);

	quat1 -= (quat2 * 2.0f);
	EXPECT_EQ(Quaternion(-1.5f, -3.5f, -1.0f, -5.0f), quat1);

	quat1 = Quaternion::Identity();
	quat1 *= quat2;
	EXPECT_EQ(quat2, quat1);


	Quaternion temp = q1;
	q1 *= q2;
	q2 *= temp;
	EXPECT_TRUE(q1 != q2);
	EXPECT_EQ(Quaternion(-60.0f, 12.0f, 30.0f, 24.0f), q1);
	EXPECT_EQ(Quaternion(-60.0f, 20.0f, 14.0f, 32.0f), q2);
}

TEST(TestQuaternion, QuaternionVectorOperations) {
	Quaternion identity = Quaternion::Identity();
	Vector3 expected, actual;

	Vector3 vector(1.0f, 2.0f, 3.0f);
	expected = vector;
	actual = identity * vector;
	EXPECT_EQ(expected, actual);


	expected = Vector3(25.0f, 2.0f, -9.0f);
	actual = Quaternion(1.0f, 2.0f, 3.0f, 4.0f) * Vector3(1.0f, 2.0f, 3.0f);
	EXPECT_EQ(expected, actual);

	expected = Vector3(261.0f, -1514.0f, -419.0f);
	actual = Quaternion(13.0f, 11.0f, 3.0f, -4.0f) * vector;
	EXPECT_EQ(expected, actual);

	expected = Vector3(-1078029.0f, 6407.0f, -2368179.0f);
	actual = Quaternion(11.0f, -3.0f, 234.0f, 2.0f) * Vector3(11.0f, -13.0f, 21.0f);
	EXPECT_EQ(expected, actual);

}

TEST(TestQuaternion, Norm) {
	EXPECT_EQ(0.0f, Quaternion::Zero().Norm());
	EXPECT_EQ(1.0f, Quaternion::Identity().Norm());
	EXPECT_EQ(3.0f * Sqrt(395.0f), Quaternion(1.0f, 23.0f, 44.0f, 33.0f).Norm());
	EXPECT_EQ(Sqrt(3035.0f), Quaternion(1.0f, -3.0f, 44.0f, -33.0f).Norm());
}

TEST(TestQuaternion, UnitNorm) {
	Quaternion expected, actual, quat;

	EXPECT_EQ(Quaternion::Identity(), Quaternion::Identity().Unit());

	quat = Quaternion(0.0f, 1.0f, 1.0f, 1.0f);
	expected = quat / Sqrt(3.0f);
	actual = quat.Unit();
	EXPECT_EQ(expected, actual);

	quat = Quaternion(25.0f, -13.0f, -3.0f, 231.0f);
	expected = quat / (2.0f * Sqrt(13541.0f));
	actual = quat.Unit();
	EXPECT_EQ(expected, actual);
}

TEST(TestQuaternion, Conjugate) {
	EXPECT_EQ(Quaternion::Identity(), Quaternion::Identity().Conjugate());
	EXPECT_EQ(Quaternion(1.0f, -3.0f, 4.5f, 13.3f), Quaternion(1.0f, 3.0f, -4.5f, -13.3f).Conjugate());
}

TEST(TestQuaternion, Inverse) {
	Quaternion expected, actual, quat;

	quat = Quaternion::Identity();
	expected = quat;
	actual = quat.Inverse();
	EXPECT_EQ(expected, actual);

	quat = Quaternion(25.0f, -13.0f, -3.0f, 231.0f);
	expected = Quaternion(25.0f, 13.0f, 3.0f, -231.0f) / (2.0f * Sqrt(13541.0f));
	actual = quat.Inverse();
	EXPECT_EQ(expected, actual);

	quat = Quaternion(35.0f, 10.0f, 11.0f, 12.0f);
	expected = Quaternion(35.0f, -10.0f, -11.0f, -12.0f) / Sqrt(1590.0f);
	actual = quat.Inverse();
	EXPECT_EQ(expected, actual);

}

TEST(TestQuaternion, Negate) {
	EXPECT_EQ(Quaternion::Identity() * -1.0f, Quaternion::Identity().Negate());
	EXPECT_EQ(Quaternion(1.0f, -3.0f, -4.5f, 5.0f), Quaternion(-1.0f, 3.0f, 4.5f, -5.0f).Negate());
}

TEST(TestQuaternion, Dot) {
	EXPECT_EQ(1.0f, Quaternion::Dot(Quaternion::Identity(), Quaternion::Identity()));
	float dot = Quaternion::Dot(Quaternion(1.0f, 3.0f, 23.0f, 113.0f), Quaternion(32.0f, 13.0f, 2.0f, 3.0f));
	EXPECT_EQ(456.0f, dot);
}

TEST(TestQuaternion, FromAngleAxis) {
	Quaternion expected, actual;

	// Round to nearest tenth and check
	auto CHECK_FLT_EQ = [] (Quaternion e, Quaternion a) {
		EXPECT_EQ(e.w_, Floor(a.w_ * 10.0f + 0.50f) / 10.0f);
		EXPECT_EQ(e.x_, Floor(a.x_ * 10.0f + 0.50f) / 10.0f);
		EXPECT_EQ(e.y_, Floor(a.y_ * 10.0f + 0.50f) / 10.0f);
		EXPECT_EQ(e.z_, Floor(a.z_ * 10.0f + 0.50f) / 10.0f);
	};

	expected = Quaternion(0.9f, 0.4f, 0.0f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vector3::Right(), Degree(45.0f));
	CHECK_FLT_EQ(expected, actual);

	expected = Quaternion(0.7f, -0.7f, 0.0f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vector3::Left(), Degree(95.0f));
	CHECK_FLT_EQ(expected, actual);

	expected = Quaternion(1.0f, 0.0f, 0.1f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vector3::Up(), Degree(11.3f));
	CHECK_FLT_EQ(expected, actual);

	expected = Quaternion(1.0f, 0.0f, -0.2f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vector3::Down(), Degree(23.3f));
	CHECK_FLT_EQ(expected, actual);

	expected = Quaternion(-1.0f, 0.0f, 0.0f, 0.2f);
	actual = Quaternion::FromAngleAxis(Vector3::Back(), Degree(385.2f));
	CHECK_FLT_EQ(expected, actual);

	expected = Quaternion(0.7f, 0.0f, 0.0f, -0.7f);
	actual = Quaternion::FromAngleAxis(Vector3::Forward(), Degree(-96.3f));
	CHECK_FLT_EQ(expected, actual);

	expected = Quaternion(-1.0f, -0.1f, -0.1f, -0.1f);
	actual = Quaternion::FromAngleAxis(Vector3::One(), Degree(-334.1f));
	CHECK_FLT_EQ(expected, actual);

	expected = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	actual = Quaternion::FromAngleAxis(Vector3::Zero(), Degree(-0.1f));
	CHECK_FLT_EQ(expected, actual);
}

TEST(TestQuaternion, LookRotation) {
	EXPECT_EQ(Vector3(0.0f, 90.0f * DEGREE_TO_RADIAN, 0.0f),
              Quaternion::LookRotation(Vector3::Right()).GetEulerAngles());
}

TEST(TestQuaternion, FromToRotation) {
	EXPECT_EQ(Vector3(0.0f, 90.0f * DEGREE_TO_RADIAN, 0.0f),
              Quaternion::FromToRotation(Vector3::Right(), Vector3::Up()).GetEulerAngles());
}