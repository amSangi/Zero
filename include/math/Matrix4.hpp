#pragma once

#include "Angle.hpp"
#include "ZMath.hpp"

namespace zero::math {

    /**
     * @brief A 4x4 Matrix
     */
    class Matrix4 {
    public:
        Matrix4() = default;
        explicit Matrix4(float m[4][4]);
        explicit Matrix4(float value);
        explicit Matrix4(const Matrix3& m3);
        Matrix4(float e00, float e01, float e02, float e03,
                float e10, float e11, float e12, float e13,
                float e20, float e21, float e22, float e23,
                float e30, float e31, float e32, float e33);

        ~Matrix4() = default;
        Matrix4& operator=(const Matrix4& other) = default;

        /**
         * @brief Check if the matrix4 is equal to another matrix4
         * @param other The other matrix4
         * @return True if the elements are equal. False otherwise.
         */
        bool operator==(const Matrix4& other) const;

        /**
         * @brief Check if the matrix4 is not equal to another matrix4
         * @param other The other matrix4
         * @return True if any of the elements are not equal. False otherwise.
         */
        bool operator!=(const Matrix4& other) const;

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
         * @return A new matrix4 containing the sums
         */
        Matrix4 operator+(float scalar) const;

        /**
         * @brief Subtract the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix4 containing the differences
         */
        Matrix4 operator-(float scalar) const;

        /**
         * @brief Multiply the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix4 containing the products
         */
        Matrix4 operator*(float scalar) const;

        /**
         * @brief Divide the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix4 containing the quotients
         */
        Matrix4 operator/(float scalar) const;

        /**
         * @brief Sum the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix4 containing the sums
         */
        Matrix4& operator+=(float scalar);

        /**
         * @brief Subtract the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix4 containing the differences
         */
        Matrix4& operator-=(float scalar);

        /**
         * @brief Multiply the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix4 containing the products
         */
        Matrix4& operator*=(float scalar);

        /**
         * @brief Divide the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix4 containing the quotients
         */
        Matrix4& operator/=(float scalar);

        /* ********** Component-Wise Operations ********** */

        /**
         * @brief Multiply each element of lhs with the same element of rhs
         * @param lhs The left matrix4
         * @param rhs The right matrix4
         * @return A new matrix containing the products
         */
        static Matrix4 Hadamard(const Matrix4& lhs, const Matrix4& rhs);

        /**
         * @brief Add the elements of this to the same elements of rhs
         * @param rhs The right matrix4
         * @return A new matrix4 containing the sums
         */
        Matrix4 operator+(const Matrix4& rhs) const;

        /**
         * @brief Subtract the elements of this by the same elements of rhs
         * @param rhs The right matrix4
         * @return A new matrix4 containing the differences
         */
        Matrix4 operator-(const Matrix4& rhs) const;

        /**
         * @brief Add the elements of this to the same elements of rhs
         * @param rhs The right matrix4
         * @return The same matrix4 containing the sums
         */
        Matrix4& operator+=(const Matrix4& rhs);

        /**
         * @brief Subtract the elements of this by the same elements of rhs
         * @param rhs The right matrix4
         * @return The same matrix4 containing the differences
         */
        Matrix4& operator-=(const Matrix4& rhs);

        /* ********** Matrix4 Operations ********** */
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
        bool InverseUtil(Matrix4& out, float epsilon=1e-05f) const;

        /**
         * @brief Computes the inverse matrix
         * @param epsilon the tolerance
         * @return the inverse matrix of this
         */
        Matrix4 Inverse(float epsilon=1e-05f) const;

        /**
         * @brief Computes the transpose matrix
         * @return the transpose matrix of this
         */
        Matrix4 Transpose() const;

        /**
         * @brief Computes the result of a matrix4 - vec4f multiplication
         * @param rhs the vec4f
         * @return the resulting vec4f
         */
        Vec4f operator*(const Vec4f& rhs) const;

        /**
         * @brief Computes the result of a matrix4 - matrix4 multiplication
         * @param rhs the right matrix4
         * @return the resulting matrix4
         */
        Matrix4 operator*(const Matrix4& rhs) const;

        /**
         * @return the identity matrix
         */
        static Matrix4 Identity();


        /**
         * @brief The doubly-nested array containing the matrix values
         */
        float matrix_[4][4];

    }; // class Matrix4

} // namespace zero