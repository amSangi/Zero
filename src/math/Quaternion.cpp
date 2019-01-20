#include "Quaternion.hpp"
#include "Matrix3.hpp"
#include "Vector3.hpp"

using namespace Zero;

bool Quaternion::operator==(const Quaternion& other) const {
	return Equal(w_, other.w_) &&
			Equal(x_, other.x_) &&
			Equal(y_, other.y_) &&
			Equal(z_, other.z_);
}

bool Quaternion::operator!=(const Quaternion& other) const {
	return !operator==(other);
}

/* ********** Scalar Operations ********** */
Quaternion Quaternion::operator+(float scalar) {
	return Quaternion(w_ + scalar, x_ + scalar, y_ + scalar, z_ + scalar);
}

Quaternion Quaternion::operator-(float scalar) {
	return Quaternion(w_ - scalar, x_ - scalar, y_ - scalar, z_ - scalar);
}

Quaternion Quaternion::operator*(float scalar) {
	return Quaternion(w_ * scalar, x_ * scalar, y_ * scalar, z_ * scalar);
}

Quaternion Quaternion::operator/(float scalar) {
	float inv_scalar = 1.0f / scalar;
	return Quaternion(w_ * inv_scalar, x_ * inv_scalar, y_ * inv_scalar, z_ * inv_scalar);
}

Quaternion& Quaternion::operator+=(float scalar) {
	w_ += scalar;
	x_ += scalar;
	y_ += scalar;
	z_ += scalar;
	return *this;
}

Quaternion& Quaternion::operator-=(float scalar) {
	w_ -= scalar;
	x_ -= scalar;
	y_ -= scalar;
	z_ -= scalar;
	return *this;
}

Quaternion& Quaternion::operator*=(float scalar) {
	w_ *= scalar;
	x_ *= scalar;
	y_ *= scalar;
	z_ *= scalar;
	return *this;
}

Quaternion& Quaternion::operator/=(float scalar) {
	float inv_scalar = 1.0f / scalar;
	w_ *= inv_scalar;
	x_ *= inv_scalar;
	y_ *= inv_scalar;
	z_ *= inv_scalar;
	return *this;
}

/* ********** Quaternion/Quaternion Operations ********** */
Quaternion Quaternion::operator+(const Quaternion& rhs) {
	return Quaternion(w_ + rhs.w_, x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_);
}

Quaternion Quaternion::operator-(const Quaternion& rhs) {
	return Quaternion(w_ - rhs.w_, x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_);
}

Quaternion Quaternion::operator*(const Quaternion& rhs) {
	return Quaternion(
		(w_ * rhs.w_) - (x_ * rhs.x_) - (y_ * rhs.y_) - (z_ * rhs.z_),
		(w_ * rhs.x_) + (x_ * rhs.w_) + (y_ * rhs.z_) - (z_ * rhs.y_),
		(w_ * rhs.y_) - (x_ * rhs.z_) + (y_ * rhs.w_) + (z_ * rhs.x_),
		(w_ * rhs.z_) + (x_ * rhs.y_) - (y_ * rhs.x_) + (z_ * rhs.w_)
	);
}

Quaternion& Quaternion::operator+=(const Quaternion& rhs) {
	w_ += rhs.w_;
	x_ += rhs.x_;
	y_ += rhs.y_;
	z_ += rhs.z_;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& rhs) {
	w_ -= rhs.w_;
	x_ -= rhs.x_;
	y_ -= rhs.y_;
	z_ -= rhs.z_;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
	float new_w = (w_ * rhs.w_) - (x_ * rhs.x_) - (y_ * rhs.y_) - (z_ * rhs.z_);
	float new_x = (w_ * rhs.x_) + (x_ * rhs.w_) + (y_ * rhs.z_) - (z_ * rhs.y_);
	float new_y = (w_ * rhs.y_) - (x_ * rhs.z_) + (y_ * rhs.w_) + (z_ * rhs.x_);
	float new_z = (w_ * rhs.z_) + (x_ * rhs.y_) - (y_ * rhs.x_) + (z_ * rhs.w_);
	w_ = new_w;
	x_ = new_x;
	y_ = new_y;
	z_ = new_z;
	return *this;
}

Vector3 Quaternion::operator*(const Vector3& v) const {
	Vector3 u = XYZ();
	Vector3 c1 = Vector3::Cross(u, v);
	Vector3 c2 = Vector3::Cross(u, c1);
	return v + 2.0f * ((c1 * w_) + c2);
}

/* ********** Quaternion Operations ********** */
float Quaternion::Norm() const {
	return Sqrt((w_ * w_) + (x_ * x_) + (y_ * y_) + (z_ * z_));
}

