#pragma once

#include "Angle.h"
#include "ZeroMath.h"

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
        Matrix3& operator=(const Matrix3& o) = default;

        bool operator==(const Matrix3& o) const;
        bool operator!=(const Matrix3& o) const;

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
        float Det() const;
        bool InverseUtil(Matrix3& out, float epsilon=1e-05f) const;
        Matrix3 Inverse(float epsilon=1e-05f) const;
        Matrix3 Transpose() const;
        Vector3 operator*(const Vector3& rhs) const;
        Matrix3 operator*(const Matrix3& rhs) const;

        static Matrix3 Identity();

        float matrix[3][3];

    }; // class Matrix3

} // namespace Zero