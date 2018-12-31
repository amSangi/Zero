#include "gtest/gtest.h"
#include "Angle.h"

using namespace Zero;

TEST(TestAngle, RadianToDegree) {
	EXPECT_TRUE(Radian(0.0f).ToDegree() == Degree(0.0f));
	EXPECT_TRUE(Radian(Zero::PI / 4.0f).ToDegree() == Degree(45.0f));
	EXPECT_TRUE(Radian(Zero::PI_2).ToDegree() == Degree(90.0f));
	EXPECT_TRUE(Radian(Zero::PI).ToDegree() == Degree(180.0f));
	EXPECT_TRUE(Radian(Zero::PI * 2.0f).ToDegree() == Degree(360.0f));
}

TEST(TestAngle, DegreeToRadian) {
	EXPECT_TRUE(Radian(0.0f) == Degree(0.0f).ToRadian());
	EXPECT_TRUE(Radian(Zero::PI / 4.0f) == Degree(45.0f).ToRadian());
	EXPECT_TRUE(Radian(Zero::PI_2) == Degree(90.0f).ToRadian());
	EXPECT_TRUE(Radian(Zero::PI) == Degree(180.0f).ToRadian());
	EXPECT_TRUE(Radian(Zero::PI * 2.0f) == Degree(360.0f).ToRadian());
}

TEST(TestAngle, RadianDegreeEquality) {
	EXPECT_TRUE(Radian(0.0f) == Degree(0.0f));
	EXPECT_TRUE(Radian(Zero::PI / 4.0f) == Degree(45.0f));
	EXPECT_TRUE(Radian(Zero::PI_2) == Degree(90.0f));
	EXPECT_TRUE(Radian(Zero::PI) == Degree(180.0f));
	EXPECT_TRUE(Radian(Zero::PI * 2.0f) == Degree(360.0f));
}