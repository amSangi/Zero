#include <gtest/gtest.h>
#include "math/Matrix4x4.hpp"
#include "math/Vector4.hpp"
#include "math/Quaternion.hpp"

using namespace zero::math;

TEST(TestMatrix4, ScalarOperations)
{
	Matrix4x4 matrix = Matrix4x4::Identity();

	EXPECT_EQ(matrix + 1.0F, Matrix4x4(2.0F, 1.0F, 1.0F, 1.0F,
	                                   1.0F, 2.0F, 1.0F, 1.0F,
	                                   1.0F, 1.0F, 2.0F, 1.0F,
	                                   1.0F, 1.0F, 1.0F, 2.0F));

	EXPECT_EQ(matrix - 1.0F, Matrix4x4(0.0F, -1.0F, -1.0F, -1.0F,
	                                   -1.0F, 0.0F, -1.0F, -1.0F,
	                                   -1.0F, -1.0F, 0.0F, -1.0F,
	                                   -1.0F, -1.0F, -1.0F, 0.0F));

	EXPECT_EQ((matrix + 1.0F) * -1.0F, Matrix4x4(-2.0F, -1.0F, -1.0F, -1.0F,
	                                             -1.0F, -2.0F, -1.0F, -1.0F,
	                                             -1.0F, -1.0F, -2.0F, -1.0F,
	                                             -1.0F, -1.0F, -1.0F, -2.0F));

	EXPECT_EQ((matrix + 1.0F) / 2.0F, Matrix4x4(1.0F, 0.5F, 0.5F, 0.5F,
	                                            0.5F, 1.0F, 0.5F, 0.5F,
	                                            0.5F, 0.5F, 1.0F, 0.5F,
	                                            0.5F, 0.5F, 0.5F, 1.0F));


	matrix += 1.0F;
	EXPECT_EQ(matrix, Matrix4x4(2.0F, 1.0F, 1.0F, 1.0F,
	                            1.0F, 2.0F, 1.0F, 1.0F,
	                            1.0F, 1.0F, 2.0F, 1.0F,
	                            1.0F, 1.0F, 1.0F, 2.0F));

	matrix -= 4.0F;
	EXPECT_EQ(matrix, Matrix4x4(-2.0F, -3.0F, -3.0F, -3.0F,
	                            -3.0F, -2.0F, -3.0F, -3.0F,
	                            -3.0F, -3.0F, -2.0F, -3.0F,
	                            -3.0F, -3.0F, -3.0F, -2.0F));

	matrix *= 10.0F;
	EXPECT_EQ(matrix, Matrix4x4(-20.0F, -30.0F, -30.0F, -30.0F,
	                            -30.0F, -20.0F, -30.0F, -30.0F,
	                            -30.0F, -30.0F, -20.0F, -30.0F,
	                            -30.0F, -30.0F, -30.0F, -20.0F));

	matrix /= -1.0F;
	EXPECT_EQ(matrix, Matrix4x4(20.0F, 30.0F, 30.0F, 30.0F,
	                            30.0F, 20.0F, 30.0F, 30.0F,
	                            30.0F, 30.0F, 20.0F, 30.0F,
	                            30.0F, 30.0F, 30.0F, 20.0F));
}

TEST(TestMatrix4, MatrixMatrixAddition)
{
	Matrix4x4 matrix = Matrix4x4::Identity();

	EXPECT_EQ((matrix + 1.0F) + matrix, Matrix4x4(3.0F, 1.0F, 1.0F, 1.0F,
	                                              1.0F, 3.0F, 1.0F, 1.0F,
	                                              1.0F, 1.0F, 3.0F, 1.0F,
	                                              1.0F, 1.0F, 1.0F, 3.0F));

	matrix += (matrix + 1.0F);
	EXPECT_EQ(matrix, Matrix4x4(3.0F, 1.0F, 1.0F, 1.0F,
	                            1.0F, 3.0F, 1.0F, 1.0F,
	                            1.0F, 1.0F, 3.0F, 1.0F,
	                            1.0F, 1.0F, 1.0F, 3.0F));
}

