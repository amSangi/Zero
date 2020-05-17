#include <gtest/gtest.h>
#include "math/Matrix3x3.hpp"
#include "math/Vector3.hpp"

using namespace zero::math;

TEST(TestMatrix3, ScalarOperations)
{
	Matrix3x3 matrix = Matrix3x3::Identity();

	EXPECT_EQ(matrix + 1.0F, Matrix3x3(2.0F, 1.0F, 1.0F,
	                                   1.0F, 2.0F, 1.0F,
	                                   1.0F, 1.0F, 2.0F));

	EXPECT_EQ(matrix - 1.0F, Matrix3x3(0.0F, -1.0F, -1.0F,
	                                   -1.0F, 0.0F, -1.0F,
	                                   -1.0F, -1.0F, 0.0F));

	EXPECT_EQ((matrix + 1.0F) * -1.0F, Matrix3x3(-2.0F, -1.0F, -1.0F,
	                                             -1.0F, -2.0F, -1.0F,
	                                             -1.0F, -1.0F, -2.0F));

	EXPECT_EQ((matrix + 1.0F) / 2.0F, Matrix3x3(1.0F, 0.5F, 0.5F,
	                                            0.5F, 1.0F, 0.5F,
	                                            0.5F, 0.5F, 1.0F));


	matrix += 1.0F;
	EXPECT_EQ(matrix, Matrix3x3(2.0F, 1.0F, 1.0F,
	                            1.0F, 2.0F, 1.0F,
	                            1.0F, 1.0F, 2.0F));

	matrix -= 4.0F;
	EXPECT_EQ(matrix, Matrix3x3(-2.0F, -3.0F, -3.0F,
	                            -3.0F, -2.0F, -3.0F,
	                            -3.0F, -3.0F, -2.0F));

	matrix *= 10.0F;
	EXPECT_EQ(matrix, Matrix3x3(-20.0F, -30.0F, -30.0F,
	                            -30.0F, -20.0F, -30.0F,
	                            -30.0F, -30.0F, -20.0F));

	matrix /= -1.0F;
	EXPECT_EQ(matrix, Matrix3x3(20.0F, 30.0F, 30.0F,
	                            30.0F, 20.0F, 30.0F,
	                            30.0F, 30.0F, 20.0F));
}

TEST(TestMatrix3, MatrixMatrixAddition)
{
	Matrix3x3 matrix = Matrix3x3::Identity();

	EXPECT_EQ((matrix + 1.0F) + matrix, Matrix3x3(3.0F, 1.0F, 1.0F,
	                                              1.0F, 3.0F, 1.0F,
	                                              1.0F, 1.0F, 3.0F));

	matrix += (matrix + 1.0F);
	EXPECT_EQ(matrix, Matrix3x3(3.0F, 1.0F, 1.0F,
	                            1.0F, 3.0F, 1.0F,
	                            1.0F, 1.0F, 3.0F));
}

TEST(TestMatrix3, MatrixMatrixSubtraction)
{
	Matrix3x3 matrix = Matrix3x3::Identity();

	EXPECT_EQ((matrix - 1.0F) - matrix, Matrix3x3(-1.0F, -1.0F, -1.0F,
	                                              -1.0F, -1.0F, -1.0F,
	                                              -1.0F, -1.0F, -1.0F));


	matrix -= (matrix - 1.0F);
	EXPECT_EQ(matrix, Matrix3x3(1.0F, 1.0F, 1.0F,
	                            1.0F, 1.0F, 1.0F,
	                            1.0F, 1.0F, 1.0F));
}

TEST(TestMatrix3, Hadamard)
{
	Matrix3x3 m1(1.0F, 2.0F, 3.0F,
	             4.0F, 5.0F, 6.0F,
	             7.0F, 8.0F, 9.0F);
	Matrix3x3 m2 = m1;

	EXPECT_EQ(Matrix3x3::Hadamard(m1, m2), Matrix3x3(1.0F, 4.0F, 9.0F,
	                                                 16.0F, 25.0F, 36.0F,
	                                                 49.0F, 64.0F, 81.0F));
}

TEST(TestMatrix3, Det)
{
	Matrix3x3 m1(1.0F, 2.0F, 3.0F,
	             4.0F, 5.0F, 6.0F,
	             7.0F, 8.0F, 9.0F);

	Matrix3x3 m2(-23, -33, 4,
	             -4, -3, 6,
	             -7, 8, -1);

	EXPECT_EQ(m1.Det(), 0.0F);
	EXPECT_EQ((m1 + m1).Det(), 0.0F);
	EXPECT_EQ((Matrix3x3::Hadamard(m1, m1)).Det(), -216.0F);
	EXPECT_EQ(m2.Det(), 2341.0F);
}

TEST(TestMatrix3, Inverse)
{
	Matrix3x3 matrix = Matrix3x3::Identity();

	EXPECT_EQ(matrix, matrix.Inverse());
	EXPECT_EQ(Matrix3x3(1.0F).Inverse(), Matrix3x3(0.0F));
	EXPECT_EQ((matrix + 1.0F).Inverse(), Matrix3x3(0.75F, -0.25F, -0.25F,
	                                               -0.25F, 0.75F, -0.25F,
	                                               -0.25F, -0.25F, 0.75F));
}

TEST(TestMatrix3, Transpose)
{
	Matrix3x3 m(1.0F, 4.0F, 9.0F,
	            16.0F, 25.0F, 36.0F,
	            49.0F, 64.0F, 81.0F);

	EXPECT_EQ(m.Transpose(), Matrix3x3(1.0F, 16.0F, 49.0F,
	                                   4.0F, 25.0F, 64.0F,
	                                   9.0F, 36.0F, 81.0F));
}

TEST(TestMatrix3, MatrixVectorMultiply)
{
	Matrix3x3 matrix = Matrix3x3::Identity();
	Vec3f vector(1.0F, 2.0F, 3.0F);

	EXPECT_EQ(matrix * vector, Vec3f(1.0F, 2.0F, 3.0F));
	EXPECT_EQ((matrix * 2.0F) * vector, Vec3f(2.0F, 4.0F, 6.0F));
	EXPECT_EQ((matrix * -1.0F) * vector, Vec3f(-1.0F, -2.0F, -3.0F));
	EXPECT_EQ((matrix + 1.0F) * vector, Vec3f(7.0F, 8.0F, 9.0F));

}

TEST(TestMatrix3, MatrixMatrixMultiply)
{
	Matrix3x3 m1(1.0F, 2.0F, 3.0F,
	             4.0F, 5.0F, 6.0F,
	             7.0F, 8.0F, 9.0F);

	Matrix3x3 m2(13.0F, 14.0F, 15.0F,
	             17.0F, 83.0F, 32.0F,
	             3.0F, 15.0F, 7.0F);

	EXPECT_EQ(Matrix3x3::Identity() * Matrix3x3::Identity(), Matrix3x3::Identity());
	EXPECT_EQ(m1 * m2, Matrix3x3(56.0F, 225.0F, 100.0F,
	                             155.0F, 561.0F, 262.0F,
	                             254.0F, 897.0F, 424.0F));
}
