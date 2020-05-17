#include <gtest/gtest.h>
#include "math/Angle.hpp"

using namespace zero::math;

TEST(TestAngle, RadianToDegree)
{
	EXPECT_EQ(Radian(0.0F).ToDegree(), Degree(0.0F));
    EXPECT_EQ(Radian(kPi / 4.0F).ToDegree(), Degree(45.0F));
    EXPECT_EQ(Radian(kHalfPi).ToDegree(), Degree(90.0F));
    EXPECT_EQ(Radian(kPi).ToDegree(), Degree(180.0F));
    EXPECT_EQ(Radian(kPi * 2.0F).ToDegree(), Degree(360.0F));
}

TEST(TestAngle, DegreeToRadian)
{
    EXPECT_EQ(Degree(0.0F).ToRadian(), Radian(0.0F));
    EXPECT_EQ(Degree(45.0F).ToRadian(), Radian(kPi / 4.0F));
    EXPECT_EQ(Degree(90.0F).ToRadian(), Radian(kHalfPi));
    EXPECT_EQ(Degree(180.0F).ToRadian(), Radian(kPi));
    EXPECT_EQ(Degree(360.0F).ToRadian(), Radian(kPi * 2.0F));
}

TEST(TestAngle, RadianDegreeEquality)
{
    EXPECT_EQ(Radian(0.0F), Degree(0.0F));
    EXPECT_EQ(Radian(kPi / 4.0F), Degree(45.0F));
    EXPECT_EQ(Radian(kHalfPi), Degree(90.0F));
    EXPECT_EQ(Radian(kPi), Degree(180.0F));
    EXPECT_EQ(Radian(kPi * 2.0F), Degree(360.0F));
}