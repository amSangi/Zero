#include <gtest/gtest.h>
#include "math/Matrix3x3.hpp"
#include "math/Vector3.hpp"

using namespace zero::math;

TEST(TestMatrix3, ScalarOperations) {
	Matrix3x3 matrix = Matrix3x3::Identity();

	EXPECT_EQ(matrix + 1.0f, Matrix3x3(2.0f, 1.0f, 1.0f,
	                                   1.0f, 2.0f, 1.0f,
	                                   1.0f, 1.0f, 2.0f));

	EXPECT_EQ(matrix - 1.0f, Matrix3x3(0.0f, -1.0f, -1.0f,
	                                   -1.0f, 0.0f, -1.0f,
	                                   -1.0f, -1.0f, 0.0f));

	EXPECT_EQ((matrix + 1.0f) * -1.0f, Matrix3x3(-2.0f, -1.0f, -1.0f,
	                                             -1.0f, -2.0f, -1.0f,
	                                             -1.0f, -1.0f, -2.0f));

	EXPECT_EQ((matrix + 1.0f) / 2.0f, Matrix3x3(1.0f, 0.5f, 0.5f,
	                                            0.5f, 1.0f, 0.5f,
	                                            0.5f, 0.5f, 1.0f));


	matrix += 1.0f;
	EXPECT_EQ(matrix, Matrix3x3(2.0f, 1.0f, 1.0f,
	                            1.0f, 2.0f, 1.0f,
	                            1.0f, 1.0f, 2.0f));

	matrix -= 4.0f;
	EXPECT_EQ(matrix, Matrix3x3(-2.0f, -3.0f, -3.0f,
	                            -3.0f, -2.0f, -3.0f,
	                            -3.0f, -3.0f, -2.0f));

	matrix *= 10.0f;
	EXPECT_EQ(matrix, Matrix3x3(-20.0f, -30.0f, -30.0f,
	                            -30.0f, -20.0f, -30.0f,
	                            -30.0f, -30.0f, -20.0f));

	matrix /= -1.0f;
	EXPECT_EQ(matrix, Matrix3x3(20.0f, 30.0f, 30.0f,
	                            30.0f, 20.0f, 30.0f,
	                            30.0f, 30.0f, 20.0f));
}

TEST(TestMatrix3, MatrixMatrixAddition) {
	Matrix3x3 matrix = Matrix3x3::Identity();

	EXPECT_EQ((matrix + 1.0f) + matrix, Matrix3x3(3.0f, 1.0f, 1.0f,
	                                              1.0f, 3.0f, 1.0f,
	                                              1.0f, 1.0f, 3.0f));

	matrix += (matrix + 1.0f);
	EXPECT_EQ(matrix, Matrix3x3(3.0f, 1.0f, 1.0f,
	                            1.0f, 3.0f, 1.0f,
	                            1.0f, 1.0f, 3.0f));
}

TEST(TestMatrix3, MatrixMatrixSubtraction) {
	Matrix3x3 matrix = Matrix3x3::Identity();

	EXPECT_EQ((matrix - 1.0f) - matrix, Matrix3x3(-1.0f, -1.0f, -1.0f,
	                                              -1.0f, -1.0f, -1.0f,
	                                              -1.0f, -1.0f, -1.0f));


	matrix -= (matrix - 1.0f);
	EXPECT_EQ(matrix, Matrix3x3(1.0f, 1.0f, 1.0f,
	                            1.0f, 1.0f, 1.0f,
	                            1.0f, 1.0f, 1.0f));
}

TEST(TestMatrix3, Hadamard) {
	Matrix3x3 m1(1.0f, 2.0f, 3.0f,
	             4.0f, 5.0f, 6.0f,
	             7.0f, 8.0f, 9.0f);
	Matrix3x3 m2 = m1;

	EXPECT_EQ(Matrix3x3::Hadamard(m1, m2), Matrix3x3(1.0f, 4.0f, 9.0f,
	                                                 16.0f, 25.0f, 36.0f,
	                                                 49.0f, 64.0f, 81.0f));
}

TEST(TestMatrix3, Det) {
	Matrix3x3 m1(1.0f, 2.0f, 3.0f,
	             4.0f, 5.0f, 6.0f,
	             7.0f, 8.0f, 9.0f);

	Matrix3x3 m2(-23, -33, 4,
	             -4, -3, 6,
	             -7, 8, -1);

	EXPECT_EQ(m1.Det(), 0.0f);
	EXPECT_EQ((m1 + m1).Det(), 0.0f);
	EXPECT_EQ((Matrix3x3::Hadamard(m1, m1)).Det(), -216.0f);
	EXPECT_EQ(m2.Det(), 2341.0f);
}

TEST(TestMatrix3, Inverse) {
	Matrix3x3 matrix = Matrix3x3::Identity();

	EXPECT_EQ(matrix, matrix.Inverse());
	EXPECT_EQ(Matrix3x3(1.0f).Inverse(), Matrix3x3(0.0f));
	EXPECT_EQ((matrix + 1.0f).Inverse(), Matrix3x3(0.75f, -0.25f, -0.25f,
	                                               -0.25f, 0.75f, -0.25f,
	                                               -0.25f, -0.25f, 0.75f));
}

TEST(TestMatrix3, Transpose) {
	Matrix3x3 m(1.0f, 4.0f, 9.0f,
	            16.0f, 25.0f, 36.0f,
	            49.0f, 64.0f, 81.0f);

	EXPECT_EQ(m.Transpose(), Matrix3x3(1.0f, 16.0f, 49.0f,
	                                   4.0f, 25.0f, 64.0f,
	                                   9.0f, 36.0f, 81.0f));
}

TEST(TestMatrix3, MatrixVectorMultiply) {
	Matrix3x3 matrix = Matrix3x3::Identity();
	Vec3f vector(1.0f, 2.0f, 3.0f);

	EXPECT_EQ(matrix * vector, Vec3f(1.0f, 2.0f, 3.0f));
	EXPECT_EQ((matrix * 2.0f) * vector, Vec3f(2.0f, 4.0f, 6.0f));
	EXPECT_EQ((matrix * -1.0f) * vector, Vec3f(-1.0f, -2.0f, -3.0f));
	EXPECT_EQ((matrix + 1.0f) * vector, Vec3f(7.0f, 8.0f, 9.0f));

}

TEST(TestMatrix3, MatrixMatrixMultiply) {
	Matrix3x3 m1(1.0f, 2.0f, 3.0f,
	             4.0f, 5.0f, 6.0f,
	             7.0f, 8.0f, 9.0f);

	Matrix3x3 m2(13.0f, 14.0f, 15.0f,
	             17.0f, 83.0f, 32.0f,
	             3.0f, 15.0f, 7.0f);

	EXPECT_EQ(Matrix3x3::Identity() * Matrix3x3::Identity(), Matrix3x3::Identity());
	EXPECT_EQ(m1 * m2, Matrix3x3(56.0f, 225.0f, 100.0f,
	                             155.0f, 561.0f, 262.0f,
	                             254.0f, 897.0f, 424.0f));
}
