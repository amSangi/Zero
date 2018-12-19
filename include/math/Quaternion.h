#pragma once

#include "ZeroMath.h"

namespace Zero {

	class Quaternion {
	public:
		Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
		Quaternion(float w1, float x1, float y1, float z1) : w(w1), x(x1), y(y1), z(z1) {}
		~Quaternion() = default;
		Quaternion(const Quaternion& other) = default;

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
		float Norm();
		Quaternion& Square();
		Quaternion& UnitNorm();
		Quaternion& Conjugate();
		Quaternion& Inverse();


		/* ********** Static Operations ********** */
		static Quaternion SquareCopy(const Quaternion& quat);
		static Quaternion UnitNormCopy(const Quaternion& quat);
		static Quaternion InverseCopy(const Quaternion& quat);

	public:
		float w;
		float x, y, z;

	}; // class Quaternion

} // namespace Zero