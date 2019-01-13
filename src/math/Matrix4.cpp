#include "Matrix4.hpp"
#include "Matrix3.hpp"
#include "Vector4.hpp"

using namespace Zero;

Matrix4::Matrix4(float m[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] = m[i][j];
        }
    }
}

Matrix4::Matrix4(float value) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] = value;
        }
    }
}

Matrix4::Matrix4(const Matrix3& m3) {
    matrix[0][0] = m3(0, 0);
    matrix[0][1] = m3(0, 1);
    matrix[0][2] = m3(0, 2);
    matrix[0][3] = 0.0f;

    matrix[1][0] = m3(1, 0);
    matrix[1][1] = m3(1, 1);
    matrix[1][2] = m3(1, 2);
    matrix[1][3] = 0.0f;

    matrix[2][0] = m3(2, 0);
    matrix[2][1] = m3(2, 1);
    matrix[2][2] = m3(2, 2);
    matrix[2][3] = 0.0f;

    matrix[3][0] = 0.0f;
    matrix[3][1] = 0.0f;
    matrix[3][2] = 0.0f;
    matrix[3][3] = 1.0f;
}

Matrix4::Matrix4(float e00, float e01, float e02, float e03,
                 float e10, float e11, float e12, float e13,
                 float e20, float e21, float e22, float e23,
                 float e30, float e31, float e32, float e33) {

    matrix[0][0] = e00; matrix[0][1] = e01; matrix[0][2] = e02; matrix[0][3] = e03;
    matrix[1][0] = e10; matrix[1][1] = e11; matrix[1][2] = e12; matrix[1][3] = e13;
    matrix[2][0] = e20; matrix[2][1] = e21; matrix[2][2] = e22; matrix[2][3] = e23;
    matrix[3][0] = e30; matrix[3][1] = e31; matrix[3][2] = e32; matrix[3][3] = e33;
}

bool Matrix4::operator==(const Matrix4& o) const {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!Equal(matrix[i][j], o.matrix[i][j])) {
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
    return matrix[row][col];
}

float Matrix4::operator()(size_t row, size_t col) {
    return matrix[row][col];
}

const float* Matrix4::operator[](size_t index) const {
    return matrix[index];
}

float* Matrix4::operator[](size_t index) {
    return matrix[index];
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
            matrix[i][j] += scalar;
        }
    }
    return *this;
}

Matrix4& Matrix4::operator-=(float scalar) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] -= scalar;
        }
    }
    return *this;
}

Matrix4& Matrix4::operator*=(float scalar) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] *= scalar;
        }
    }
    return *this;
}

Matrix4& Matrix4::operator/=(float scalar) {
    float inv_scalar = 1.0f / scalar;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] *= inv_scalar;
        }
    }
    return *this;
}

Matrix4 Matrix4::Hadamard(const Matrix4& lhs, const Matrix4& rhs) {
    Matrix4 m{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.matrix[i][j] = lhs.matrix[i][j] * rhs.matrix[i][j];
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
            matrix[i][j] += rhs.matrix[i][j];
        }
    }
    return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& rhs) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] -= rhs.matrix[i][j];
        }
    }
    return *this;
}

float Matrix4::Det() const {
    float A2323 = matrix[2][2] * matrix[3][3] - matrix[2][3] * matrix[3][2];
    float A1323 = matrix[2][1] * matrix[3][3] - matrix[2][3] * matrix[3][1];
    float A1223 = matrix[2][1] * matrix[3][2] - matrix[2][2] * matrix[3][1];
    float A0323 = matrix[2][0] * matrix[3][3] - matrix[2][3] * matrix[3][0];
    float A0223 = matrix[2][0] * matrix[3][2] - matrix[2][2] * matrix[3][0];
    float A0123 = matrix[2][0] * matrix[3][1] - matrix[2][1] * matrix[3][0];
    return matrix[0][0] * ( matrix[1][1] * A2323 - matrix[1][2] * A1323 + matrix[1][3] * A1223 )
         - matrix[0][1] * ( matrix[1][0] * A2323 - matrix[1][2] * A0323 + matrix[1][3] * A0223 )
         + matrix[0][2] * ( matrix[1][0] * A1323 - matrix[1][1] * A0323 + matrix[1][3] * A0123 )
         - matrix[0][3] * ( matrix[1][0] * A1223 - matrix[1][1] * A0223 + matrix[1][2] * A0123 ) ;
}

