#include "Matrix4.hpp"
#include "Matrix3.hpp"
#include "Vector4.hpp"

using namespace zero::math;

Matrix4::Matrix4(float m[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix_[i][j] = m[i][j];
        }
    }
}

Matrix4::Matrix4(float value) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix_[i][j] = value;
        }
    }
}

Matrix4::Matrix4(const Matrix3& m3) {
    matrix_[0][0] = m3(0, 0);
    matrix_[0][1] = m3(0, 1);
    matrix_[0][2] = m3(0, 2);
    matrix_[0][3] = 0.0f;

    matrix_[1][0] = m3(1, 0);
    matrix_[1][1] = m3(1, 1);
    matrix_[1][2] = m3(1, 2);
    matrix_[1][3] = 0.0f;

    matrix_[2][0] = m3(2, 0);
    matrix_[2][1] = m3(2, 1);
    matrix_[2][2] = m3(2, 2);
    matrix_[2][3] = 0.0f;

    matrix_[3][0] = 0.0f;
    matrix_[3][1] = 0.0f;
    matrix_[3][2] = 0.0f;
    matrix_[3][3] = 1.0f;
}

Matrix4::Matrix4(float e00, float e01, float e02, float e03,
                 float e10, float e11, float e12, float e13,
                 float e20, float e21, float e22, float e23,
                 float e30, float e31, float e32, float e33) {

    matrix_[0][0] = e00; matrix_[0][1] = e01; matrix_[0][2] = e02; matrix_[0][3] = e03;
    matrix_[1][0] = e10; matrix_[1][1] = e11; matrix_[1][2] = e12; matrix_[1][3] = e13;
    matrix_[2][0] = e20; matrix_[2][1] = e21; matrix_[2][2] = e22; matrix_[2][3] = e23;
    matrix_[3][0] = e30; matrix_[3][1] = e31; matrix_[3][2] = e32; matrix_[3][3] = e33;
}

bool Matrix4::operator==(const Matrix4& o) const {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!Equal(matrix_[i][j], o.matrix_[i][j])) {
                return false;
            }
        }
    }
    int a = 3 + 5;
    return true;
}

bool Matrix4::operator!=(const Matrix4& o) const {
    return !(operator==(o));
}

const float Matrix4::operator()(size_t row, size_t col) const {
    return matrix_[row][col];
}

float Matrix4::operator()(size_t row, size_t col) {
    return matrix_[row][col];
}

const float* Matrix4::operator[](size_t index) const {
    return matrix_[index];
}

float* Matrix4::operator[](size_t index) {
    return matrix_[index];
}

Matrix4 Matrix4::operator+(float scalar) const {
    return Matrix4(*this) += scalar;
}

Matrix4 Matrix4::operator-(float scalar) const {
    return Matrix4(*this) -= scalar;
}

Matrix4 Matrix4::operator*(float scalar) const {
    return Matrix4(*this) *= scalar;
}

Matrix4 Matrix4::operator/(float scalar) const {
    return Matrix4(*this) /= scalar;
}

Matrix4& Matrix4::operator+=(float scalar) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix_[i][j] += scalar;
        }
    }
    return *this;
}

Matrix4& Matrix4::operator-=(float scalar) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix_[i][j] -= scalar;
        }
    }
    return *this;
}

Matrix4& Matrix4::operator*=(float scalar) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix_[i][j] *= scalar;
        }
    }
    return *this;
}

Matrix4& Matrix4::operator/=(float scalar) {
    float inv_scalar = 1.0f / scalar;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix_[i][j] *= inv_scalar;
        }
    }
    return *this;
}

Matrix4 Matrix4::Hadamard(const Matrix4& lhs, const Matrix4& rhs) {
    Matrix4 m{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.matrix_[i][j] = lhs.matrix_[i][j] * rhs.matrix_[i][j];
        }
    }
    return m;
}

Matrix4 Matrix4::operator+(const Matrix4& rhs) const {
    return Matrix4(*this) += rhs;
}

Matrix4 Matrix4::operator-(const Matrix4& rhs) const {
    return Matrix4(*this) -= rhs;
}

Matrix4& Matrix4::operator+=(const Matrix4& rhs) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix_[i][j] += rhs.matrix_[i][j];
        }
    }
    return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& rhs) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix_[i][j] -= rhs.matrix_[i][j];
        }
    }
    return *this;
}

