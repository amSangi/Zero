#pragma once

#include "Radian.h"
#include "ZeroMath.h"

namespace Zero {

	class Degree {
	public:
		Degree() : deg(0.0f) {}
		explicit Degree(float d) : deg(d) {}
		Degree(const Degree& other) = default;
		~Degree() = default;

		Degree& operator=(const Degree& other) = default;


		/* ********** Comparison Operators ********** */
		bool operator==(const Degree& d) const             { return equal(deg, d.deg); }
		bool operator!=(const Degree& d) const             { return !equal(deg, d.deg); }
		bool operator>=(const Degree& d) const             { return deg >= d.deg; }
		bool operator>(const Degree& d)  const             { return deg > d.deg;  }
		bool operator<=(const Degree& d) const             { return deg <= d.deg; }
		bool operator<(const Degree& d)  const             { return deg < d.deg;  }

		bool operator==(const Radian& r) const             { return operator==(r.ToDegree()); }
		bool operator!=(const Radian& r) const             { return operator!=(r.ToDegree()); }
		bool operator>=(const Radian& r) const             { return operator>=(r.ToDegree()); }
		bool operator>(const Radian& r)  const             { return operator>(r.ToDegree());  }
		bool operator<=(const Radian& r) const             { return operator<=(r.ToDegree()); }
		bool operator<(const Radian& r)  const             { return operator<(r.ToDegree());  }

		/* ********** Math Operators ********** */
		Degree operator+(const Degree& d) const            { return Degree(deg + d.deg); }
		Degree operator-(const Degree& d) const            { return Degree(deg - d.deg); }
		Degree operator*(const Degree& d) const            { return Degree(deg * d.deg); }
		Degree operator/(const Degree& d) const            { return Degree(deg / d.deg); }

		Degree& operator+=(const Degree& d)                { deg += d.deg; return *this; }
		Degree& operator-=(const Degree& d)                { deg -= d.deg; return *this; }
		Degree& operator*=(const Degree& d)                { deg *= d.deg; return *this; }
		Degree& operator/=(const Degree& d)                { deg /= d.deg; return *this; }

		Degree operator+(const Radian& r) const            { return Degree(deg + r.ToDegree().deg); }
		Degree operator-(const Radian& r) const            { return Degree(deg - r.ToDegree().deg); }
		Degree operator*(const Radian& r) const            { return Degree(deg * r.ToDegree().deg); }
		Degree operator/(const Radian& r) const            { return Degree(deg / r.ToDegree().deg); }

		Degree& operator+=(const Radian& r)                { deg += r.ToDegree().deg; return *this; }
		Degree& operator-=(const Radian& r)                { deg -= r.ToDegree().deg; return *this; }
		Degree& operator*=(const Radian& r)                { deg *= r.ToDegree().deg; return *this; }
		Degree& operator/=(const Radian& r)                { deg /= r.ToDegree().deg; return *this; }

		Radian ToRadian() const                            { return Radian(deg * DEGREE_TO_RADIAN); }

		static inline Degree FromRadian(float r)           { return Degree(r * RADIAN_TO_DEGREE); }

		float deg;

	}; // class Degree

} // namespace Zero