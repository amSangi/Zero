#include "math/Quaternion.hpp"
#include "math/Matrix3x3.hpp"
#include "math/Vector3.hpp"

using namespace zero::math;

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
Quaternion Quaternion::operator+(const Quaternion& rhs)  const {
	return Quaternion(w_ + rhs.w_, x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_);
}

Quaternion Quaternion::operator-(const Quaternion& rhs)  const {
	return Quaternion(w_ - rhs.w_, x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_);
}

Quaternion Quaternion::operator*(const Quaternion& rhs)  const {
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

Vec3f Quaternion::operator*(const Vec3f& v) const {
	Vec3f u = XYZ();
	Vec3f c1 = Vec3f::Cross(u, v);
	Vec3f c2 = Vec3f::Cross(u, c1);
	return v + 2.0f * ((c1 * w_) + c2);
}

/* ********** Quaternion Operations ********** */
float Quaternion::Norm() const {
	return Sqrt((w_ * w_) + (x_ * x_) + (y_ * y_) + (z_ * z_));
}

Quaternion& Quaternion::Unit() {
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

Quaternion Quaternion::UnitCopy() const {
	return Quaternion(*this).Unit();
}

Quaternion Quaternion::ConjugateCopy() const {
	return Quaternion(*this).Conjugate();
}

Quaternion Quaternion::InverseCopy() const {
	return Quaternion(*this).Inverse();
}

Vec3f Quaternion::GetEulerAngles() const {
	float ww = w_ * w_;
	float xx = x_ * x_;
	float yy = y_ * y_;
	float zz = z_ * z_;

	float unit = xx + yy + zz + ww;
	float test = x_ * y_ + z_ * w_;

	if (test > 0.499f * unit) {
		return Vec3f(2.0f * Atan2(x_, w_), PI_2, 0.0f);
	}

	if (test < -0.499f * unit) {
		return Vec3f(-2.0f * Atan2(x_, w_), -PI_2, 0.0f);
	}

	float heading = Atan2(2.0f * (y_ * w_ - x_ * z_), xx - yy - zz + ww);
	float attitude = Asin(2.0f * test / unit);
	float bank = Atan2(2.0f * (x_ * w_ - y_ * z_), -xx + yy - zz + ww);

	return Vec3f(bank, heading, attitude);
}

Vec3f Quaternion::XYZ() const {
	return Vec3f(x_, y_, z_);
}

Matrix3x3 Quaternion::GetRotationMatrix() const {
	Quaternion q = UnitCopy();
	if (q == Zero()) {
		return Matrix3x3::Identity();
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

	return Matrix3x3(e00, e01, e02,
                     e10, e11, e12,
                     e20, e21, e22);
}

/* ********** Static Operations ********** */
float Quaternion::Dot(const Quaternion& lhs, const Quaternion& rhs) {
	return (lhs.w_ * rhs.w_) + (lhs.x_ * rhs.x_) + (lhs.y_ * rhs.y_) + (lhs.z_ * rhs.z_);
}

Quaternion Quaternion::FromAngleAxis(const Vec3f& axis, Radian angle) {
	Vec3f normalized = Vec3f::Normalize(axis);
	float half_angle_rad = angle.rad_ * 0.5f;
	float sin_half_angle_rad = Sin(half_angle_rad);

	// Assume axis is normalized
	return Quaternion(Cos(half_angle_rad),
	                  normalized.x_ * sin_half_angle_rad,
	                  normalized.y_ * sin_half_angle_rad,
	                  normalized.z_ * sin_half_angle_rad);
}

Quaternion Quaternion::FromAngleAxis(const Vec3f& axis, Degree angle) {
	return FromAngleAxis(axis, angle.ToRadian());
}

Quaternion Quaternion::FromAxes(const Vec3f& xAxis, const Vec3f& yAxis, const Vec3f& zAxis) {
	Matrix3x3 rotation_matrix(xAxis.x_, yAxis.x_, zAxis.x_,
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

Quaternion Quaternion::FromMatrix3(const Matrix3x3& matrix) {
	float w = Sqrt(Max(0.0f, 1.0f + matrix(0, 0) + matrix(1, 1) + matrix(2, 2))) * 0.5f;
	float x = Sqrt(Max(0.0f, 1.0f + matrix(0, 0) - matrix(1, 1) - matrix(2, 2))) * 0.5f;
	float y = Sqrt(Max(0.0f, 1.0f - matrix(0, 0) + matrix(1, 1) - matrix(2, 2))) * 0.5f;
	float z = Sqrt(Max(0.0f, 1.0f - matrix(0, 0) - matrix(1, 1) + matrix(2, 2))) * 0.5f;

	x = Copysign(x, matrix(2, 1) - matrix(1, 2));
	y = Copysign(y, matrix(0, 2) - matrix(2, 0));
	z = Copysign(z, matrix(2, 0) - matrix(0, 2));

	return Quaternion(w, x, y, z);
}

Quaternion Quaternion::LookRotation(const Vec3f& direction, const Vec3f& up) {
	if (direction.SquareMagnitude() < EPSILON) {
		return Identity();
	}

	// Recompute desired up so that it is perpendicular to direction
	Vec3f right = Vec3f::Cross(direction, up);
	Vec3f recomputed_up = Vec3f::Cross(right, direction);

	Quaternion rot1 = FromToRotation(Vec3f::Forward(), direction);
	Vec3f new_up = rot1 * Vec3f::Up();
	Quaternion rot2 = FromToRotation(new_up, recomputed_up);

	return rot2 * rot1;
}

Quaternion Quaternion::FromToRotation(const Vec3f& from, const Vec3f& to) {
	Vec3f from_N = Vec3f::Normalize(from);
	Vec3f to_N = Vec3f::Normalize(to);

	float theta = Vec3f::Dot(from_N, to_N);
	Vec3f rotation_axis;

	if (theta < -1.0f + EPSILON) {
		rotation_axis = Vec3f::Cross(Vec3f::Forward(), from_N);

		if (rotation_axis.SquareMagnitude() < EPSILON) {
			rotation_axis = Vec3f::Cross(Vec3f::Right(), from_N);
		}

		rotation_axis.Normalize();
		return FromAngleAxis(rotation_axis, Degree(180.0f));
	}

	rotation_axis = Vec3f::Cross(from_N, to_N);
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