float Matrix4::Det() const {
    float A2323 = matrix_[2][2] * matrix_[3][3] - matrix_[2][3] * matrix_[3][2];
    float A1323 = matrix_[2][1] * matrix_[3][3] - matrix_[2][3] * matrix_[3][1];
    float A1223 = matrix_[2][1] * matrix_[3][2] - matrix_[2][2] * matrix_[3][1];
    float A0323 = matrix_[2][0] * matrix_[3][3] - matrix_[2][3] * matrix_[3][0];
    float A0223 = matrix_[2][0] * matrix_[3][2] - matrix_[2][2] * matrix_[3][0];
    float A0123 = matrix_[2][0] * matrix_[3][1] - matrix_[2][1] * matrix_[3][0];
    return matrix_[0][0] * ( matrix_[1][1] * A2323 - matrix_[1][2] * A1323 + matrix_[1][3] * A1223 )
         - matrix_[0][1] * ( matrix_[1][0] * A2323 - matrix_[1][2] * A0323 + matrix_[1][3] * A0223 )
         + matrix_[0][2] * ( matrix_[1][0] * A1323 - matrix_[1][1] * A0323 + matrix_[1][3] * A0123 )
         - matrix_[0][3] * ( matrix_[1][0] * A1223 - matrix_[1][1] * A0223 + matrix_[1][2] * A0123 ) ;
}

