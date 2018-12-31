#include "Quaternion.h"
#include "Matrix3.h"
#include "Vector3.h"

using namespace Zero;

bool Quaternion::operator==(const Quaternion& other) const {
	return Equal(w, other.w) &&
			Equal(x, other.x) &&
			Equal(y, other.y) &&
			Equal(z, other.z);
}

bool Quaternion::operator!=(const Quaternion& other) const {
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

Quaternion Quaternion::operator/(float scalar) {
	float inv_scalar = 1.0f / scalar;
	return Quaternion(w * inv_scalar, x * inv_scalar, y * inv_scalar, z * inv_scalar);
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
	Vector3 u = XYZ();
	Vector3 c1 = Vector3::Cross(u, v);
	Vector3 c2 = Vector3::Cross(u, c1);
	return v + 2.0f * ((c1 * w) + c2);
}

/* ********** Quaternion Operations ********** */
float Quaternion::Norm() const {
	return Sqrt((w * w) + (x * x) + (y * y) + (z * z));
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
		w = 1.0f;
		x = y = z = 0.0f;
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
		w = 1.0f;
		x = y = z = 0.0f;
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
	float ww = w * w;
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;

	float unit = xx + yy + zz + ww;
	float test = x * y + z * w;

	if (test > 0.499f * unit) {
		return Vector3(2.0f * Zero::Atan2(x, w), Zero::PI_2, 0.0f);
	}

	if (test < -0.499f * unit) {
		return Vector3(-2.0f * Zero::Atan2(x, w), -Zero::PI_2, 0.0f);
	}

	float heading = Zero::Atan2(2.0f * (y * w - x * z), xx - yy - zz + ww);
	float attitude = Zero::Asin(2.0f * test / unit);
	float bank = Zero::Atan2(2.0f * (x * w - y * z), -xx + yy - zz + ww);

	return Vector3(bank, heading, attitude);
}

Vector3 Quaternion::XYZ() const {
	return Vector3(x, y, z);
}

Matrix3 Quaternion::GetRotationMatrix() const {
	Quaternion q = UnitNormCopy();
	if (q == Zero()) {
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

Quaternion Quaternion::FromAngleAxis(const Vector3& axis, Radian angle) {
	Vector3 normalized = Vector3::Normalize(axis);
	float half_angle_rad = angle.rad * 0.5f;
	float sin_half_angle_rad = Zero::Sin(half_angle_rad);

	// Assume axis is normalized
	return Quaternion(Zero::Cos(half_angle_rad),
	                  normalized.x * sin_half_angle_rad,
	                  normalized.y * sin_half_angle_rad,
	                  normalized.z * sin_half_angle_rad);
}

Quaternion Quaternion::FromAngleAxis(const Vector3& axis, Degree angle) {
	return FromAngleAxis(axis, angle.ToRadian());
}

Quaternion Quaternion::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) {
	Matrix3 rotation_matrix(xAxis.x, yAxis.x, zAxis.x,
                            xAxis.y, yAxis.y, zAxis.y,
                            xAxis.z, yAxis.z, zAxis.z);

	return FromMatrix3(rotation_matrix);
}

Quaternion Quaternion::FromEuler(Radian x, Radian y, Radian z) {
	float rx = 0.5f * x.rad;
	float ry = 0.5f * y.rad;
	float rz = 0.5f * z.rad;

	float cx = Cos(rx);
	float sx = Sin(rx);
	float cy = Cos(ry);
	float sy = Sin(ry);
	float cz = Cos(rz);
	float sz = Sin(rz);

	float cxcy = cx * cy;
	float sxsy = sx * sy;

	return Quaternion((cxcy * cz) - (sxsy * sz),
                      (cxcy * sz) + (sxsy * cz),
                      (sx * cy * cz) + (cx * sy * sz),
                      (cx * sy * cz) - (sx * cy * sz));
}

Quaternion Quaternion::FromMatrix3(const Matrix3& matrix) {
	float w = Sqrt(Max(0.0f, 1.0f + matrix(0, 0) + matrix(1, 1) + matrix(2, 2))) * 0.5f;
	float x = Sqrt(Max(0.0f, 1.0f + matrix(0, 0) - matrix(1, 1) - matrix(2, 2))) * 0.5f;
	float y = Sqrt(Max(0.0f, 1.0f - matrix(0, 0) + matrix(1, 1) - matrix(2, 2))) * 0.5f;
	float z = Sqrt(Max(0.0f, 1.0f - matrix(0, 0) - matrix(1, 1) + matrix(2, 2))) * 0.5f;

	x = Copysign(x, matrix(2, 1) - matrix(1, 2));
	y = Copysign(y, matrix(0, 2) - matrix(2, 0));
	z = Copysign(z, matrix(2, 0) - matrix(0, 2));

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

	Quaternion rot1 = FromToRotation(Vector3::Forward(), direction);
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
		rotation_axis = Vector3::Cross(Vector3::Forward(), from_N);

		if (rotation_axis.SquareMagnitude() < EPSILON) {
			rotation_axis = Vector3::Cross(Vector3::Right(), from_N);
		}

		rotation_axis.Normalize();
		return FromAngleAxis(rotation_axis, Degree(180.0f));
	}

	rotation_axis = Vector3::Cross(from_N, to_N);
	float s = Sqrt((1.0f + theta) * 2.0f);
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


