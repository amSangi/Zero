#include "gtest/gtest.h"
#include "math/Matrix4x4.hpp"
#include "math/Vector4.hpp"

using namespace zero::math;

TEST(TestMatrix4, ScalarOperations) {
	Matrix4x4 matrix = Matrix4x4::Identity();

	EXPECT_EQ(Matrix4x4(2.0f, 1.0f, 1.0f, 1.0f,
	                    1.0f, 2.0f, 1.0f, 1.0f,
	                    1.0f, 1.0f, 2.0f, 1.0f,
	                    1.0f, 1.0f, 1.0f, 2.0f), matrix + 1.0f);

	EXPECT_EQ(Matrix4x4(0.0f, -1.0f, -1.0f, -1.0f,
	                    -1.0f, 0.0f, -1.0f, -1.0f,
	                    -1.0f, -1.0f, 0.0f, -1.0f,
	                    -1.0f, -1.0f, -1.0f, 0.0f), matrix - 1.0f);

	EXPECT_EQ(Matrix4x4(-2.0f, -1.0f, -1.0f, -1.0f,
	                    -1.0f, -2.0f, -1.0f, -1.0f,
	                    -1.0f, -1.0f, -2.0f, -1.0f,
	                    -1.0f, -1.0f, -1.0f, -2.0f), (matrix + 1.0f) * -1.0f);

	EXPECT_EQ(Matrix4x4(1.0f, 0.5f, 0.5f, 0.5f,
	                    0.5f, 1.0f, 0.5f, 0.5f,
	                    0.5f, 0.5f, 1.0f, 0.5f,
	                    0.5f, 0.5f, 0.5f, 1.0f), (matrix + 1.0f) / 2.0f);


	matrix += 1.0f;
	EXPECT_EQ(Matrix4x4(2.0f, 1.0f, 1.0f, 1.0f,
	                    1.0f, 2.0f, 1.0f, 1.0f,
	                    1.0f, 1.0f, 2.0f, 1.0f,
	                    1.0f, 1.0f, 1.0f, 2.0f), matrix);

	matrix -= 4.0f;
	EXPECT_EQ(Matrix4x4(-2.0f, -3.0f, -3.0f, -3.0f,
	                    -3.0f, -2.0f, -3.0f, -3.0f,
	                    -3.0f, -3.0f, -2.0f, -3.0f,
	                    -3.0f, -3.0f, -3.0f, -2.0f), matrix);

	matrix *= 10.0f;
	EXPECT_EQ(Matrix4x4(-20.0f, -30.0f, -30.0f, -30.0f,
	                    -30.0f, -20.0f, -30.0f, -30.0f,
	                    -30.0f, -30.0f, -20.0f, -30.0f,
	                    -30.0f, -30.0f, -30.0f, -20.0f), matrix);

	matrix /= -1.0f;
	EXPECT_EQ(Matrix4x4(20.0f, 30.0f, 30.0f, 30.0f,
	                    30.0f, 20.0f, 30.0f, 30.0f,
	                    30.0f, 30.0f, 20.0f, 30.0f,
	                    30.0f, 30.0f, 30.0f, 20.0f), matrix);
}

TEST(TestMatrix4, MatrixMatrixAddition) {
	Matrix4x4 matrix = Matrix4x4::Identity();

	EXPECT_EQ(Matrix4x4(3.0f, 1.0f, 1.0f, 1.0f,
	                    1.0f, 3.0f, 1.0f, 1.0f,
	                    1.0f, 1.0f, 3.0f, 1.0f,
	                    1.0f, 1.0f, 1.0f, 3.0f), (matrix + 1.0f) + matrix);

	matrix += (matrix + 1.0f);
	EXPECT_EQ(Matrix4x4(3.0f, 1.0f, 1.0f, 1.0f,
	                    1.0f, 3.0f, 1.0f, 1.0f,
	                    1.0f, 1.0f, 3.0f, 1.0f,
	                    1.0f, 1.0f, 1.0f, 3.0f), matrix);
}

TEST(TestMatrix4, MatrixMatrixSubtraction) {
	Matrix4x4 matrix = Matrix4x4::Identity();

	EXPECT_EQ(Matrix4x4(-1.0f, -1.0f, -1.0f, -1.0f,
	                    -1.0f, -1.0f, -1.0f, -1.0f,
	                    -1.0f, -1.0f, -1.0f, -1.0f,
	                    -1.0f, -1.0f, -1.0f, -1.0f), (matrix - 1.0f) - matrix);


	matrix -= (matrix - 1.0f);
	EXPECT_EQ(Matrix4x4(1.0f, 1.0f, 1.0f, 1.0f,
	                    1.0f, 1.0f, 1.0f, 1.0f,
	                    1.0f, 1.0f, 1.0f, 1.0f,
	                    1.0f, 1.0f, 1.0f, 1.0f), matrix);
}

