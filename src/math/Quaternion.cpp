#include "Quaternion.h"
#include "Matrix3.h"
#include "Vector3.h"

using namespace Zero;


bool Quaternion::operator==(const Quaternion& other) {
	return equal(w, other.w) && equal(x, other.x) && equal(y, other.y) && equal(z, other.z);
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
	w *= cos (half_hangle_rad);
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
	// TODO: Finish Implementation
	return Zero::Vector3(); // stub
}

Vector3 Quaternion::xyz() const {
	return Vector3(x, y, z);
}

Matrix3 Quaternion::GetRotationMatrix3() const {
	// TODO: Finish Implementation
	return Matrix3();
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
	float trace = matrix(0, 0) + matrix(1, 1) + matrix(2, 2);
	float w, x, y, z;
	// TODO: Finish Implementation
	return Quaternion(w, x, y, z);
}

Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& up = Vector3::Up()) {
	// TODO: Finish Implementation
	return Quaternion(); // stub
}

Quaternion Quaternion::FromToRotation(const Vector3& fromDirection, const Vector3& toDirection) {
	// TODO: Finish Implementation
	return Quaternion(); // stub
}

Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to, Degree maxDegreesDelta) {
	// TODO: Finish Implementation
	return Quaternion(); // stub
}

/* ********** Useful Quaternions ********** */
Quaternion Quaternion::Identity() {
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion Quaternion::Zero() {
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}


