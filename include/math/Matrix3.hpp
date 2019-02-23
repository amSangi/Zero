#pragma once

#include "Angle.hpp"
#include "ZMath.hpp"

namespace zero {
namespace math {

    /**
     * @brief A 3x3 Matrix
     */
    class Matrix3 {
    public:
        Matrix3() = default;
        explicit Matrix3(float m[3][3]);
        explicit Matrix3(float value);
        Matrix3(float e00, float e01, float e02,
                float e10, float e11, float e12,
                float e20, float e21, float e22);

        ~Matrix3() = default;
        Matrix3& operator=(const Matrix3& other) = default;

        /**
         * @brief Check if the matrix3 is equal to another matrix3
         * @param other The other matrix3
         * @return True if the elements are equal. False otherwise.
         */
        bool operator==(const Matrix3& other) const;

        /**
         * @brief Check if the matrix3 is not equal to another matrix3
         * @param other The other matrix3
         * @return True if any of the elements are not equal False otherwise.
         */
        bool operator!=(const Matrix3& other) const;

        /**
         * @brief Retrieve the element at the given row and column
         * @param row The 0-indexed row
         * @param col The 0-indexed column
         * @return The element at the given row and column
         */
        const float operator()(size_t row, size_t col) const;

        /**
         * @brief Retrieve the element at the given row and column
         * @param row The 0-indexed row
         * @param col The 0-indexed column
         * @return The element at the given row and column
         */
        float operator()(size_t row, size_t col);

        /**
         * @brief Retrieve the row at the given row index
         * @param index The 0-indexed row
         * @return The row elements
         */
        const float* operator[](size_t index) const;

        /**
         * @brief Retrieve the row at the given row index
         * @param index The 0-indexed row
         * @return The row elements
         */
        float* operator[](size_t index);

        /* ********** Scalar Operations ********** */

        /**
         * @brief Sum the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix3 containing the sums
         */
        Matrix3 operator+(float scalar) const;

        /**
         * @brief Subtract the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix3 containing the differences
         */
        Matrix3 operator-(float scalar) const;

        /**
         * @brief Multiply the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix3 containing the products
         */
        Matrix3 operator*(float scalar) const;

        /**
         * @brief Divide the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix3 containing the quotients
         */
        Matrix3 operator/(float scalar) const;

        /**
         * @brief Sum the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix3 containing the sums
         */
        Matrix3& operator+=(float scalar);

        /**
         * @brief Subtract the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix3 containing the differences
         */
        Matrix3& operator-=(float scalar);

        /**
         * @brief Multiply the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix3 containing the products
         */
        Matrix3& operator*=(float scalar);

        /**
         * @brief Divide the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix3 containing the quotients
         */
        Matrix3& operator/=(float scalar);

        /* ********** Component-Wise Operations ********** */

        /**
         * @brief Multiply each element of lhs with the same element of rhs
         * @param lhs The left matrix3
         * @param rhs The right matrix3
         * @return A new matrix containing the products
         */
        static Matrix3 Hadamard(const Matrix3& lhs, const Matrix3& rhs);

        /**
         * @brief Add the elements of this to the same elements of rhs
         * @param rhs The right matrix3
         * @return A new matrix3 containing the sums
         */
        Matrix3 operator+(const Matrix3& rhs) const;

        /**
         * @brief Subtract the elements of this by the same elements of rhs
         * @param rhs The right matrix3
         * @return A new matrix3 containing the differences
         */
        Matrix3 operator-(const Matrix3& rhs) const;

        /**
         * @brief Add the elements of this to the same elements of rhs
         * @param rhs The right matrix3
         * @return The same matrix3 containing the sums
         */
        Matrix3& operator+=(const Matrix3& rhs);

        /**
         * @brief Subtract the elements of this by the same elements of rhs
         * @param rhs The right matrix3
         * @return The same matrix3 containing the differences
         */
        Matrix3& operator-=(const Matrix3& rhs);

        /* ********** Matrix3 Operations ********** */

        /**
         * @return the determinant of the matrix
         */
        float Det() const;

        /**
         * @brief Computes the inverse of the matrix and stores it in out.
         * @param out the resulting inverse matrix
         * @param epsilon the tolerance
         * @return True if the det is not 0. Otherwise false.
         */
        bool InverseUtil(Matrix3& out, float epsilon=1e-05f) const;

        /**
         * @brief Computes the inverse matrix
         * @param epsilon the tolerance
         * @return the inverse matrix of this
         */
        Matrix3 Inverse(float epsilon=1e-05f) const;

        /**
         * @brief Computes the transpose matrix
         * @return the transpose matrix of this
         */
        Matrix3 Transpose() const;

        /**
         * @brief Computes the result of a matrix3 - vector3 multiplication
         * @param rhs the vector3
         * @return the resulting vector3
         */
        Vector3 operator*(const Vector3& rhs) const;

        /**
         * @brief Computes the result of a matrix3 - matrix3 multiplication
         * @param rhs the right matrix3
         * @return the resulting matrix3
         */
        Matrix3 operator*(const Matrix3& rhs) const;

        /**
         * @return the identity matrix
         */
        static Matrix3 Identity();

        /**
         * @brief The doubly-nested array containing the matrix values
         */
        float matrix_[3][3];

    }; // class Matrix3

} // namespace math
} // namespace zero