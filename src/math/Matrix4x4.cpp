#include "math/Matrix4x4.hpp"
#include "math/Vector4.hpp"
#include "math/Quaternion.hpp"

namespace zero::math
{

Matrix4x4::Matrix4x4(float m[4][4])
: matrix_()
{
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            matrix_[i][j] = m[i][j];
        }
    }
}

Matrix4x4::Matrix4x4(float value)
: matrix_()
{
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            matrix_[i][j] = value;
        }
    }
}

Matrix4x4::Matrix4x4(const Matrix3x3& m3)
: matrix_()
{
    matrix_[0][0] = m3(0, 0);
    matrix_[0][1] = m3(0, 1);
    matrix_[0][2] = m3(0, 2);
    matrix_[0][3] = 0.0F;

    matrix_[1][0] = m3(1, 0);
    matrix_[1][1] = m3(1, 1);
    matrix_[1][2] = m3(1, 2);
    matrix_[1][3] = 0.0F;

    matrix_[2][0] = m3(2, 0);
    matrix_[2][1] = m3(2, 1);
    matrix_[2][2] = m3(2, 2);
    matrix_[2][3] = 0.0F;

    matrix_[3][0] = 0.0F;
    matrix_[3][1] = 0.0F;
    matrix_[3][2] = 0.0F;
    matrix_[3][3] = 1.0F;
}

Matrix4x4::Matrix4x4(float e00, float e01, float e02, float e03,
                     float e10, float e11, float e12, float e13,
                     float e20, float e21, float e22, float e23,
                     float e30, float e31, float e32, float e33)
: matrix_()
{

    matrix_[0][0] = e00; matrix_[0][1] = e01; matrix_[0][2] = e02; matrix_[0][3] = e03;
    matrix_[1][0] = e10; matrix_[1][1] = e11; matrix_[1][2] = e12; matrix_[1][3] = e13;
    matrix_[2][0] = e20; matrix_[2][1] = e21; matrix_[2][2] = e22; matrix_[2][3] = e23;
    matrix_[3][0] = e30; matrix_[3][1] = e31; matrix_[3][2] = e32; matrix_[3][3] = e33;
}

bool Matrix4x4::operator==(const Matrix4x4& o) const
{
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            if (!Equal(matrix_[i][j], o.matrix_[i][j]))
            {
                return false;
            }
        }
    }

    return true;
}

bool Matrix4x4::operator!=(const Matrix4x4& o) const
{
    return !(operator==(o));
}

float Matrix4x4::operator()(size_t row, size_t col) const
{
    return matrix_[row][col];
}

float Matrix4x4::operator()(size_t row, size_t col)
{
    return matrix_[row][col];
}

const float* Matrix4x4::operator[](size_t index) const
{
    return matrix_[index];
}

float* Matrix4x4::operator[](size_t index)
{
    return matrix_[index];
}

Matrix4x4 Matrix4x4::operator+(float scalar) const
{
    return Matrix4x4(*this) += scalar;
}

Matrix4x4 Matrix4x4::operator-(float scalar) const
{
    return Matrix4x4(*this) -= scalar;
}

Matrix4x4 Matrix4x4::operator*(float scalar) const
{
    return Matrix4x4(*this) *= scalar;
}

Matrix4x4 Matrix4x4::operator/(float scalar) const
{
    return Matrix4x4(*this) /= scalar;
}

Matrix4x4& Matrix4x4::operator+=(float scalar)
{
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            matrix_[i][j] += scalar;
        }
    }
    return *this;
}

Matrix4x4& Matrix4x4::operator-=(float scalar)
{
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            matrix_[i][j] -= scalar;
        }
    }
    return *this;
}

Matrix4x4& Matrix4x4::operator*=(float scalar)
{
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            matrix_[i][j] *= scalar;
        }
    }
    return *this;
}

Matrix4x4& Matrix4x4::operator/=(float scalar)
{
    float inv_scalar = 1.0F / scalar;
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            matrix_[i][j] *= inv_scalar;
        }
    }
    return *this;
}

Matrix4x4 Matrix4x4::Hadamard(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 m{};
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            m.matrix_[i][j] = lhs.matrix_[i][j] * rhs.matrix_[i][j];
        }
    }
    return m;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& rhs) const
{
    return Matrix4x4(*this) += rhs;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& rhs) const
{
    return Matrix4x4(*this) -= rhs;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs)
{
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            matrix_[i][j] += rhs.matrix_[i][j];
        }
    }
    return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs)
{
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            matrix_[i][j] -= rhs.matrix_[i][j];
        }
    }
    return *this;
}

float Matrix4x4::Det() const
{
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

bool Matrix4x4::InverseUtil(Matrix4x4& out, float epsilon) const
{
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

    if (Abs(det) <= epsilon)
    {
        return false;
    }

    float inv_det = 1.0F / det;

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

Matrix4x4 Matrix4x4::Inverse(float epsilon) const
{
    Matrix4x4 m{};
    InverseUtil(m, epsilon);
    return m;
}

Matrix4x4 Matrix4x4::Transpose() const
{
    Matrix4x4 m{};
    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            m.matrix_[i][j] = matrix_[j][i];
        }
    }
    return m;
}

