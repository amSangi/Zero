#pragma once

#include "Radian.hpp"
#include "ZMath.hpp"

namespace zero {
namespace math {

	/**
	 * @brief Encapsulate a degree as a class for type-safety
	 */
	class Degree {
	public:
		Degree() : deg_(0.0f) {}
		explicit Degree(float d) : deg_(d) {}
		Degree(const Degree& other) = default;
		~Degree() = default;

		Degree& operator=(const Degree& other) = default;


		/* ********** Comparison Operators ********** */
		bool operator==(const Degree& d) const             { return Equal(deg_, d.deg_); }
		bool operator!=(const Degree& d) const             { return !Equal(deg_, d.deg_); }
		bool operator>=(const Degree& d) const             { return deg_ >= d.deg_; }
		bool operator>(const Degree& d)  const             { return deg_ > d.deg_;  }
		bool operator<=(const Degree& d) const             { return deg_ <= d.deg_; }
		bool operator<(const Degree& d)  const             { return deg_ < d.deg_;  }

		bool operator==(const Radian& r) const             { return operator==(r.ToDegree()); }
		bool operator!=(const Radian& r) const             { return operator!=(r.ToDegree()); }
		bool operator>=(const Radian& r) const             { return operator>=(r.ToDegree()); }
		bool operator>(const Radian& r)  const             { return operator>(r.ToDegree());  }
		bool operator<=(const Radian& r) const             { return operator<=(r.ToDegree()); }
		bool operator<(const Radian& r)  const             { return operator<(r.ToDegree());  }

		/* ********** Math Operators ********** */
		Degree operator+(const Degree& d) const            { return Degree(deg_ + d.deg_); }
		Degree operator-(const Degree& d) const            { return Degree(deg_ - d.deg_); }
		Degree operator*(const Degree& d) const            { return Degree(deg_ * d.deg_); }
		Degree operator/(const Degree& d) const            { return Degree(deg_ / d.deg_); }

		Degree& operator+=(const Degree& d)                { deg_ += d.deg_; return *this; }
		Degree& operator-=(const Degree& d)                { deg_ -= d.deg_; return *this; }
		Degree& operator*=(const Degree& d)                { deg_ *= d.deg_; return *this; }
		Degree& operator/=(const Degree& d)                { deg_ /= d.deg_; return *this; }

		Degree operator+(const Radian& r) const            { return Degree(deg_ + r.ToDegree().deg_); }
		Degree operator-(const Radian& r) const            { return Degree(deg_ - r.ToDegree().deg_); }
		Degree operator*(const Radian& r) const            { return Degree(deg_ * r.ToDegree().deg_); }
		Degree operator/(const Radian& r) const            { return Degree(deg_ / r.ToDegree().deg_); }

		Degree& operator+=(const Radian& r)                { deg_ += r.ToDegree().deg_; return *this; }
		Degree& operator-=(const Radian& r)                { deg_ -= r.ToDegree().deg_; return *this; }
		Degree& operator*=(const Radian& r)                { deg_ *= r.ToDegree().deg_; return *this; }
		Degree& operator/=(const Radian& r)                { deg_ /= r.ToDegree().deg_; return *this; }

		/**
		 * @brief Convert this Degree object into a Radian object
		 * @return The Radian object
		 */
		Radian ToRadian() const                            { return Radian(deg_ * DEGREE_TO_RADIAN); }

		/**
		 * @brief Create a Degree from a radian value
		 * @param r The radian value
		 * @return The Degree
		 */
		static inline Degree FromRadian(float r)           { return Degree(r * RADIAN_TO_DEGREE); }

		/**
		 * @brief The degree value
		 */
		float deg_;

	}; // class Degree

} // namespace math
} // namespace zero