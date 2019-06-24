#include "gtest/gtest.h"
#include "math/Angle.hpp"

using namespace zero::math;

TEST(TestAngle, RadianToDegree) {
	EXPECT_EQ(Radian(0.0f).ToDegree(), Degree(0.0f));
    EXPECT_EQ(Radian(PI / 4.0f).ToDegree(), Degree(45.0f));
    EXPECT_EQ(Radian(PI_2).ToDegree(), Degree(90.0f));
    EXPECT_EQ(Radian(PI).ToDegree(), Degree(180.0f));
    EXPECT_EQ(Radian(PI * 2.0f).ToDegree(), Degree(360.0f));
}

TEST(TestAngle, DegreeToRadian) {
    EXPECT_EQ(Degree(0.0f).ToRadian(), Radian(0.0f));
    EXPECT_EQ(Degree(45.0f).ToRadian(), Radian(PI / 4.0f));
    EXPECT_EQ(Degree(90.0f).ToRadian(), Radian(PI_2));
    EXPECT_EQ(Degree(180.0f).ToRadian(), Radian(PI));
    EXPECT_EQ(Degree(360.0f).ToRadian(), Radian(PI * 2.0f));
}

TEST(TestAngle, RadianDegreeEquality) {
    EXPECT_EQ(Radian(0.0f), Degree(0.0f));
    EXPECT_EQ(Radian(PI / 4.0f), Degree(45.0f));
    EXPECT_EQ(Radian(PI_2), Degree(90.0f));
    EXPECT_EQ(Radian(PI), Degree(180.0f));
    EXPECT_EQ(Radian(PI * 2.0f), Degree(360.0f));
}