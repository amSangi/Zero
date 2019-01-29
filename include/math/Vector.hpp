#pragma once

#include "Angle.hpp"
#include "ZMath.hpp"

namespace zero {
namespace math {

	namespace detail {

		/**
		 * @brief A simple base container for the vector
		 * @tparam dims the dimensions of the vector
		 */
		template<int dims>
		class VectorBase {
		public:
			VectorBase() = default;

			explicit VectorBase(float value) {
				for (int i = 0; i < dims; ++i) {
					data_[i] = value;
				}
			}

			VectorBase(float x, float y) {
				static_assert(dims > 1, "Require at least 2 dimensions");
				data_[0] = x;
				data_[1] = y;
			}

			VectorBase(float x, float y, float z) {
				static_assert(dims > 2, "Require at least 3 dimensions");
				data_[0] = x;
				data_[1] = y;
				data_[2] = z;
			}

			VectorBase(float x, float y, float z, float w) {
				static_assert(dims > 3, "Require at least 4 dimensions");
				data_[0] = x;
				data_[1] = y;
				data_[2] = z;
				data_[3] = w;
			}

			inline float *Data() { return data_; }

			inline const float *Data() const { return data_; }

			float data_[dims];

		}; // template class VectorBase

		template<>
		class VectorBase<2> {
		public:
			VectorBase() = default;

			VectorBase(float x, float y) : x_(x), y_(y) {}

			explicit VectorBase(float value) : x_(value), y_(value) {}


			inline float *Data() { return &x_; }

			inline const float *Data() const { return &x_; }

			float x_, y_;

		}; // template specialization class VectorBase<2>


		template<>
		class VectorBase<3> {
		public:
			VectorBase() = default;

			VectorBase(float x, float y) : x_(x), y_(y), z_(0.0f) {}

			VectorBase(float x, float y, float z) : x_(x), y_(y), z_(z) {}

			explicit VectorBase(float value) : x_(value), y_(value), z_(value) {}


			inline float *Data() { return &x_; }

			inline const float *Data() const { return &x_; }

			float x_, y_, z_;

		}; // template specialization class VectorBase<3>


		template<>
		class VectorBase<4> {
		public:
			VectorBase() = default;

			VectorBase(float x, float y) : x_(x), y_(y), z_(0.0f), w_(0.0f) {}

			VectorBase(float x, float y, float z) : x_(x), y_(y), z_(z), w_(0.0f) {}

			VectorBase(float x, float y, float z, float w) : x_(x), y_(y), z_(z), w_(w) {}

			explicit VectorBase(float value) : x_(value), y_(value), z_(value), w_(value) {}

			inline float *Data() { return &x_; }

			inline const float *Data() const { return &x_; }

			float x_, y_, z_, w_;

		}; // template specialization class VectorBase<4>

	} // namespace detail


	/**
	 * @brief An dims sized mathematical vector
	 * @tparam dims The vector dimensions
	 */
	template<int dims>
	class Vector  : public detail::VectorBase<dims> {
		using VectorBase = detail::VectorBase<dims>;
		using VectorBase::Data;
	public:
		Vector<dims>() = default;
		Vector<dims>(const Vector<dims>& other) = default;
		explicit Vector<dims>(float value) : VectorBase(value) {}
		Vector<dims>(float x, float y) : VectorBase(x, y) {};
		Vector<dims>(float x, float y, float z) : VectorBase(x, y, z) {}
		Vector<dims>(float x, float y, float z, float w) : VectorBase(x, y, z, w) {}

		~Vector<dims>() = default;
		Vector<dims>& operator=(const Vector<dims>& other) = default;

		float operator[](size_t index);
		const float operator[](size_t index) const;

		inline bool operator==(const Vector<dims>& o) const;
		inline bool operator!=(const Vector<dims>& o) const;

		/* ********** Scalar Operations ********** */
		Vector<dims> operator+(float scalar) const;
		Vector<dims> operator-(float scalar) const;
		Vector<dims> operator*(float scalar) const;
		Vector<dims> operator/(float scalar) const;

		Vector<dims>& operator+=(float scalar);
		Vector<dims>& operator-=(float scalar);
		Vector<dims>& operator*=(float scalar);
		Vector<dims>& operator/=(float scalar);

		/* **** Component-Wise Vector Operations **** */
		Vector<dims> operator+(const Vector<dims>& rhs) const;
		Vector<dims> operator-(const Vector<dims>& rhs) const;
		Vector<dims> operator*(const Vector<dims>& rhs) const;
		Vector<dims> operator/(const Vector<dims>& rhs) const;

		Vector<dims>& operator+=(const Vector<dims>& rhs);
		Vector<dims>& operator-=(const Vector<dims>& rhs);
		Vector<dims>& operator*=(const Vector<dims>& rhs);
		Vector<dims>& operator/=(const Vector<dims>& rhs);

