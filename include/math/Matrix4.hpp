#pragma once

#include "Angle.hpp"
#include "ZeroMath.hpp"

namespace Zero {

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

        bool operator==(const Matrix4& other) const;
        bool operator!=(const Matrix4& other) const;

        const float operator()(size_t row, size_t col) const;
        float operator()(size_t row, size_t col);

        const float* operator[](size_t index) const;
        float* operator[](size_t index);

        /* ********** Scalar Operations ********** */
        Matrix4 operator+(float scalar) const;
        Matrix4 operator-(float scalar) const;
        Matrix4 operator*(float scalar) const;
        Matrix4 operator/(float scalar) const;
        Matrix4& operator+=(float scalar);
        Matrix4& operator-=(float scalar);
        Matrix4& operator*=(float scalar);
        Matrix4& operator/=(float scalar);

        /* ********** Component-Wise Operations ********** */
        static Matrix4 Hadamard(const Matrix4& lhs, const Matrix4& rhs);
        Matrix4 operator+(const Matrix4& rhs) const;
        Matrix4 operator-(const Matrix4& rhs) const;
        Matrix4& operator+=(const Matrix4& rhs);
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
         * @brief Computes the result of a matrix4 - vector4 multiplication
         * @param rhs the vector4
         * @return the resulting vector4
         */
        Vector4 operator*(const Vector4& rhs) const;

        /**
         * @brief Computes the result of a matrix3 - matrix3 multiplication
         * @param rhs the right matrix3
         * @return the resulting matrix3
         */
        Matrix4 operator*(const Matrix4& rhs) const;

        /**
         * @return the identity matrix
         */
        static Matrix4 Identity();

        float matrix[4][4];

    }; // class Matrix4

} // namespace Zero