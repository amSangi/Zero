#pragma once

#include "ZeroMath.h"

namespace Zero {

	class Quaternion {
	public:
		Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
		Quaternion(float w1, float x1, float y1, float z1) : w(w1), x(x1), y(y1), z(z1) {}
		Quaternion(const Quaternion& other) = default;
		~Quaternion() = default;

		Quaternion& operator=(const Quaternion& other) = default;

		inline bool operator==(const Quaternion& other);
		inline bool operator!=(const Quaternion& other);


		/* ********** Scalar Operations ********** */
		Quaternion operator+(float scalar);
		Quaternion operator-(float scalar);
		Quaternion operator*(float scalar);

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


		/* ********** Quaternion Operations ********** */
		float Norm() const;
		Quaternion& UnitNorm();
		Quaternion& Conjugate();
		Quaternion& Inverse();
		Quaternion& Negate();
		Quaternion& RotateX(Radian angle);
		Quaternion& RotateY(Radian angle);
		Quaternion& RotateZ(Radian angle);

		Quaternion UnitNormCopy() const;
		Quaternion ConjugateCopy() const;
		Quaternion InverseCopy() const;

		Vector3 GetEulerAngles() const;
		Vector3 xyz() const;
		Matrix3 GetRotationMatrix3() const;


		/* ********** Static Operations ********** */
		static float Dot(const Quaternion& lhs, const Quaternion& rhs);


		static Quaternion FromAxisAngle(const Vector3& axis, Radian angle);
		static Quaternion FromAxisAngle(const Vector3& axis, Degree angle);
		static Quaternion FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
		static Quaternion FromEuler(float x, float y, float z);
		static Quaternion FromMatrix3(const Matrix3& matrix);
		static Quaternion LookRotation(const Vector3& forward, const Vector3& up);
		static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection);
		static Quaternion RotateTowards(Quaternion from, Quaternion to, Degree maxDegreesDelta);

		/* ********** Useful Quaternions ********** */
		static Quaternion Identity();
		static Quaternion Zero();

	public:
		float w;
		float x, y, z;

	}; // class Quaternion

} // namespace Zero