Quaternion& Quaternion::UnitNorm() {
	float norm = Norm();
	if (norm > 0.0f) {
		float inv_norm = 1.0f / norm;
		w_ *= inv_norm;
		x_ *= inv_norm;
		y_ *= inv_norm;
		z_ *= inv_norm;
	}
	else {
		w_ = 1.0f;
		x_ = y_ = z_ = 0.0f;
	}
	return *this;
}

Quaternion& Quaternion::Conjugate() {
	x_ = -x_;
	y_ = -y_;
	z_ = -z_;
 	return *this;
}

Quaternion& Quaternion::Inverse() {
	float norm = Norm();
	if (norm > 0.0f) {
		float inv_norm = 1.0f / norm;
		w_ *= inv_norm;
		x_ *= -inv_norm;
		y_ *= -inv_norm;
		z_ *= -inv_norm;
	}
	else {
		w_ = 1.0f;
		x_ = y_ = z_ = 0.0f;
	}
	return *this;
}

Quaternion& Quaternion::Negate() {
	w_ = -w_;
	x_ = -x_;
	y_ = -y_;
	z_ = -z_;
	return *this;
}

Quaternion Quaternion::UnitNormCopy() const {
	return Quaternion(*this).UnitNorm();
}

Quaternion Quaternion::ConjugateCopy() const {
	return Quaternion(*this).Conjugate();
}

Quaternion Quaternion::InverseCopy() const {
	return Quaternion(*this).Inverse();
}

Vector3 Quaternion::GetEulerAngles() const {
	float ww = w_ * w_;
	float xx = x_ * x_;
	float yy = y_ * y_;
	float zz = z_ * z_;

	float unit = xx + yy + zz + ww;
	float test = x_ * y_ + z_ * w_;

	if (test > 0.499f * unit) {
		return Vector3(2.0f * Zero::Atan2(x_, w_), Zero::PI_2, 0.0f);
	}

	if (test < -0.499f * unit) {
		return Vector3(-2.0f * Zero::Atan2(x_, w_), -Zero::PI_2, 0.0f);
	}

	float heading = Zero::Atan2(2.0f * (y_ * w_ - x_ * z_), xx - yy - zz + ww);
	float attitude = Zero::Asin(2.0f * test / unit);
	float bank = Zero::Atan2(2.0f * (x_ * w_ - y_ * z_), -xx + yy - zz + ww);

	return Vector3(bank, heading, attitude);
}

Vector3 Quaternion::XYZ() const {
	return Vector3(x_, y_, z_);
}

Matrix3 Quaternion::GetRotationMatrix() const {
	Quaternion q = UnitNormCopy();
	if (q == Zero()) {
		return Matrix3::Identity();
	}

	float xx = q.x_ * q.x_;
	float yy = q.y_ * q.y_;
	float zz = q.z_ * q.z_;

	float xy = q.x_ * q.y_;
	float xz = q.x_ * q.z_;
	float yz = q.y_ * q.z_;

	float wx = q.w_ * q.x_;
	float wy = q.w_ * q.y_;
	float wz = q.w_ * q.z_;

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
	return (lhs.w_ * rhs.w_) + (lhs.x_ * rhs.x_) + (lhs.y_ * rhs.y_) + (lhs.z_ * rhs.z_);
}

Quaternion Quaternion::FromAngleAxis(const Vector3& axis, Radian angle) {
	Vector3 normalized = Vector3::Normalize(axis);
	float half_angle_rad = angle.rad_ * 0.5f;
	float sin_half_angle_rad = Zero::Sin(half_angle_rad);

	// Assume axis is normalized
	return Quaternion(Zero::Cos(half_angle_rad),
	                  normalized.x_ * sin_half_angle_rad,
	                  normalized.y_ * sin_half_angle_rad,
	                  normalized.z_ * sin_half_angle_rad);
}

Quaternion Quaternion::FromAngleAxis(const Vector3& axis, Degree angle) {
	return FromAngleAxis(axis, angle.ToRadian());
}

Quaternion Quaternion::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) {
	Matrix3 rotation_matrix(xAxis.x_, yAxis.x_, zAxis.x_,
                            xAxis.y_, yAxis.y_, zAxis.y_,
                            xAxis.z_, yAxis.z_, zAxis.z_);

	return FromMatrix3(rotation_matrix);
}

Quaternion Quaternion::FromEuler(Radian x, Radian y, Radian z) {
	float rx = 0.5f * x.rad_;
	float ry = 0.5f * y.rad_;
	float rz = 0.5f * z.rad_;

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
                      rotation_axis.x_ * inv_s,
                      rotation_axis.y_ * inv_s,
                      rotation_axis.z_ * inv_s);
}

/* ********** Useful Quaternions ********** */
Quaternion Quaternion::Identity() {
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion Quaternion::Zero() {
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}


