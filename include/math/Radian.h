#pragma once

#include "ZeroMath.h"

namespace Zero {

	class Radian {
	public:
		Radian() : rad(0.0f) {}
		explicit Radian(float r) : rad(r) {}
		Radian(const Radian& other) = default;
		~Radian() = default;

		Radian& operator=(const Radian& other) = default;


		/* ********** Comparison Operators ********** */
		bool operator==(const Radian& r) const             { return Equal(rad, r.rad); }
		bool operator!=(const Radian& r) const             { return !Equal(rad, r.rad); }
		bool operator>=(const Radian& r) const             { return rad >= r.rad; }
		bool operator>(const Radian& r)  const             { return rad > r.rad;  }
		bool operator<=(const Radian& r) const             { return rad <= r.rad; }
		bool operator<(const Radian& r)  const             { return rad < r.rad;  }

		bool operator==(const Degree& d) const;
		bool operator!=(const Degree& d) const;
		bool operator>=(const Degree& d) const;
		bool operator>(const Degree& d)  const;
		bool operator<=(const Degree& d) const;
		bool operator<(const Degree& d)  const;

		/* ********** Math Operators ********** */
		Radian operator+(const Radian& r) const            { return Radian(rad + r.rad); }
		Radian operator-(const Radian& r) const            { return Radian(rad - r.rad); }
		Radian operator*(const Radian& r) const            { return Radian(rad * r.rad); }
		Radian operator/(const Radian& r) const            { return Radian(rad / r.rad); }

		Radian& operator+=(const Radian& r)                { rad += r.rad; return *this; }
		Radian& operator-=(const Radian& r)                { rad -= r.rad; return *this; }
		Radian& operator*=(const Radian& r)                { rad *= r.rad; return *this; }
		Radian& operator/=(const Radian& r)                { rad /= r.rad; return *this; }

		Radian operator+(const Degree& d) const;
		Radian operator-(const Degree& d) const;
		Radian operator*(const Degree& d) const;
		Radian operator/(const Degree& d) const;

		Radian& operator+=(const Degree& d);
		Radian& operator-=(const Degree& d);
		Radian& operator*=(const Degree& d);
		Radian& operator/=(const Degree& d);

		Degree ToDegree() const;

		static inline Radian FromDegree(float d)           { return Radian(d * DEGREE_TO_RADIAN); }

		float rad;

	}; // class Radian

} // namespace Zero