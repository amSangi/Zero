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
		float Norm() const;
		Quaternion UnitNorm() const;
		Quaternion Conjugate() const;
		Quaternion Inverse() const;

		static float Dot(const Quaternion& lhs, const Quaternion& rhs);


	public:
		float w;
		float x, y, z;

	}; // class Quaternion

} // namespace Zero