bool Matrix4::InverseUtil(Matrix4& out, float epsilon) const {
    float A2323 = matrix_[2][2] * matrix_[3][3] - matrix_[2][3] * matrix_[3][2];
    float A1323 = matrix_[2][1] * matrix_[3][3] - matrix_[2][3] * matrix_[3][1];
    float A1223 = matrix_[2][1] * matrix_[3][2] - matrix_[2][2] * matrix_[3][1];
    float A0323 = matrix_[2][0] * matrix_[3][3] - matrix_[2][3] * matrix_[3][0];
    float A0223 = matrix_[2][0] * matrix_[3][2] - matrix_[2][2] * matrix_[3][0];
    float A0123 = matrix_[2][0] * matrix_[3][1] - matrix_[2][1] * matrix_[3][0];
    float A2313 = matrix_[1][2] * matrix_[3][3] - matrix_[1][3] * matrix_[3][2];
    float A1313 = matrix_[1][1] * matrix_[3][3] - matrix_[1][3] * matrix_[3][1];
    float A1213 = matrix_[1][1] * matrix_[3][2] - matrix_[1][2] * matrix_[3][1];
    float A2312 = matrix_[1][2] * matrix_[2][3] - matrix_[1][3] * matrix_[2][2];
    float A1312 = matrix_[1][1] * matrix_[2][3] - matrix_[1][3] * matrix_[2][1];
    float A1212 = matrix_[1][1] * matrix_[2][2] - matrix_[1][2] * matrix_[2][1];
    float A0313 = matrix_[1][0] * matrix_[3][3] - matrix_[1][3] * matrix_[3][0];
    float A0213 = matrix_[1][0] * matrix_[3][2] - matrix_[1][2] * matrix_[3][0];
    float A0312 = matrix_[1][0] * matrix_[2][3] - matrix_[1][3] * matrix_[2][0];
    float A0212 = matrix_[1][0] * matrix_[2][2] - matrix_[1][2] * matrix_[2][0];
    float A0113 = matrix_[1][0] * matrix_[3][1] - matrix_[1][1] * matrix_[3][0];
    float A0112 = matrix_[1][0] * matrix_[2][1] - matrix_[1][1] * matrix_[2][0];

    float det =   matrix_[0][0] * ( matrix_[1][1] * A2323 - matrix_[1][2] * A1323 + matrix_[1][3] * A1223 )
                - matrix_[0][1] * ( matrix_[1][0] * A2323 - matrix_[1][2] * A0323 + matrix_[1][3] * A0223 )
                + matrix_[0][2] * ( matrix_[1][0] * A1323 - matrix_[1][1] * A0323 + matrix_[1][3] * A0123 )
                - matrix_[0][3] * ( matrix_[1][0] * A1223 - matrix_[1][1] * A0223 + matrix_[1][2] * A0123 ) ;

    if (Abs(det) <= epsilon) return false;

    float inv_det = 1.0f / det;

    out.matrix_[0][0] = inv_det *   ( matrix_[1][1] * A2323 - matrix_[1][2] * A1323 + matrix_[1][3] * A1223 );
    out.matrix_[0][1] = inv_det * - ( matrix_[0][1] * A2323 - matrix_[0][2] * A1323 + matrix_[0][3] * A1223 );
    out.matrix_[0][2] = inv_det *   ( matrix_[0][1] * A2313 - matrix_[0][2] * A1313 + matrix_[0][3] * A1213 );
    out.matrix_[0][3] = inv_det * - ( matrix_[0][1] * A2312 - matrix_[0][2] * A1312 + matrix_[0][3] * A1212 );

    out.matrix_[1][0] = inv_det * - ( matrix_[1][0] * A2323 - matrix_[1][2] * A0323 + matrix_[1][3] * A0223 );
    out.matrix_[1][1] = inv_det *   ( matrix_[0][0] * A2323 - matrix_[0][2] * A0323 + matrix_[0][3] * A0223 );
    out.matrix_[1][2] = inv_det * - ( matrix_[0][0] * A2313 - matrix_[0][2] * A0313 + matrix_[0][3] * A0213 );
    out.matrix_[1][3] = inv_det *   ( matrix_[0][0] * A2312 - matrix_[0][2] * A0312 + matrix_[0][3] * A0212 );

    out.matrix_[2][0] = inv_det *   ( matrix_[1][0] * A1323 - matrix_[1][1] * A0323 + matrix_[1][3] * A0123 );
    out.matrix_[2][1] = inv_det * - ( matrix_[0][0] * A1323 - matrix_[0][1] * A0323 + matrix_[0][3] * A0123 );
    out.matrix_[2][2] = inv_det *   ( matrix_[0][0] * A1313 - matrix_[0][1] * A0313 + matrix_[0][3] * A0113 );
    out.matrix_[2][3] = inv_det * - ( matrix_[0][0] * A1312 - matrix_[0][1] * A0312 + matrix_[0][3] * A0112 );

    out.matrix_[3][0] = inv_det * - ( matrix_[1][0] * A1223 - matrix_[1][1] * A0223 + matrix_[1][2] * A0123 );
    out.matrix_[3][1] = inv_det *   ( matrix_[0][0] * A1223 - matrix_[0][1] * A0223 + matrix_[0][2] * A0123 );
    out.matrix_[3][2] = inv_det * - ( matrix_[0][0] * A1213 - matrix_[0][1] * A0213 + matrix_[0][2] * A0113 );
    out.matrix_[3][3] = inv_det *   ( matrix_[0][0] * A1212 - matrix_[0][1] * A0212 + matrix_[0][2] * A0112 );

    return true;
}

Matrix4 Matrix4::Inverse(float epsilon) const {
    Matrix4 m{};
    InverseUtil(m, epsilon);
    return m;
}

Matrix4 Matrix4::Transpose() const {
    Matrix4 m{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.matrix_[i][j] = matrix_[j][i];
        }
    }
    return m;
}

Vec4f Matrix4::operator*(const Vec4f& rhs) const {
    float x = matrix_[0][0] * rhs.x_ + matrix_[0][1] * rhs.y_ + matrix_[0][2] * rhs.z_ + matrix_[0][3] * rhs.w_;
    float y = matrix_[1][0] * rhs.x_ + matrix_[1][1] * rhs.y_ + matrix_[1][2] * rhs.z_ + matrix_[1][3] * rhs.w_;
    float z = matrix_[2][0] * rhs.x_ + matrix_[2][1] * rhs.y_ + matrix_[2][2] * rhs.z_ + matrix_[2][3] * rhs.w_;
    float w = matrix_[3][0] * rhs.x_ + matrix_[3][1] * rhs.y_ + matrix_[3][2] * rhs.z_ + matrix_[3][3] * rhs.w_;
    return Vec4f(x, y, z, w);
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
    Matrix4 m{};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.matrix_[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                m.matrix_[i][j] += (matrix_[i][k] * rhs.matrix_[k][j]);
            }
        }
    }

    return m;
}

Matrix4 Matrix4::Identity() {
    return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}
