#pragma once

#include "ZMath.hpp"

namespace zero {

	/**
	 * @brief Encapsulate a radian as a class for type-safety
	 */
	class Radian {
	public:
		Radian() : rad_(0.0f) {}
		explicit Radian(float r) : rad_(r) {}
		Radian(const Radian& other) = default;
		~Radian() = default;

		Radian& operator=(const Radian& other) = default;


		/* ********** Comparison Operators ********** */
		bool operator==(const Radian& r) const             { return Equal(rad_, r.rad_); }
		bool operator!=(const Radian& r) const             { return !Equal(rad_, r.rad_); }
		bool operator>=(const Radian& r) const             { return rad_ >= r.rad_; }
		bool operator>(const Radian& r)  const             { return rad_ > r.rad_;  }
		bool operator<=(const Radian& r) const             { return rad_ <= r.rad_; }
		bool operator<(const Radian& r)  const             { return rad_ < r.rad_;  }

		bool operator==(const Degree& d) const;
		bool operator!=(const Degree& d) const;
		bool operator>=(const Degree& d) const;
		bool operator>(const Degree& d)  const;
		bool operator<=(const Degree& d) const;
		bool operator<(const Degree& d)  const;

		/* ********** Math Operators ********** */
		Radian operator+(const Radian& r) const            { return Radian(rad_ + r.rad_); }
		Radian operator-(const Radian& r) const            { return Radian(rad_ - r.rad_); }
		Radian operator*(const Radian& r) const            { return Radian(rad_ * r.rad_); }
		Radian operator/(const Radian& r) const            { return Radian(rad_ / r.rad_); }

		Radian& operator+=(const Radian& r)                { rad_ += r.rad_; return *this; }
		Radian& operator-=(const Radian& r)                { rad_ -= r.rad_; return *this; }
		Radian& operator*=(const Radian& r)                { rad_ *= r.rad_; return *this; }
		Radian& operator/=(const Radian& r)                { rad_ /= r.rad_; return *this; }

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

		float rad_;

	}; // class Radian

} // namespace zero