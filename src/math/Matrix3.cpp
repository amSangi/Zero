#include "Matrix3.h"
#include "Vector3.h"

using namespace Zero;

Matrix3::Matrix3(float m[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix[i][j] = m[i][j];
        }
    }
}

Matrix3::Matrix3(float value) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix[i][j] = value;
        }
    }
}

Matrix3::Matrix3(float e00, float e01, float e02,
                 float e10, float e11, float e12,
                 float e20, float e21, float e22) {

    matrix[0][0] = e00; matrix[0][1] = e01; matrix[0][2] = e02;
    matrix[1][0] = e10; matrix[1][1] = e11; matrix[1][2] = e12;
    matrix[2][0] = e20; matrix[2][1] = e21; matrix[2][2] = e22;
}

bool Matrix3::operator==(const Matrix3& o) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!Equal(matrix[i][j], o.matrix[i][j])) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix3::operator!=(const Matrix3& o) const {
    return !(operator==(o));
}

const float Matrix3::operator()(size_t row, size_t col) const {
    return matrix[row][col];
}

float Matrix3::operator()(size_t row, size_t col) {
    return matrix[row][col];
}

const float* Matrix3::operator[](size_t index) const {
    return matrix[index];
}

float* Matrix3::operator[](size_t index) {
    return matrix[index];
}

Matrix3 Matrix3::operator+(float scalar) const {
    Matrix3 m{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix[i][j] = matrix[i][j] + scalar;
        }
    }
    return m;
}

Matrix3 Matrix3::operator-(float scalar) const {
    Matrix3 m{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix[i][j] = matrix[i][j] - scalar;
        }
    }
    return m;
}

Matrix3 Matrix3::operator*(float scalar) const {
    Matrix3 m{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix[i][j] = matrix[i][j] * scalar;
        }
    }
    return m;
}

Matrix3 Matrix3::operator/(float scalar) const {
    Matrix3 m{};
    float inv_scalar = 1.0f / scalar;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix[i][j] = matrix[i][j] * inv_scalar;
        }
    }
    return m;
}

Matrix3& Matrix3::operator+=(float scalar) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            matrix[i][j] += scalar;
    }
    return *this;
}

Matrix3& Matrix3::operator-=(float scalar) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            matrix[i][j] -= scalar;
    }
    return *this;
}

Matrix3& Matrix3::operator*=(float scalar) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            matrix[i][j] *= scalar;
    }
    return *this;
}

Matrix3& Matrix3::operator/=(float scalar) {
    float inv_scalar = 1.0f / scalar;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            matrix[i][j] *= inv_scalar;
    }
    return *this;
}

Matrix3 Matrix3::Hadamard(const Matrix3& lhs, const Matrix3& rhs) {
    Matrix3 m{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix[i][j] = lhs.matrix[i][j] * rhs.matrix[i][j];
        }
    }
    return m;
}

Matrix3 Matrix3::operator+(const Matrix3& rhs) const {
    Matrix3 m{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix[i][j] = matrix[i][j] + rhs.matrix[i][j];
        }
    }
    return m;
}

Matrix3 Matrix3::operator-(const Matrix3& rhs) const {
    Matrix3 m{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix[i][j] = matrix[i][j] - rhs.matrix[i][j];
        }
    }
    return m;
}

Matrix3& Matrix3::operator+=(const Matrix3& rhs) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix[i][j] += rhs.matrix[i][j];
        }
    }
    return *this;
}

Matrix3& Matrix3::operator-=(const Matrix3& rhs) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix[i][j] -= rhs.matrix[i][j];
        }
    }
    return *this;
}

float Matrix3::Det() const {
    return   matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
           - matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
           + matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

bool Matrix3::InverseUtil(Matrix3& out, float epsilon) const {
    float t1 = matrix[1][1] * matrix[2][2];
    float t2 = matrix[1][0] * matrix[2][2];
    float t3 = matrix[1][2] * matrix[2][0];
    float t4 = matrix[1][0] * matrix[2][1];

    float det =   matrix[0][0] * (t1 - matrix[1][2] * matrix[2][1])
                - matrix[0][1] * (t2 - t3)
                + matrix[0][2] * (t4 - matrix[1][1] * matrix[2][0]);

    if (Zero::Abs(det) <= epsilon) return false;

    float inv_det = 1.0f / det;

    out.matrix[0][0] = (t1 - matrix[2][1] * matrix[1][2]) * inv_det;
    out.matrix[0][1] = (matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2]) * inv_det;
    out.matrix[0][2] = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) * inv_det;
    out.matrix[1][0] = (t3 - t2) * inv_det;
    out.matrix[1][1] = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) * inv_det;
    out.matrix[1][2] = (matrix[1][0] * matrix[0][2] - matrix[0][0] * matrix[1][2]) * inv_det;
    out.matrix[2][0] = (t4 - matrix[2][0] * matrix[1][1]) * inv_det;
    out.matrix[2][1] = (matrix[2][0] * matrix[0][1] - matrix[0][0] * matrix[2][1]) * inv_det;
    out.matrix[2][2] = (matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1]) * inv_det;

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
            m.matrix[i][j] = matrix[j][i];
        }
    }
    return m;
}

Vector3 Matrix3::operator*(const Vector3& rhs) const {
    float x = matrix[0][0] * rhs.x + matrix[0][1] * rhs.y + matrix[0][2] * rhs.z;
    float y = matrix[1][0] * rhs.x + matrix[1][1] * rhs.y + matrix[1][2] * rhs.z;
    float z = matrix[2][0] * rhs.x + matrix[2][1] * rhs.y + matrix[2][2] * rhs.z;
    return Vector3(x, y, z);
}

Matrix3 Matrix3::operator*(const Matrix3& rhs) const {
    Matrix3 m{};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.matrix[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                m.matrix[i][j] += (matrix[i][k] * rhs.matrix[k][j]);
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