TEST(TestMatrix4, MatrixMatrixSubtraction)
{
	Matrix4x4 matrix = Matrix4x4::Identity();

	EXPECT_EQ((matrix - 1.0F) - matrix, Matrix4x4(-1.0F, -1.0F, -1.0F, -1.0F,
	                                              -1.0F, -1.0F, -1.0F, -1.0F,
	                                              -1.0F, -1.0F, -1.0F, -1.0F,
	                                              -1.0F, -1.0F, -1.0F, -1.0F));


	matrix -= (matrix - 1.0F);
	EXPECT_EQ(matrix, Matrix4x4(1.0F, 1.0F, 1.0F, 1.0F,
	                            1.0F, 1.0F, 1.0F, 1.0F,
	                            1.0F, 1.0F, 1.0F, 1.0F,
	                            1.0F, 1.0F, 1.0F, 1.0F));
}

TEST(TestMatrix4, Hadamard)
{
	Matrix4x4 m1(1.0F, 2.0F, 3.0F, 3.0F,
	             4.0F, 5.0F, 6.0F, 6.0F,
	             7.0F, 8.0F, 9.0F, 9.0F,
	             10.0F, 11.0F, 12.0F, 13.0F);
	Matrix4x4 m2 = m1;

	EXPECT_EQ(Matrix4x4::Hadamard(m1, m2), Matrix4x4(1.0F, 4.0F, 9.0F, 9.0F,
	                                                 16.0F, 25.0F, 36.0F, 36.0F,
	                                                 49.0F, 64.0F, 81.0F, 81.0F,
	                                                 100.0F, 121.0F, 144.0F, 169.0F));
}

TEST(TestMatrix4, Det)
{
	Matrix4x4 m1(1.0F, 2.0F, 3.0F, 3.0F,
	             4.0F, 5.0F, 6.0F, 6.0F,
	             7.0F, 8.0F, 9.0F, 9.0F,
	             10.0F, 11.0F, 12.0F, 13.0F);

	Matrix4x4 m2(-23, -33, 4, 3,
	             -4, -3, 6, 5,
	             -7, 8, -1, 6,
	             8, -9, 1, -2);

	EXPECT_EQ(m1.Det(), 0.0F);
	EXPECT_EQ((m1 + m1).Det(), 0.0F);
	EXPECT_EQ((Matrix4x4::Hadamard(m1, m1)).Det(), -5400.0F);
	EXPECT_EQ(m2.Det(), 11469.0F);
}

TEST(TestMatrix4, Inverse)
{
	Matrix4x4 matrix = Matrix4x4::Identity();

	EXPECT_EQ(matrix.Inverse(), matrix);
	EXPECT_EQ(Matrix4x4(1.0F).Inverse(), Matrix4x4(0.0F));
	EXPECT_EQ((matrix + 1.0F).Inverse(), Matrix4x4(0.8F, -0.2F, -0.2F, -0.2F,
	                                               -0.2F, 0.8F, -0.2F, -0.2F,
	                                               -0.2F, -0.2F, 0.8F, -0.2F,
	                                               -0.2F, -0.2F, -0.2F, 0.8F));
}

TEST(TestMatrix4, Transpose)
{
	Matrix4x4 m(1.0F, 4.0F, 9.0F, 9.0F,
	            16.0F, 25.0F, 36.0F, 4.0F,
	            49.0F, 64.0F, 81.0F, 81.0F,
	            49.0F, 64.0F, 81.0F, 144.0F);

	EXPECT_EQ(m.Transpose(), Matrix4x4(1.0F, 16.0F, 49.0F, 49.0F,
	                                   4.0F, 25.0F, 64.0F, 64.0F,
	                                   9.0F, 36.0F, 81.0F, 81.0F,
	                                   9.0F, 4.0F, 81.0F, 144.0F));
}

