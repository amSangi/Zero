#include <gtest/gtest.h>
#include "math/Angle.hpp"

using namespace zero::math;

TEST(TestAngle, RadianToDegree) {
	EXPECT_EQ(Radian(0.0F).ToDegree(), Degree(0.0F));
    EXPECT_EQ(Radian(PI / 4.0F).ToDegree(), Degree(45.0F));
    EXPECT_EQ(Radian(PI_2).ToDegree(), Degree(90.0F));
    EXPECT_EQ(Radian(PI).ToDegree(), Degree(180.0F));
    EXPECT_EQ(Radian(PI * 2.0F).ToDegree(), Degree(360.0F));
}

TEST(TestAngle, DegreeToRadian) {
    EXPECT_EQ(Degree(0.0F).ToRadian(), Radian(0.0F));
    EXPECT_EQ(Degree(45.0F).ToRadian(), Radian(PI / 4.0F));
    EXPECT_EQ(Degree(90.0F).ToRadian(), Radian(PI_2));
    EXPECT_EQ(Degree(180.0F).ToRadian(), Radian(PI));
    EXPECT_EQ(Degree(360.0F).ToRadian(), Radian(PI * 2.0F));
}

TEST(TestAngle, RadianDegreeEquality) {
    EXPECT_EQ(Radian(0.0F), Degree(0.0F));
    EXPECT_EQ(Radian(PI / 4.0F), Degree(45.0F));
    EXPECT_EQ(Radian(PI_2), Degree(90.0F));
    EXPECT_EQ(Radian(PI), Degree(180.0F));
    EXPECT_EQ(Radian(PI * 2.0F), Degree(360.0F));
}