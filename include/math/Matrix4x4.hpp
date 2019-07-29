#pragma once

#include "Angle.hpp"
#include "ZMath.hpp"
#include "Matrix3x3.hpp"

namespace zero::math {

    /**
     * @brief A 4x4 Matrix
     */
    class Matrix4x4 {
    public:
        Matrix4x4() = default;
        explicit Matrix4x4(float m[4][4]);
        explicit Matrix4x4(float value);
        explicit Matrix4x4(const Matrix3x3& m3);
        Matrix4x4(float e00, float e01, float e02, float e03,
                  float e10, float e11, float e12, float e13,
                  float e20, float e21, float e22, float e23,
                  float e30, float e31, float e32, float e33);

        ~Matrix4x4() = default;
        Matrix4x4& operator=(const Matrix4x4& other) = default;

        /**
         * @brief Check if the matrix4x4 is equal to another matrix4
         * @param other The other matrix4x4
         * @return True if the elements are equal. False otherwise.
         */
        bool operator==(const Matrix4x4& other) const;

        /**
         * @brief Check if the matrix4x4 is not equal to another matrix4
         * @param other The other matrix4x4
         * @return True if any of the elements are not equal. False otherwise.
         */
        bool operator!=(const Matrix4x4& other) const;

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
         * @return A new matrix4x4 containing the sums
         */
        Matrix4x4 operator+(float scalar) const;

        /**
         * @brief Subtract the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix4x4 containing the differences
         */
        Matrix4x4 operator-(float scalar) const;

        /**
         * @brief Multiply the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix4x4 containing the products
         */
        Matrix4x4 operator*(float scalar) const;

        /**
         * @brief Divide the elements by the given scalar
         * @param scalar The scalar
         * @return A new matrix4x4 containing the quotients
         */
        Matrix4x4 operator/(float scalar) const;

        /**
         * @brief Sum the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix4x4 containing the sums
         */
        Matrix4x4& operator+=(float scalar);

        /**
         * @brief Subtract the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix4x4 containing the differences
         */
        Matrix4x4& operator-=(float scalar);

        /**
         * @brief Multiply the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix4x4 containing the products
         */
        Matrix4x4& operator*=(float scalar);

        /**
         * @brief Divide the elements by the given scalar
         * @param scalar The scalar
         * @return The same matrix4x4 containing the quotients
         */
        Matrix4x4& operator/=(float scalar);

        /* ********** Component-Wise Operations ********** */

        /**
         * @brief Multiply each element of lhs with the same element of rhs
         * @param lhs The left matrix4x4
         * @param rhs The right matrix4x4
         * @return A new matrix containing the products
         */
        static Matrix4x4 Hadamard(const Matrix4x4& lhs, const Matrix4x4& rhs);

        /**
         * @brief Add the elements of this to the same elements of rhs
         * @param rhs The right matrix4
         * @return A new matrix4x4 containing the sums
         */
        Matrix4x4 operator+(const Matrix4x4& rhs) const;

        /**
         * @brief Subtract the elements of this by the same elements of rhs
         * @param rhs The right matrix4x4
         * @return A new matrix4 containing the differences
         */
        Matrix4x4 operator-(const Matrix4x4& rhs) const;

        /**
         * @brief Add the elements of this to the same elements of rhs
         * @param rhs The right matrix4x4
         * @return The same matrix4x4 containing the sums
         */
        Matrix4x4& operator+=(const Matrix4x4& rhs);

        /**
         * @brief Subtract the elements of this by the same elements of rhs
         * @param rhs The right matrix4x4
         * @return The same matrix4x4 containing the differences
         */
        Matrix4x4& operator-=(const Matrix4x4& rhs);

        /* ********** Matrix4x4 Operations ********** */
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
        bool InverseUtil(Matrix4x4& out, float epsilon=1e-05f) const;

        /**
         * @brief Computes the inverse matrix
         * @param epsilon the tolerance
         * @return the inverse matrix of this
         */
        Matrix4x4 Inverse(float epsilon=1e-05f) const;

        /**
         * @brief Computes the transpose matrix
         * @return the transpose matrix of this
         */
        Matrix4x4 Transpose() const;

        /**
         * @brief Computes the result of a matrix4x4 - vec4f multiplication
         * @param rhs the vec4f
         * @return the resulting vec4f
         */
        Vec4f operator*(const Vec4f& rhs) const;

        /**
         * @brief Computes the result of a matrix4x4 - matrix4x4 multiplication
         * @param rhs the right matrix4x4
         * @return the resulting matrix4x4
         */
        Matrix4x4 operator*(const Matrix4x4& rhs) const;

        /**
         * @brief Apply a translation to the matrix
         * @param translation the translation
         */
        Matrix4x4& Translate(const Vec3f& translation);

        /**
         * @brief Apply a rotation to the matrix
         * @param rotation the rotation
         */
        Matrix4x4& Rotate(const Quaternion& rotation);

        /**
         * @brief Apply a scale to the matrix
         * @param scale the scale
         */
        Matrix4x4& Scale(const Vec3f& scale);

        /**
         * @brief Get the translation component of the matrix
         * @return the translation in 3D space
         */
        Vec3f GetTranslation() const;

        /**
         * @brief Get the rotation component of the matrix
         * @return the rotation
         */
        Quaternion GetRotation() const;

        /**
         * @brief Get the scale component of the matrix. Does not support negative scaling. 
         * @return the scale
         */
        Vec3f GetScale() const;

        /**
         * @brief Get the Matrix3x3 containing the scale and orientation components
         * @return a matrix3x3
         */
        Matrix3x3 GetMatrix3x3() const;

        /**
         * @return the identity matrix
         */
        static Matrix4x4 Identity();


        /**
         * @brief The doubly-nested array containing the matrix values
         */
        float matrix_[4][4];

    }; // class Matrix4x4

} // namespace zero::math