TEST(TestMatrix4, MatrixVectorMultiply)
{
	Matrix4x4 matrix = Matrix4x4::Identity();
	Vec4f vector(1.0F, 2.0F, 3.0F, 4.0F);

	EXPECT_EQ(matrix * vector, Vec4f(1.0F, 2.0F, 3.0F, 4.0F));
	EXPECT_EQ((matrix * 2.0F) * vector, Vec4f(2.0F, 4.0F, 6.0F, 8.0F));
	EXPECT_EQ((matrix * -1.0F) * vector, Vec4f(-1.0F, -2.0F, -3.0F, -4.0F));
	EXPECT_EQ((matrix + 1.0F) * vector, Vec4f(11.0F, 12.0F, 13.0F, 14.0F));
}

TEST(TestMatrix4, MatrixMatrixMultiply)
{
	Matrix4x4 m1(1.0F, 2.0F, 3.0F, 3.0F,
	             4.0F, 5.0F, 6.0F, 6.0F,
	             7.0F, 8.0F, 9.0F, 10.0F,
	             11.0F, 12.0F, 13.0F, 14.0F);

	Matrix4x4 m2(13.0F, 14.0F, 15.0F, 11.0F,
	             17.0F, 83.0F, 32.0F, 3.0F,
	             3.0F, 15.0F, 7.0F, 5.0F,
	             17.0F, 14.0F, 13.0F, 5.0F);

	EXPECT_EQ(Matrix4x4::Identity() * Matrix4x4::Identity(), Matrix4x4::Identity());
	EXPECT_EQ(m1 * m2, Matrix4x4(107.0F, 267.0F, 139.0F, 47.0F,
	                             257.0F, 645.0F, 340.0F, 119.0F,
	                             424.0F, 1037.0F, 554.0F, 196.0F,
	                             624.0F, 1541.0F, 822.0F, 292.0F));
}

TEST(TestMatrix4, Translate)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    EXPECT_EQ(matrix.Translate(Vec3f::One()), Matrix4x4(1.0F, 0.0F, 0.0F, 1.0F,
                                                        0.0F, 1.0F, 0.0F, 1.0F,
                                                        0.0F, 0.0F, 1.0F, 1.0F,
                                                        0.0F, 0.0F, 0.0F, 1.0F));
    EXPECT_EQ(matrix.Translate(Vec3f::One()), Matrix4x4(1.0F, 0.0F, 0.0F, 2.0F,
                                                        0.0F, 1.0F, 0.0F, 2.0F,
                                                        0.0F, 0.0F, 1.0F, 2.0F,
                                                        0.0F, 0.0F, 0.0F, 1.0F));
    EXPECT_EQ(matrix.Translate(Vec3f(3.3F, -2.5F, -1.0F)), Matrix4x4(1.0F, 0.0F, 0.0F, 5.3F,
                                                                     0.0F, 1.0F, 0.0F, -0.5F,
                                                                     0.0F, 0.0F, 1.0F, 1.0F,
                                                                     0.0F, 0.0F, 0.0F, 1.0F));
}

TEST(TestMatrix4, Rotate)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    Quaternion rotation = Quaternion::FromEuler(Radian(0.0F),
                                                Radian(0.0F),
                                                Degree(90.0F).ToRadian());
    Quaternion rotation2 = Quaternion::FromEuler(Radian(0.0F),
                                                 Radian(0.0F),
                                                 Degree(180.0F).ToRadian());
    EXPECT_EQ(matrix.Rotate(rotation),
              Matrix4x4(1.0F, 0.0F, 0.0F, 0.0F,
                        0.0F, 0.0F, -1.0F, 0.0F,
                        0.0F, 1.0F, 0.0F, 0.0F,
                        0.0F, 0.0F, 0.0F, 1.0F));

    EXPECT_EQ(matrix.Rotate(rotation),
              Matrix4x4::Identity().Rotate(rotation2));
}

