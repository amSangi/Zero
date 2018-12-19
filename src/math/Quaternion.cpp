#include "Quaternion.h"

using namespace Zero;


float Quaternion::Norm() {
	return sqrt((w * w) + (x * x) + (y * y) + (z * z));
}

Quaternion& Quaternion::Square() {
	float w_sq = (w * w) - (x * x) - (y * y) - (z * z);
	float w2 = (2 * w);

	x *= w2;
	y *= w2;
	z *= w2;
	w = w_sq;

	return *this;
}

Quaternion& Quaternion::UnitNorm() {
	operator/=(Norm());
	return *this;
}

Quaternion& Quaternion::Conjugate() {
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
	return *this;
}

Quaternion& Quaternion::Inverse() {
	Conjugate(); 
	operator/=((w * w) + (x * x) + (y * y) + (z * z));
	return *this;
}

Quaternion Quaternion::SquareCopy(const Quaternion& quat) {
	Quaternion q = quat;
	return q.Square();
}

Quaternion Quaternion::UnitNormCopy(const Quaternion& quat) {
	Quaternion q = quat;
	return q.UnitNorm();
}

Quaternion Quaternion::InverseCopy(const Quaternion& quat) {
	Quaternion q = quat;
	return q.Inverse();
}


bool Quaternion::operator==(const Quaternion& other) {
	return equal(w, other.w) && equal(x, other.x) && equal(y, other.y) && equal(z, other.z);
}

bool Quaternion::operator!=(const Quaternion& other) {
	return !operator==(other);
}

Quaternion Quaternion::operator+(float scalar) {
	Quaternion q = *this;
	q.w += scalar;
	q.x += scalar;
	q.y += scalar;
	q.z += scalar;
	return q;
}

Quaternion Quaternion::operator-(float scalar) {
	Quaternion q = *this;
	q.w -= scalar;
	q.x -= scalar;
	q.y -= scalar;
	q.z -= scalar;
	return q;
}

Quaternion Quaternion::operator*(float scalar) {
	Quaternion q = *this;
	q.w *= scalar;
	q.x *= scalar;
	q.y *= scalar;
	q.z *= scalar;
	return q;
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

Quaternion Quaternion::operator+(const Quaternion& rhs) {
	Quaternion q = *this;
	q.w += rhs.w;
	q.x += rhs.x;
	q.y += rhs.y;
	q.z += rhs.z;
	return q;
}

Quaternion Quaternion::operator-(const Quaternion& rhs) {
	Quaternion q = *this;
	q.w -= rhs.w;
	q.x -= rhs.x;
	q.y -= rhs.y;
	q.z -= rhs.z;
	return q;
}

Quaternion Quaternion::operator*(const Quaternion& rhs) {
	Quaternion q = *this;
	q.w *= rhs.w;
	q.x *= rhs.x;
	q.y *= rhs.y;
	q.z *= rhs.z;
	return q;
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
	w *= rhs.w;
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}

