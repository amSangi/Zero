#pragma once

#include "Angle.h"
#include "ZeroMath.h"

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
        Matrix4& operator=(const Matrix4& o) = default;

        bool operator==(const Matrix4& o) const;
        bool operator!=(const Matrix4& o) const;

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
        float Det() const;
        bool InverseUtil(Matrix4& out, float epsilon=1e-05f) const;
        Matrix4 Inverse(float epsilon=1e-05f) const;
        Matrix4 Transpose() const;
        Vector4 operator*(const Vector4& rhs) const;
        Matrix4 operator*(const Matrix4& rhs) const;

        static Matrix4 Identity();
        static Matrix4 Identity3();

        float matrix[4][4];

    }; // class Matrix4

} // namespace Zero