#include "Quaternion.h"
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
	w /= scalar;
	x /= scalar;
	y /= scalar;
	z /= scalar;
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

Quaternion Quaternion::UnitNorm() const {
	float norm = Norm();
	if (norm > 0.0f) {
		return Quaternion(w / norm, x / norm, y / norm, z / norm);
	}

	return Zero();
}

Quaternion Quaternion::Conjugate() const {
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::Inverse() const {
	float norm = Norm();
	if (norm > 0.0f) {
		return Quaternion(w / norm, -x / norm, -y / norm, -z / norm);
	}

	return Zero();
}

/* ********** Static Operations ********** */
float Quaternion::Dot(const Quaternion& lhs, const Quaternion& rhs) {
	return (lhs.w * rhs.w) + (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

Vector3 Quaternion::GetEulerAngles() const {
	return Zero::Vector3(); // stub
}

Quaternion Quaternion::FromAngleAxis(Radian angle, const Vector3& axis) {
	return Quaternion(); // stub
}

Quaternion Quaternion::FromAngleAxis(Degree angle, const Vector3& axis) {
	return Quaternion(); // stub
}

Quaternion Quaternion::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) {
	return Quaternion(); // stub
}

Quaternion Quaternion::FromEuler(float x, float y, float z) {
	return Quaternion(); // stub
}

Quaternion Quaternion::FromMatrix3(const Matrix3& matrix) {
	return Quaternion(); // stub
}

Quaternion Quaternion::FromLookRotation(const Vector3& forward, const Vector3& up = Vector3::Up()) {
	return Quaternion(); // stub
}

Quaternion Quaternion::FromToRotation(const Vector3& fromDirection, const Vector3& toDirection) {
	return Quaternion(); // stub
}

Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to, Degree maxDegreesDelta) {
	return Quaternion(); // stub
}

/* ********** Useful Quaternions ********** */
Quaternion Quaternion::Identity() {
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion Quaternion::Zero() {
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}


