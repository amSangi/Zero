#include "Matrix3.hpp"
#include "Vector3.hpp"

using namespace zero::math;

Matrix3::Matrix3(float m[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix_[i][j] = m[i][j];
        }
    }
}

Matrix3::Matrix3(float value) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix_[i][j] = value;
        }
    }
}

Matrix3::Matrix3(float e00, float e01, float e02,
                 float e10, float e11, float e12,
                 float e20, float e21, float e22) {

    matrix_[0][0] = e00; matrix_[0][1] = e01; matrix_[0][2] = e02;
    matrix_[1][0] = e10; matrix_[1][1] = e11; matrix_[1][2] = e12;
    matrix_[2][0] = e20; matrix_[2][1] = e21; matrix_[2][2] = e22;
}

bool Matrix3::operator==(const Matrix3& other) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!Equal(matrix_[i][j], other.matrix_[i][j])) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix3::operator!=(const Matrix3& other) const {
    return !(operator==(other));
}

const float Matrix3::operator()(size_t row, size_t col) const {
    return matrix_[row][col];
}

float Matrix3::operator()(size_t row, size_t col) {
    return matrix_[row][col];
}

const float* Matrix3::operator[](size_t index) const {
    return matrix_[index];
}

float* Matrix3::operator[](size_t index) {
    return matrix_[index];
}

Matrix3 Matrix3::operator+(float scalar) const {
    return Matrix3(*this) += scalar;
}

Matrix3 Matrix3::operator-(float scalar) const {
    return Matrix3(*this) -= scalar;
}

Matrix3 Matrix3::operator*(float scalar) const {
    return Matrix3(*this) *= scalar;
}

Matrix3 Matrix3::operator/(float scalar) const {
    return Matrix3(*this) /= scalar;
}

Matrix3& Matrix3::operator+=(float scalar) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            matrix_[i][j] += scalar;
    }
    return *this;
}

Matrix3& Matrix3::operator-=(float scalar) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            matrix_[i][j] -= scalar;
    }
    return *this;
}

Matrix3& Matrix3::operator*=(float scalar) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            matrix_[i][j] *= scalar;
    }
    return *this;
}

Matrix3& Matrix3::operator/=(float scalar) {
    float inv_scalar = 1.0f / scalar;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            matrix_[i][j] *= inv_scalar;
    }
    return *this;
}

Matrix3 Matrix3::Hadamard(const Matrix3& lhs, const Matrix3& rhs) {
    Matrix3 m{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix_[i][j] = lhs.matrix_[i][j] * rhs.matrix_[i][j];
        }
    }
    return m;
}

Matrix3 Matrix3::operator+(const Matrix3& rhs) const {
    return Matrix3(*this) += rhs;
}

Matrix3 Matrix3::operator-(const Matrix3& rhs) const {
    return Matrix3(*this) -= rhs;
}

Matrix3& Matrix3::operator+=(const Matrix3& rhs) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix_[i][j] += rhs.matrix_[i][j];
        }
    }
    return *this;
}

Matrix3& Matrix3::operator-=(const Matrix3& rhs) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix_[i][j] -= rhs.matrix_[i][j];
        }
    }
    return *this;
}

float Matrix3::Det() const {
    return   matrix_[0][0] * (matrix_[1][1] * matrix_[2][2] - matrix_[1][2] * matrix_[2][1])
           - matrix_[0][1] * (matrix_[1][0] * matrix_[2][2] - matrix_[1][2] * matrix_[2][0])
           + matrix_[0][2] * (matrix_[1][0] * matrix_[2][1] - matrix_[1][1] * matrix_[2][0]);
}

bool Matrix3::InverseUtil(Matrix3& out, float epsilon) const {
    float t1 = matrix_[1][1] * matrix_[2][2];
    float t2 = matrix_[1][0] * matrix_[2][2];
    float t3 = matrix_[1][2] * matrix_[2][0];
    float t4 = matrix_[1][0] * matrix_[2][1];

    float det =   matrix_[0][0] * (t1 - matrix_[1][2] * matrix_[2][1])
                - matrix_[0][1] * (t2 - t3)
                + matrix_[0][2] * (t4 - matrix_[1][1] * matrix_[2][0]);

    if (Abs(det) <= epsilon) return false;

    float inv_det = 1.0f / det;

    out.matrix_[0][0] = (t1 - matrix_[2][1] * matrix_[1][2]) * inv_det;
    out.matrix_[0][1] = (matrix_[0][2] * matrix_[2][1] - matrix_[0][1] * matrix_[2][2]) * inv_det;
    out.matrix_[0][2] = (matrix_[0][1] * matrix_[1][2] - matrix_[0][2] * matrix_[1][1]) * inv_det;
    out.matrix_[1][0] = (t3 - t2) * inv_det;
    out.matrix_[1][1] = (matrix_[0][0] * matrix_[2][2] - matrix_[0][2] * matrix_[2][0]) * inv_det;
    out.matrix_[1][2] = (matrix_[1][0] * matrix_[0][2] - matrix_[0][0] * matrix_[1][2]) * inv_det;
    out.matrix_[2][0] = (t4 - matrix_[2][0] * matrix_[1][1]) * inv_det;
    out.matrix_[2][1] = (matrix_[2][0] * matrix_[0][1] - matrix_[0][0] * matrix_[2][1]) * inv_det;
    out.matrix_[2][2] = (matrix_[0][0] * matrix_[1][1] - matrix_[1][0] * matrix_[0][1]) * inv_det;

    return true;
}

Matrix3 Matrix3::Inverse(float epsilon) const {
    Matrix3 m(0.0f);
    InverseUtil(m, epsilon);
    return m;
}

Matrix3 Matrix3::Transpose() const {
    Matrix3 m{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix_[i][j] = matrix_[j][i];
        }
    }
    return m;
}

Vector3 Matrix3::operator*(const Vector3& rhs) const {
    float x = matrix_[0][0] * rhs.x_ + matrix_[0][1] * rhs.y_ + matrix_[0][2] * rhs.z_;
    float y = matrix_[1][0] * rhs.x_ + matrix_[1][1] * rhs.y_ + matrix_[1][2] * rhs.z_;
    float z = matrix_[2][0] * rhs.x_ + matrix_[2][1] * rhs.y_ + matrix_[2][2] * rhs.z_;
    return Vector3(x, y, z);
}

Matrix3 Matrix3::operator*(const Matrix3& rhs) const {
    Matrix3 m{};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix_[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                m.matrix_[i][j] += (matrix_[i][k] * rhs.matrix_[k][j]);
            }
        }
    }

    return m;
}

Matrix3 Matrix3::Identity() {
    return Matrix3(1.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 1.0f);
}
