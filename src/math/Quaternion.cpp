#include "Quaternion.h"
#include "Matrix3.h"
#include "Vector3.h"

using namespace Zero;

static const auto ZERO = Quaternion::Zero();

bool Quaternion::operator==(const Quaternion& other) {
	return equal(w, other.w) &&
           equal(x, other.x) &&
           equal(y, other.y) &&
           equal(z, other.z);
}

bool Quaternion::operator!=(const Quaternion& other) {
	return !operator==(other);
}

/* ********** Scalar Operations ********** */
Quaternion Quaternion::operator+(float scalar) {
	return Quaternion(w + scalar, x + scalar, y + scalar, z + scalar);
}

Quaternion Quaternion::operator-(float scalar) {
	return Quaternion(w - scalar, x - scalar, y - scalar, z - scalar);
}

Quaternion Quaternion::operator*(float scalar) {
	return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}

Quaternion& Quaternion::operator+=(float scalar) {
	w += scalar;
	x += scalar;
	y += scalar;
	z += scalar;
	return *this;
}

Quaternion& Quaternion::operator-=(float scalar) {
	w -= scalar;
	x -= scalar;
	y -= scalar;
	z -= scalar;
	return *this;
}

Quaternion& Quaternion::operator*=(float scalar) {
	w *= scalar;
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Quaternion& Quaternion::operator/=(float scalar) {
	float inv_scalar = 1.0f / scalar;
	w *= inv_scalar;
	x *= inv_scalar;
	y *= inv_scalar;
	z *= inv_scalar;
	return *this;
}

/* ********** Quaternion/Quaternion Operations ********** */
Quaternion Quaternion::operator+(const Quaternion& rhs) {
	return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

Quaternion Quaternion::operator-(const Quaternion& rhs) {
	return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
}

Quaternion Quaternion::operator*(const Quaternion& rhs) {
	return Quaternion(
		(w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
		(w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y),
		(w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x),
		(w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w)
	);
}

Quaternion& Quaternion::operator+=(const Quaternion& rhs) {
	w += rhs.w;
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& rhs) {
	w -= rhs.w;
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
	float new_w = (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z);
	float new_x = (w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y);
	float new_y = (w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x);
	float new_z = (w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w);
	w = new_w;
	x = new_x;
	y = new_y;
	z = new_z;
	return *this;
}

Vector3 Quaternion::operator*(const Vector3& v) const {
	Vector3 u = xyz();
	return (2.0f * Vector3::Dot(u, v) * u)
           + (w * w - Vector3::Dot(u, u) * v)
           + (2.0f * w * Vector3::Cross(u, v));
}

/* ********** Quaternion Operations ********** */
float Quaternion::Norm() const {
	return sqrt((w * w) + (x * x) + (y * y) + (z * z));
}

Quaternion& Quaternion::UnitNorm() {
	float norm = Norm();
	if (norm > 0.0f) {
		float inv_norm = 1.0f / norm;
		w *= inv_norm;
		x *= inv_norm;
		y *= inv_norm;
		z *= inv_norm;
	}
	else {
		w = x = y = z = 0.0f;
	}
	return *this;
}

Quaternion& Quaternion::Conjugate() {
	x = -x;
	y = -y;
	z = -z;
 	return *this;
}

Quaternion& Quaternion::Inverse() {
	float norm = Norm();
	if (norm > 0.0f) {
		float inv_norm = 1.0f / norm;
		w *= inv_norm;
		x *= -inv_norm;
		y *= -inv_norm;
		z *= -inv_norm;
	}
	else {
		w = x = y = z = 0.0f;
	}
	return *this;
}

Quaternion& Quaternion::Negate() {
	w = -w;
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

Quaternion& Quaternion::RotateX(Radian angle) {
	float half_hangle_rad = angle.rad * 0.5f;
	w *= cos(half_hangle_rad);
	x *= sin(half_hangle_rad);
	return *this;
}

Quaternion& Quaternion::RotateY(Radian angle) {
	float half_hangle_rad = angle.rad * 0.5f;
	w *= cos(half_hangle_rad);
	y *= sin(half_hangle_rad);
	return *this;
}

Quaternion& Quaternion::RotateZ(Radian angle) {
	float half_hangle_rad = angle.rad * 0.5f;
	w *= cos(half_hangle_rad);
	z *= sin(half_hangle_rad);
	return *this;
}

Quaternion Quaternion::UnitNormCopy() const {
	Quaternion q = *this;
	return q.UnitNorm();
}

Quaternion Quaternion::ConjugateCopy() const {
	Quaternion q = *this;
	return q.Conjugate();
}

Quaternion Quaternion::InverseCopy() const {
	Quaternion q = *this;
	return q.Inverse();
}

Vector3 Quaternion::GetEulerAngles() const {
	return Vector3(GetRoll().rad,
                   GetPitch().rad,
                   GetYaw().rad);
}

Radian Quaternion::GetRoll() const {
	float sinr_cosp = 2.0f * (w * x + y * z);
	float cosr_cosp = 1.0f - (2.0f * (x * x + y * y));
	float roll = atan2(sinr_cosp, cosr_cosp);

	return Radian(roll);
}

Radian Quaternion::GetPitch() const {
	float pitch;
	float sinp = 2.0f * (w * y - z * x);
	if (sinp >= 1) {
		pitch = copysign(PI * 0.5f, sinp);
	}
	else {
		pitch = asin(sinp);
	}

	return Radian(pitch);
}

Radian Quaternion::GetYaw() const {
	float siny_cosp = 2.0f * (w * z + x * y);
	float cosy_cosp = 1.0f - (2.0f * (y * y + z * z));
	float yaw = atan2(siny_cosp, cosy_cosp);

	return Radian(yaw);
}

Vector3 Quaternion::xyz() const {
	return Vector3(x, y, z);
}

Matrix3 Quaternion::GetRotationMatrix() const {
	Quaternion q = UnitNormCopy();
	if (q == ZERO) {
		return Matrix3::Identity();
	}

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;

	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;

	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	float e00 = 1.0f - (2.0f * (yy + zz));
	float e01 = 2.0f * (xy - wz);
	float e02 = 2.0f * (wy + xz);

	float e10 = 2.0f * (xy + wz);
	float e11 = 1.0f - (2.0f * (xx + zz));
	float e12 = 2.0f * (yz - wx);

	float e20 = 2.0f * (xz - wy);
	float e21 = 2.0f * (wx + yz);
	float e22 = 1.0f - (2.0f * (xx + yy));

	return Matrix3(e00, e01, e02,
                   e10, e11, e12,
                   e20, e21, e22);
}

/* ********** Static Operations ********** */
float Quaternion::Dot(const Quaternion& lhs, const Quaternion& rhs) {
	return (lhs.w * rhs.w) + (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, Radian angle) {
	float half_angle_rad = angle.rad * 0.5f;
	float sin_half_angle_rad = sin(half_angle_rad);

	// Assume axis is normalized
	return Quaternion(cos(half_angle_rad),
                      axis.x * sin_half_angle_rad,
                      axis.y * sin_half_angle_rad,
                      axis.z * sin_half_angle_rad);
}

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, Degree angle) {
	return FromAxisAngle(axis, angle.ToRadian());
}

Quaternion Quaternion::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) {
	Matrix3 rotation_matrix(xAxis.x, yAxis.x, zAxis.x,
                            xAxis.y, yAxis.y, zAxis.y,
                            xAxis.z, yAxis.z, zAxis.z);

	return FromMatrix3(rotation_matrix);
}

Quaternion Quaternion::FromEuler(float x, float y, float z) {
	float z_half = z * 0.5f;
	float y_half = y * 0.5f;
	float x_half = x * 0.5f;

	float cz = cos(z_half);
	float sz = sin(z_half);
	float cy = cos(y_half);
	float sy = sin(y_half);
	float cx = cos(x_half);
	float sx = sin(x_half);

	return Quaternion((cz * cy * cx) + (sz * sy * sz),
                      (cz * cy * sx) - (sz * sy * cx),
                      (sz * cy * sx) + (cz * sy * cx),
                      (sz * cy * cx) - (cz * sy * sx));
}

Quaternion Quaternion::FromMatrix3(const Matrix3& matrix) {
	float w = sqrt( max(0.0f, 1.0f + matrix(0, 0) + matrix(1, 1) + matrix(2, 2)) ) * 0.5f;
	float x = sqrt( max(0.0f, 1.0f + matrix(0, 0) - matrix(1, 1) - matrix(2, 2)) ) * 0.5f;
	float y = sqrt( max(0.0f, 1.0f - matrix(0, 0) + matrix(1, 1) - matrix(2, 2)) ) * 0.5f;
	float z = sqrt( max(0.0f, 1.0f - matrix(0, 0) - matrix(1, 1) + matrix(2, 2)) ) * 0.5f;

	x = copysign(x, matrix(2, 1) - matrix(1, 2));
	y = copysign(y, matrix(0, 2) - matrix(2, 0));
	z = copysign(z, matrix(2, 0) - matrix(0, 2));

	return Quaternion(w, x, y, z);
}

Quaternion Quaternion::LookRotation(const Vector3& direction, const Vector3& up) {
	// Source: https://github.com/opengl-tutorials/ogl/blob/master/common/quaternion_utils.cpp

	if (direction.SquareMagnitude() < EPSILON) {
		return Identity();
	}

	// Recompute desired up so that it is perpendicular to direction
	Vector3 right = Vector3::Cross(direction, up);
	Vector3 recomputed_up = Vector3::Cross(right, direction);

	Quaternion rot1 = FromToRotation(Vector3::Front(), direction);
	Vector3 new_up = rot1 * Vector3::Up();
	Quaternion rot2 = FromToRotation(new_up, recomputed_up);

	return rot2 * rot1;
}

Quaternion Quaternion::FromToRotation(const Vector3& from, const Vector3& to) {
	// Source: https://github.com/opengl-tutorials/ogl/blob/master/common/quaternion_utils.cpp
	Vector3 from_N = Vector3::Normalize(from);
	Vector3 to_N = Vector3::Normalize(to);

	float theta = Vector3::Dot(from_N, to_N);
	Vector3 rotation_axis;

	if (theta < -1.0f + EPSILON) {
		rotation_axis = Vector3::Cross(Vector3::Front(), from_N);

		if (rotation_axis.SquareMagnitude() < EPSILON) {
			rotation_axis = Vector3::Cross(Vector3::Right(), from_N);
		}

		rotation_axis.Normalize();
		return FromAxisAngle(rotation_axis, Degree(180.0f));
	}

	rotation_axis = Vector3::Cross(from_N, to_N);
	float s = sqrt( (1.0f + theta) * 2.0f);
	float inv_s = 1.0f / s;

	return Quaternion(s * 0.5f,
                      rotation_axis.x * inv_s,
                      rotation_axis.y * inv_s,
                      rotation_axis.z * inv_s);
}

/* ********** Useful Quaternions ********** */
Quaternion Quaternion::Identity() {
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion Quaternion::Zero() {
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}