		/* ********** Vector Operations ********** */
		float Magnitude() const;

		float SquareMagnitude() const;

		Vector<dims>& Abs();

		bool Normalize();

		bool IsEpsilon(float epsilon = EPSILON) const;

		/* ********** Static Operations ********** */
		static inline Vector<dims> Normalize(const Vector<dims>& v);

		static inline Vector<dims> AbsCopy(const Vector<dims>& v);

		static inline float Dot(const Vector<dims>& lhs, const Vector<dims>& rhs);

		static inline Vector<dims> Cross(const Vector<dims>& lhs, const Vector<dims>& rhs);

		static inline Vector<dims> Reflect(const Vector<dims>& in, const Vector<dims>& normal);

		static inline float Distance(const Vector<dims>& from, const Vector<dims>& to);

		static inline float SquareDistance(const Vector<dims>& from, const Vector<dims>& to);

		static inline Vector<dims> Lerp(const Vector<dims>& start, const Vector<dims>& end, float t);

		static inline Radian Angle(const Vector<dims>& from, const Vector<dims>& to);


		/* ********** Useful Vectors ********** */
		static Vector<dims> Zero();
		static Vector<dims> One();
		static Vector<dims> Up();
		static Vector<dims> Down();
		static Vector<dims> Right();
		static Vector<dims> Left();
		static Vector<dims> Forward();
		static Vector<dims> Back();

	}; // template class Vector

	/* ********** Useful Vectors ********** */

	template<int dims>
	Vector<dims> Vector<dims>::Zero() {
		return Vector<dims>(0.0f);
	}

	template<int dims>
	Vector<dims> Vector<dims>::One() {
		return Vector<dims>(1.0f);
	}

	template<int dims>
	Vector<dims> Vector<dims>::Up() {
		static_assert(dims > 1, "Require at least 2 dimensions");
		return Vector<dims>(0.0f, 1.0f);
	}

	template<int dims>
	Vector<dims> Vector<dims>::Down() {
		static_assert(dims > 1, "Require at least 2 dimensions");
		return Vector<dims>(0.0f, -1.0f);
	}

	template<int dims>
	Vector<dims> Vector<dims>::Right() {
		static_assert(dims > 1, "Require at least 2 dimensions");
		return Vector<dims>(1.0f, 0.0f);
	}

	template<int dims>
	Vector<dims> Vector<dims>::Left() {
		static_assert(dims > 1, "Require at least 2 dimensions");
		return Vector<dims>(-1.0f, 0.0f);
	}

	template<int dims>
	Vector<dims> Vector<dims>::Forward() {
		static_assert(dims > 2, "Require at least 3 dimensions");
		return Vector<dims>(0.0f, 0.0f, 1.0f);
	}

	template<int dims>
	Vector<dims> Vector<dims>::Back() {
		static_assert(dims > 2, "Require at least 3 dimensions");
		return Vector<dims>(0.0f, 0.0f, -1.0f);
	}


	/* ********** Vector Operations Implementation ********** */

	template<int dims>
	float Vector<dims>::Magnitude() const {
		return Sqrt(SquareMagnitude());
	}

	template<int dims>
	float Vector<dims>::SquareMagnitude() const {
		float square_mag = 0.0f;
		const float* data = Data();
		for (int i = 0; i < dims; ++i) {
			square_mag += (data[i] * data[i]);
		}
		return square_mag;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::Abs() {
		float* data = Data();
		for (int i = 0; i < dims; ++i) {
			data[i] = math::Abs(data[i]);
		}
		return *this;
	}

	template<int dims>
	bool Vector<dims>::Normalize() {
		float magnitude = Magnitude();

		float inv_magnitude = 1.0f / magnitude;
		if (magnitude > SMALL_EPSILON) {
			operator*=(inv_magnitude);
			return true;
		}

		return false;
	}

	template<int dims>
	bool Vector<dims>::IsEpsilon(float epsilon) const {
		const float* data = Data();
		for (int i = 0; i < dims; ++i) {
			if (math::Abs(data[i]) > epsilon) {
				return false;
			}
		}

		return true;
	}

	/* ********** Static Operations Implementation ********** */

	template<int dims>
	Vector<dims> Vector<dims>::Normalize(const Vector<dims>& v) {
		Vector<dims> copy = v;
		return copy.Normalize() ? copy : Zero();
	}

	template<int dims>
	Vector<dims> Vector<dims>::AbsCopy(const Vector<dims>& v) {
		Vector<dims> copy = v;
		copy.Abs();
		return copy;
	}

	template<int dims>
	float Vector<dims>::Dot(const Vector<dims>& lhs, const Vector<dims>& rhs) {
		float result = 0.0f;
		const float* lhs_data = lhs.Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			result += (lhs_data[i] * rhs_data[i]);
		}
		return result;
	}