TEST(TestMatrix4, Hadamard) {
	Matrix4x4 m1(1.0f, 2.0f, 3.0f, 3.0f,
	             4.0f, 5.0f, 6.0f, 6.0f,
	             7.0f, 8.0f, 9.0f, 9.0f,
	             10.0f, 11.0f, 12.0f, 13.0f);
	Matrix4x4 m2 = m1;

	EXPECT_EQ(Matrix4x4(1.0f, 4.0f, 9.0f, 9.0f,
	                    16.0f, 25.0f, 36.0f, 36.0f,
	                    49.0f, 64.0f, 81.0f, 81.0f,
	                    100.0f, 121.0f, 144.0f, 169.0f), Matrix4x4::Hadamard(m1, m2));
}

TEST(TestMatrix4, Det) {
	Matrix4x4 m1(1.0f, 2.0f, 3.0f, 3.0f,
	             4.0f, 5.0f, 6.0f, 6.0f,
	             7.0f, 8.0f, 9.0f, 9.0f,
	             10.0f, 11.0f, 12.0f, 13.0f);

	Matrix4x4 m2(-23, -33, 4, 3,
	             -4, -3, 6, 5,
	             -7, 8, -1, 6,
	             8, -9, 1, -2);

	EXPECT_EQ(0.0f, m1.Det());
	EXPECT_EQ(0.0f, (m1 + m1).Det());
	EXPECT_EQ(-5400.0f, (Matrix4x4::Hadamard(m1, m1)).Det());
	EXPECT_EQ(11469.0f, m2.Det());
}

TEST(TestMatrix4, Inverse) {
	Matrix4x4 matrix = Matrix4x4::Identity();

	EXPECT_EQ(matrix, matrix.Inverse());
	EXPECT_EQ(Matrix4x4(0.0f), Matrix4x4(1.0f).Inverse());
	EXPECT_EQ(Matrix4x4(0.8f, -0.2f, -0.2f, -0.2f,
	                    -0.2f, 0.8f, -0.2f, -0.2f,
	                    -0.2f, -0.2f, 0.8f, -0.2f,
	                    -0.2f, -0.2f, -0.2f, 0.8f), (matrix + 1.0f).Inverse());
}

TEST(TestMatrix4, Transpose) {
	Matrix4x4 m(1.0f, 4.0f, 9.0f, 9.0f,
	            16.0f, 25.0f, 36.0f, 4.0f,
	            49.0f, 64.0f, 81.0f, 81.0f,
	            49.0f, 64.0f, 81.0f, 144.0f);

	EXPECT_EQ(Matrix4x4(1.0f, 16.0f, 49.0f, 49.0f,
	                    4.0, 25.0, 64.0f, 64.0f,
	                    9.0f, 36.0f, 81.0f, 81.0f,
	                    9.0f, 4.0f, 81.0f, 144.0f), m.Transpose());
}

TEST(TestMatrix4, MatrixVectorMultiply) {
	Matrix4x4 matrix = Matrix4x4::Identity();
	Vec4f vector(1.0f, 2.0f, 3.0f, 4.0f);

	EXPECT_EQ(Vec4f(1.0f, 2.0f, 3.0f, 4.0f), matrix * vector);
	EXPECT_EQ(Vec4f(2.0f, 4.0f, 6.0f, 8.0f), (matrix * 2.0f) * vector);
	EXPECT_EQ(Vec4f(-1.0f, -2.0f, -3.0f, -4.0f), (matrix * -1.0f) * vector);
	EXPECT_EQ(Vec4f(11.0f, 12.0f, 13.0f, 14.0f), (matrix + 1.0f) * vector);
}

TEST(TestMatrix4, MatrixMatrixMultiply) {
	Matrix4x4 m1(1.0f, 2.0f, 3.0f, 3.0f,
	             4.0f, 5.0f, 6.0f, 6.0f,
	             7.0f, 8.0f, 9.0f, 10.0f,
	             11.0f, 12.0f, 13.0f, 14.0f);

	Matrix4x4 m2(13.0f, 14.0f, 15.0f, 11.0f,
	             17.0f, 83.0f, 32.0f, 3.0f,
	             3.0f, 15.0f, 7.0f, 5.0f,
	             17.0f, 14.0f, 13.0f, 5.0f);

	EXPECT_EQ(Matrix4x4::Identity(), Matrix4x4::Identity() * Matrix4x4::Identity());
	EXPECT_EQ(Matrix4x4(107.0f, 267.0f, 139.0f, 47.0f,
	                    257.0f, 645.0f, 340.0f, 119.0f,
	                    424.0f, 1037.0f, 554.0f, 196.0f,
	                    624.0f, 1541.0f, 822.0f, 292.0f), m1 * m2);
}