TEST(TestMatrix4, Scale)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    EXPECT_EQ(matrix.Scale(Vec3f::One()), Matrix4x4(1.0F, 0.0F, 0.0F, 0.0F,
                                                    0.0F, 1.0F, 0.0F, 0.0F,
                                                    0.0F, 0.0F, 1.0F, 0.0F,
                                                    0.0F, 0.0F, 0.0F, 1.0F));
    EXPECT_EQ(matrix.Scale(Vec3f(2.0, 3.0F, -4.5F)), Matrix4x4(2.0F, 0.0F, 0.0F, 0.0F,
                                                               0.0F, 3.0F, 0.0F, 0.0F,
                                                               0.0F, 0.0F, -4.5F, 0.0F,
                                                               0.0F, 0.0F, 0.0F, 1.0F));
    EXPECT_EQ(matrix.Scale(Vec3f(2.0F)), Matrix4x4(4.0F, 0.0F, 0.0F, 0.0F,
                                                   0.0F, 6.0F, 0.0F, 0.0F,
                                                   0.0F, 0.0F, -9.0F, 0.0F,
                                                   0.0F, 0.0F, 0.0F, 1.0F));
}

TEST(TestMatrix4, TranslateRotateScale)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    Quaternion rotation = Quaternion::FromEuler(Radian(0.0F), Radian(0.0F), Degree(90.0F).ToRadian());
    Vec3f translation(2.5F, 2.5F, -10.0F);
    Vec3f scale(2.0F, 2.0F, 2.0F);
    matrix.Scale(scale)
          .Rotate(rotation)
          .Translate(translation);
    EXPECT_EQ(matrix, Matrix4x4(2.0F, 0.0F, 0.0F, 2.5F,
                                0.0F, 0.0F, -2.0F, 2.5F,
                                0.0F, 2.0F, 0.0F, -10.0F,
                                0.0F, 0.0F, 0.0F, 1.0F));
}

TEST(TestMatrix4, GetTranslate)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    Quaternion rotation = Quaternion::FromEuler(Radian(0.0F), Radian(0.0F), Degree(90.0F).ToRadian());
    Vec3f translation(2.5F, 2.5F, -10.0F);
    Vec3f scale(2.0F, 2.0F, 2.0F);
    matrix.Scale(scale)
          .Rotate(rotation)
          .Translate(translation);
    EXPECT_EQ(matrix.GetTranslation(), translation);
}

TEST(TestMatrix4, GetRotation)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    Quaternion rotation = Quaternion::FromEuler(Radian(0.0F), Radian(0.0F), Degree(90.0F).ToRadian());
    Vec3f translation(2.5F, 2.5F, -10.0F);
    Vec3f scale(2.0F, 2.0F, 2.0F);
    matrix.Scale(scale)
          .Rotate(rotation)
          .Translate(translation);
    EXPECT_EQ(matrix.GetRotation(), rotation);
}

TEST(TestMatrix4, GetScale)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    Quaternion rotation = Quaternion::FromEuler(Radian(0.0F), Radian(0.0F), Degree(90.0F).ToRadian());
    Vec3f translation(2.5F, 2.5F, -10.0F);
    Vec3f scale(2.0F, 2.0F, 2.0F);
    matrix.Scale(scale)
          .Rotate(rotation)
          .Translate(translation);
    EXPECT_EQ(matrix.GetScale(), scale);
}

TEST(TestMatrix4, GetNegativeScale)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    Quaternion rotation = Quaternion::FromEuler(Radian(0.0F), Radian(0.0F), Degree(90.0F).ToRadian());
    Vec3f translation(2.5F, 2.5F, -10.0F);
    Vec3f scale(2.0F, -2.0F, 2.0F);
    matrix.Scale(scale)
            .Rotate(rotation)
            .Translate(translation);
    EXPECT_NE(matrix.GetScale(), scale);
}

TEST(TestMatrix4, GetMatrix3x3)
{
    Matrix4x4 matrix = Matrix4x4::Identity();
    Quaternion rotation = Quaternion::FromEuler(Radian(0.0F), Radian(0.0F), Degree(90.0F).ToRadian());
    Vec3f translation(2.5F, 2.5F, -10.0F);
    Vec3f scale(2.0F, 2.0F, 2.0F);
    matrix.Scale(scale)
          .Rotate(rotation)
          .Translate(translation);
    EXPECT_EQ(matrix.GetMatrix3x3(), Matrix3x3(2.0F, 0.0F, 0.0F,
                                               0.0F, 0.0F, -2.0F,
                                               0.0F, 2.0F, 0.0F));
}
