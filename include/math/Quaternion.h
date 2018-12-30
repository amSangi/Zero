#pragma once

#include "Angle.h"
#include "Vector3.h"
#include "ZeroMath.h"

namespace Zero {

	class Quaternion {
	public:
		Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
		Quaternion(float w1, float x1, float y1, float z1) : w(w1), x(x1), y(y1), z(z1) {}
		Quaternion(const Quaternion& other) = default;
		~Quaternion() = default;

		Quaternion& operator=(const Quaternion& other) = default;

		bool operator==(const Quaternion& other) const;
		bool operator!=(const Quaternion& other) const;


		/* ********** Scalar Operations ********** */
		Quaternion operator+(float scalar);
		Quaternion operator-(float scalar);
		Quaternion operator*(float scalar);
		Quaternion operator/(float scalar);

		Quaternion& operator+=(float scalar);
		Quaternion& operator-=(float scalar);
		Quaternion& operator*=(float scalar);
		Quaternion& operator/=(float scalar);


		/* ********** Quaternion/Quaternion Operations ********** */
		Quaternion operator+(const Quaternion& rhs);
		Quaternion operator-(const Quaternion& rhs);
		Quaternion operator*(const Quaternion& rhs);

		Quaternion& operator+=(const Quaternion& rhs);
		Quaternion& operator-=(const Quaternion& rhs);
		Quaternion& operator*=(const Quaternion& rhs);

		/* ********** Quaternion/Vector3 Operations ********** */
		Vector3 operator*(const Vector3& v) const;

		/* ********** Quaternion Operations ********** */
		float Norm() const;
		Quaternion& UnitNorm();
		Quaternion& Conjugate();
		Quaternion& Inverse();
		Quaternion& Negate();

		Quaternion UnitNormCopy() const;
		Quaternion ConjugateCopy() const;
		Quaternion InverseCopy() const;

		Vector3 GetEulerAngles() const;
		Vector3 XYZ() const;
		Matrix3 GetRotationMatrix() const;


		/* ********** Static Operations ********** */
		static float Dot(const Quaternion& lhs, const Quaternion& rhs);


		static Quaternion FromAngleAxis(const Vector3& axis, Radian angle);
		static Quaternion FromAngleAxis(const Vector3& axis, Degree angle);
		static Quaternion FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
		static Quaternion FromEuler(Radian x, Radian y, Radian z);
		static Quaternion FromMatrix3(const Matrix3& matrix);
		static Quaternion LookRotation(const Vector3& direction, const Vector3& up = Vector3::Up());
		static Quaternion FromToRotation(const Vector3& from, const Vector3& to);

		/* ********** Useful Quaternions ********** */
		static Quaternion Identity();
		static Quaternion Zero();

	public:
		float w;
		float x, y, z;

	}; // class Quaternion

} // namespace Zero