#pragma once

#include "Angle.hpp"
#include "ZMath.hpp"

namespace Zero {

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

        bool operator==(const Matrix3& other) const;
        bool operator!=(const Matrix3& other) const;

        const float operator()(size_t row, size_t col) const;
        float operator()(size_t row, size_t col);

        const float* operator[](size_t index) const;
        float* operator[](size_t index);

        /* ********** Scalar Operations ********** */
        Matrix3 operator+(float scalar) const;
        Matrix3 operator-(float scalar) const;
        Matrix3 operator*(float scalar) const;
        Matrix3 operator/(float scalar) const;
        Matrix3& operator+=(float scalar);
        Matrix3& operator-=(float scalar);
        Matrix3& operator*=(float scalar);
        Matrix3& operator/=(float scalar);

        /* ********** Component-Wise Operations ********** */

        static Matrix3 Hadamard(const Matrix3& lhs, const Matrix3& rhs);
        Matrix3 operator+(const Matrix3& rhs) const;
        Matrix3 operator-(const Matrix3& rhs) const;
        Matrix3& operator+=(const Matrix3& rhs);
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

        float matrix_[3][3];

    }; // class Matrix3

} // namespace Zero