bool Matrix4::InverseUtil(Matrix4& out, float epsilon) const {
    float A2323 = matrix[2][2] * matrix[3][3] - matrix[2][3] * matrix[3][2];
    float A1323 = matrix[2][1] * matrix[3][3] - matrix[2][3] * matrix[3][1];
    float A1223 = matrix[2][1] * matrix[3][2] - matrix[2][2] * matrix[3][1];
    float A0323 = matrix[2][0] * matrix[3][3] - matrix[2][3] * matrix[3][0];
    float A0223 = matrix[2][0] * matrix[3][2] - matrix[2][2] * matrix[3][0];
    float A0123 = matrix[2][0] * matrix[3][1] - matrix[2][1] * matrix[3][0];
    float A2313 = matrix[1][2] * matrix[3][3] - matrix[1][3] * matrix[3][2];
    float A1313 = matrix[1][1] * matrix[3][3] - matrix[1][3] * matrix[3][1];
    float A1213 = matrix[1][1] * matrix[3][2] - matrix[1][2] * matrix[3][1];
    float A2312 = matrix[1][2] * matrix[2][3] - matrix[1][3] * matrix[2][2];
    float A1312 = matrix[1][1] * matrix[2][3] - matrix[1][3] * matrix[2][1];
    float A1212 = matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1];
    float A0313 = matrix[1][0] * matrix[3][3] - matrix[1][3] * matrix[3][0];
    float A0213 = matrix[1][0] * matrix[3][2] - matrix[1][2] * matrix[3][0];
    float A0312 = matrix[1][0] * matrix[2][3] - matrix[1][3] * matrix[2][0];
    float A0212 = matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0];
    float A0113 = matrix[1][0] * matrix[3][1] - matrix[1][1] * matrix[3][0];
    float A0112 = matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0];

    float det =   matrix[0][0] * ( matrix[1][1] * A2323 - matrix[1][2] * A1323 + matrix[1][3] * A1223 )
                - matrix[0][1] * ( matrix[1][0] * A2323 - matrix[1][2] * A0323 + matrix[1][3] * A0223 )
                + matrix[0][2] * ( matrix[1][0] * A1323 - matrix[1][1] * A0323 + matrix[1][3] * A0123 )
                - matrix[0][3] * ( matrix[1][0] * A1223 - matrix[1][1] * A0223 + matrix[1][2] * A0123 ) ;

    if (Zero::Abs(det) <= epsilon) return false;

    float inv_det = 1.0f / det;

    out.matrix[0][0] = inv_det *   ( matrix[1][1] * A2323 - matrix[1][2] * A1323 + matrix[1][3] * A1223 );
    out.matrix[0][1] = inv_det * - ( matrix[0][1] * A2323 - matrix[0][2] * A1323 + matrix[0][3] * A1223 );
    out.matrix[0][2] = inv_det *   ( matrix[0][1] * A2313 - matrix[0][2] * A1313 + matrix[0][3] * A1213 );
    out.matrix[0][3] = inv_det * - ( matrix[0][1] * A2312 - matrix[0][2] * A1312 + matrix[0][3] * A1212 );

    out.matrix[1][0] = inv_det * - ( matrix[1][0] * A2323 - matrix[1][2] * A0323 + matrix[1][3] * A0223 );
    out.matrix[1][1] = inv_det *   ( matrix[0][0] * A2323 - matrix[0][2] * A0323 + matrix[0][3] * A0223 );
    out.matrix[1][2] = inv_det * - ( matrix[0][0] * A2313 - matrix[0][2] * A0313 + matrix[0][3] * A0213 );
    out.matrix[1][3] = inv_det *   ( matrix[0][0] * A2312 - matrix[0][2] * A0312 + matrix[0][3] * A0212 );

    out.matrix[2][0] = inv_det *   ( matrix[1][0] * A1323 - matrix[1][1] * A0323 + matrix[1][3] * A0123 );
    out.matrix[2][1] = inv_det * - ( matrix[0][0] * A1323 - matrix[0][1] * A0323 + matrix[0][3] * A0123 );
    out.matrix[2][2] = inv_det *   ( matrix[0][0] * A1313 - matrix[0][1] * A0313 + matrix[0][3] * A0113 );
    out.matrix[2][3] = inv_det * - ( matrix[0][0] * A1312 - matrix[0][1] * A0312 + matrix[0][3] * A0112 );

    out.matrix[3][0] = inv_det * - ( matrix[1][0] * A1223 - matrix[1][1] * A0223 + matrix[1][2] * A0123 );
    out.matrix[3][1] = inv_det *   ( matrix[0][0] * A1223 - matrix[0][1] * A0223 + matrix[0][2] * A0123 );
    out.matrix[3][2] = inv_det * - ( matrix[0][0] * A1213 - matrix[0][1] * A0213 + matrix[0][2] * A0113 );
    out.matrix[3][3] = inv_det *   ( matrix[0][0] * A1212 - matrix[0][1] * A0212 + matrix[0][2] * A0112 );

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
            m.matrix[i][j] = matrix[j][i];
        }
    }
    return m;
}

Vector4 Matrix4::operator*(const Vector4& rhs) const {
    float x = matrix[0][0] * rhs.x + matrix[0][1] * rhs.y + matrix[0][2] * rhs.z + matrix[0][3] * rhs.w;
    float y = matrix[1][0] * rhs.x + matrix[1][1] * rhs.y + matrix[1][2] * rhs.z + matrix[1][3] * rhs.w;
    float z = matrix[2][0] * rhs.x + matrix[2][1] * rhs.y + matrix[2][2] * rhs.z + matrix[2][3] * rhs.w;
    float w = matrix[3][0] * rhs.x + matrix[3][1] * rhs.y + matrix[3][2] * rhs.z + matrix[3][3] * rhs.w;
    return Vector4(x, y, z, w);
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
    Matrix4 m{};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.matrix[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                m.matrix[i][j] += (matrix[i][k] * rhs.matrix[k][j]);
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