	template<int dims>
	Vector<dims> Vector<dims>::Cross(const Vector<dims>& lhs, const Vector<dims>& rhs) {
		static_assert(dims == 3, "Only 3D cross product supported");
		return Vector<dims>(lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
                            lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
                            lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_);
	}

	template<int dims>
	Vector<dims> Vector<dims>::Reflect(const Vector<dims>& in, const Vector<dims>& normal) {
		return in - (normal * 2 * Dot(in, normal));
	}

	template<int dims>
	float Vector<dims>::Distance(const Vector<dims>& from, const Vector<dims>& to) {
		return (from - to).Magnitude();
	}

	template<int dims>
	float Vector<dims>::SquareDistance(const Vector<dims>& from, const Vector<dims>& to) {
		return (from - to).SquareMagnitude();
	}

	template<int dims>
	Vector<dims> Vector<dims>::Lerp(const Vector<dims>& start, const Vector<dims>& end, float t) {
		return (start * (1.0f - t)) + (end * t);
	}

	template<int dims>
	Radian Vector<dims>::Angle(const Vector<dims>& from, const Vector<dims>& to) {
		float dot = Dot(from, to);
		float square_mag_from = from.SquareMagnitude();
		float square_mag_to = to.SquareMagnitude();
		float inv_sqrt = 1.0f / Sqrt(square_mag_from * square_mag_to);
		float angle = Acos(dot * inv_sqrt);
		return Radian(angle);
	}


	/* ********** Operator Overload Implementation ********** */

	template<int dims>
	bool Vector<dims>::operator==(const Vector<dims>& o) const {
		const float* data = Data();
		const float* rhs_data = o.Data();
		for (int i = 0; i < dims; ++i) {
			if (!Equal(data[i], rhs_data[i])) return false;
		}

		return true;
	}

	template<int dims>
	float Vector<dims>::operator[](size_t index) {
		return Data()[index];
	}

	template<int dims>
	const float Vector<dims>::operator[](size_t index) const {
		return Data()[index];
	}

	template<int dims>
	bool Vector<dims>::operator!=(const Vector<dims>& o) const {
		return !operator==(o);
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator+(float scalar) const {
		return Vector<dims>(*this) += scalar;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator-(float scalar) const {
		return Vector<dims>(*this) -= scalar;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator*(float scalar) const {
		return Vector<dims>(*this) *= scalar;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator/(float scalar) const {
		return Vector<dims>(*this) /= scalar;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::operator+=(float scalar) {
		float* data = Data();
		for (int i = 0; i < dims; ++i) {
			data[i] += scalar;
		}
		return *this;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::operator-=(float scalar) {
		float* data = Data();
		for (int i = 0; i < dims; ++i) {
			data[i] -= scalar;
		}
		return *this;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::operator*=(float scalar) {
		float* data = Data();
		for (int i = 0; i < dims; ++i) {
			data[i] *= scalar;
		}
		return *this;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::operator/=(float scalar) {
		float* data = Data();
		float inv_scalar = 1.0f / scalar;
		for (int i = 0; i < dims; ++i) {
			data[i] *= inv_scalar;
		}
		return *this;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator+(const Vector<dims>& rhs) const {
		return Vector<dims>(*this) += rhs;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator-(const Vector<dims>& rhs) const {
		return Vector<dims>(*this) -= rhs;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator*(const Vector<dims>& rhs) const {
		return Vector<dims>(*this) *= rhs;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator/(const Vector<dims>& rhs) const {
		return Vector<dims>(*this) /= rhs;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::operator+=(const Vector<dims>& rhs) {
		float* data = Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			data[i] += rhs_data[i];
		}
		return *this;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::operator-=(const Vector<dims>& rhs) {
		float* data = Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			data[i] -= rhs_data[i];
		}
		return *this;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::operator*=(const Vector<dims>& rhs) {
		float* data = Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			data[i] *= rhs_data[i];
		}
		return *this;
	}

	template<int dims>
	Vector<dims>& Vector<dims>::operator/=(const Vector<dims>& rhs) {
		float* data = Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			data[i] /= rhs_data[i];
		}
		return *this;
	}


	// Scalar/Vector Operations
	template<int dims>
	inline Vector<dims> operator+(float scalar, const Vector<dims>& v) {
		return v + scalar;
	}

	template<int dims>
	inline Vector<dims> operator-(float scalar, const Vector<dims>& v) {
		return v - scalar;
	}

	template<int dims>
	inline Vector<dims> operator*(float scalar, const Vector<dims>& v) {
		return v * scalar;
	}

	template<int dims>
	inline Vector<dims> operator/(float scalar, const Vector<dims>& v) {
		return v / scalar;
	}

} // namespace math
} // namespace zero