Vec4f Matrix4x4::operator*(const Vec4f& rhs) const
{
    float x = matrix_[0][0] * rhs.x_ + matrix_[0][1] * rhs.y_ + matrix_[0][2] * rhs.z_ + matrix_[0][3] * rhs.w_;
    float y = matrix_[1][0] * rhs.x_ + matrix_[1][1] * rhs.y_ + matrix_[1][2] * rhs.z_ + matrix_[1][3] * rhs.w_;
    float z = matrix_[2][0] * rhs.x_ + matrix_[2][1] * rhs.y_ + matrix_[2][2] * rhs.z_ + matrix_[2][3] * rhs.w_;
    float w = matrix_[3][0] * rhs.x_ + matrix_[3][1] * rhs.y_ + matrix_[3][2] * rhs.z_ + matrix_[3][3] * rhs.w_;
    return Vec4f(x, y, z, w);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const
{
    Matrix4x4 m{};

    for (uint32 i = 0; i < 4; ++i)
    {
        for (uint32 j = 0; j < 4; ++j)
        {
            m.matrix_[i][j] = 0;
            for (uint32 k = 0; k < 4; ++k)
            {
                m.matrix_[i][j] += (matrix_[i][k] * rhs.matrix_[k][j]);
            }
        }
    }

    return m;
}

Matrix4x4& Matrix4x4::Translate(const Vec3f& translation)
{
    for (int i = 0; i < 3; ++i)
    {
        matrix_[i][3] += translation[i];
    }
    return *this;
}

Matrix4x4& Matrix4x4::Rotate(const Quaternion& rotation)
{
    auto result = rotation.GetRotationMatrix() * GetMatrix3x3();
    for (uint32 i = 0; i < 3; ++i)
    {
        for (uint32 j = 0; j < 3; ++j)
        {
            matrix_[i][j] = result[i][j];
        }
    }
    return *this;
}

Matrix4x4& Matrix4x4::Scale(const Vec3f& scale)
{
    for (uint32 i = 0; i < 3; ++i)
    {
        matrix_[i][i] *= scale[i];
    }
    return *this;
}

Vec3f Matrix4x4::GetTranslation() const
{
    return Vec3f(matrix_[0][3],
                 matrix_[1][3],
                 matrix_[2][3]);
}

Quaternion Matrix4x4::GetRotation() const
{
    return Quaternion::FromMatrix3(GetMatrix3x3());
}

Vec3f Matrix4x4::GetScale() const
{
    auto sx = Vec3f(matrix_[0][0], matrix_[1][0], matrix_[2][0]).Magnitude();
    auto sy = Vec3f(matrix_[0][1], matrix_[1][1], matrix_[2][1]).Magnitude();
    auto sz = Vec3f(matrix_[0][2], matrix_[1][2], matrix_[2][2]).Magnitude();
    return Vec3f(sx, sy, sz);
}

Matrix3x3 Matrix4x4::GetMatrix3x3() const
{
    return Matrix3x3(matrix_[0][0], matrix_[0][1], matrix_[0][2],
                     matrix_[1][0], matrix_[1][1], matrix_[1][2],
                     matrix_[2][0], matrix_[2][1], matrix_[2][2]);
}

Matrix4x4 Matrix4x4::Identity()
{
    return Matrix4x4(1.0F, 0.0F, 0.0F, 0.0F,
                     0.0F, 1.0F, 0.0F, 0.0F,
                     0.0F, 0.0F, 1.0F, 0.0F,
                     0.0F, 0.0F, 0.0F, 1.0F);
}
math::Matrix4x4 Matrix4x4::LookAt(const math::Vec3f& eye, const math::Vec3f& center, const math::Vec3f& up)
{
    const math::Vec3f f(math::Vec3f::Normalize(center - eye));
    const math::Vec3f s(math::Vec3f::Normalize(math::Vec3f::Cross(f, up)));
    const math::Vec3f u(math::Vec3f::Cross(s, f));

    math::Matrix4x4 result = math::Matrix4x4::Identity();
    result[0][0] = s.x_;
    result[0][1] = s.y_;
    result[0][2] = s.z_;
    result[1][0] = u.x_;
    result[1][1] = u.y_;
    result[1][2] = u.z_;
    result[2][0] = -f.x_;
    result[2][1] = -f.y_;
    result[2][2] = -f.z_;
    result[0][3] = -math::Vec3f::Dot(s, eye);
    result[1][3] = -math::Vec3f::Dot(u, eye);
    result[2][3] = math::Vec3f::Dot(f, eye);
    return result;
}

zero::math::Matrix4x4 Matrix4x4::Perspective(math::Radian vertical_fov, float aspect_ratio, float near, float far)
{

    math::Matrix4x4 perspective_matrix(0.0F);

    float tan_half_vertical_fov = math::Tan(vertical_fov.rad_ / 2.0F);
    perspective_matrix[0][0] = 1.0F / (aspect_ratio * tan_half_vertical_fov);
    perspective_matrix[1][1] = 1.0F / (tan_half_vertical_fov);
    perspective_matrix[2][2] = -(far + near) / (far - near);
    perspective_matrix[2][3] = -(2.0F * far * near) / (far - near);
    perspective_matrix[3][2] = -1.0F;

    return perspective_matrix;
}

zero::math::Matrix4x4 Matrix4x4::Orthographic(float left, float right, float bottom, float top, float near, float far)
{

    math::Matrix4x4 orthographic_matrix = Identity();

    orthographic_matrix[0][0] = 2.0F / (right - left);
    orthographic_matrix[1][1] = 2.0F / (top - bottom);
    orthographic_matrix[2][2] = -2.0F / (far - near);
    orthographic_matrix[0][3] = -(right + left) / (right - left);
    orthographic_matrix[1][3] = -(top + bottom) / (top - bottom);
    orthographic_matrix[2][3] = -(far + near) / (far - near);

    return orthographic_matrix;
}

} // namespace zero::math