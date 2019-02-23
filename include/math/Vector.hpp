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

		/**
		 * @brief Get the component at the index
		 * @param index The index
		 * @return the component
		 */
		float operator[](size_t index);

		/**
		 * @brief Get the component at the index
		 * @param index The index
		 * @return the component
		 */
		const float operator[](size_t index) const;

		/**
		 * @brief Check if the two given vectors are equal
		 * @param o The other vector
		 * @return True if the components are equal. False otherwise.
		 */
		inline bool operator==(const Vector<dims>& o) const;

		/**
		 * @brief Check if the two given vectors are not equal
		 * @param o The other vector
		 * @return True if the components are not equal. False otherwise.
		 */
		inline bool operator!=(const Vector<dims>& o) const;

		/* ********** Scalar Operations ********** */

		/**
		 * @brief Add the components by the given scalar
		 * @param scalar The scalar
		 * @return A new Vector containing the sums
		 */
		Vector<dims> operator+(float scalar) const;

		/**
		 * @brief Subtract the components by the given scalar
		 * @param scalar The scalar
		 * @return A new Vector containing the differences
		 */
		Vector<dims> operator-(float scalar) const;

		/**
		 * @brief Multiply the components by the given scalar
		 * @param scalar The scalar
		 * @return A new Vector containing the products
		 */
		Vector<dims> operator*(float scalar) const;

		/**
		 * @brief Divide the components by the given scalar
		 * @param scalar The scalar
		 * @return A new Vector containing the quotients
		 */
		Vector<dims> operator/(float scalar) const;

		/**
		 * @brief Add the components by the given scalar
		 * @param scalar The scalar
		 * @return The same Vector containing the sums
		 */
		Vector<dims>& operator+=(float scalar);

		/**
		 * @brief Subtract the components by the given scalar
		 * @param scalar The scalar
		 * @return The same Vector containing the differences
		 */
		Vector<dims>& operator-=(float scalar);

		/**
		 * @brief Multiply the components by the given scalar
		 * @param scalar The scalar
		 * @return The same Vector containing the products
		 */
		Vector<dims>& operator*=(float scalar);

		/**
		 * @brief Divide the components by the given scalar
		 * @param scalar The scalar
		 * @return The same Vector containing the quotients
		 */
		Vector<dims>& operator/=(float scalar);

		/* **** Component-Wise Vector Operations **** */

		/**
		 * @brief Add the components of this with the components of rhs
		 * @param rhs The right vector
		 * @return A new Vector containing the sums
		 */
		Vector<dims> operator+(const Vector<dims>& rhs) const;

		/**
		 * @brief Subtract the components of this by the components of rhs
		 * @param rhs The right vector
		 * @return A new Vector containing the differences
		 */
		Vector<dims> operator-(const Vector<dims>& rhs) const;

		/**
		 * @brief Multiply the components of this by the components of rhs
		 * @param rhs The right vector
		 * @return A new Vector containing the products
		 */
		Vector<dims> operator*(const Vector<dims>& rhs) const;

		/**
		 * @brief Divide the components of this by the components of rhs
		 * @param rhs The right vector
		 * @return A new Vector containing the quotients
		 */
		Vector<dims> operator/(const Vector<dims>& rhs) const;

		/**
		 * @brief Add the components of this with the components of rhs
		 * @param rhs The right vector
		 * @return The same Vector containing the sums
		 */
		Vector<dims>& operator+=(const Vector<dims>& rhs);

		/**
		 * @brief Subtract the components of this by the components of rhs
		 * @param rhs The right vector
		 * @return The same Vector containing the differences
		 */
		Vector<dims>& operator-=(const Vector<dims>& rhs);

		/**
		 * @brief Multiply the components of this by the components of rhs
		 * @param rhs The right vector
		 * @return The same Vector containing the products
		 */
		Vector<dims>& operator*=(const Vector<dims>& rhs);

		/**
		 * @brief Divide the components of this by the components of rhs
		 * @param rhs The right vector
		 * @return The same Vector containing the quotients
		 */
		Vector<dims>& operator/=(const Vector<dims>& rhs);

		/* ********** Vector Operations ********** */

		/**
		 * @brief Get the magnitude
		 * @return the magnitude
		 */
		float Magnitude() const;

		/**
		 * @brief Get the squared magnitude
		 * @return the square magnitude
		 */
		float SquareMagnitude() const;

		/**
		 * @brief Take the absolute value of all the components
		 * @return The same vector (this)
		 */
		Vector<dims>& Abs();

		/**
		 * @brief Normalize the vector if it's possible
		 * @return True if magnitude is > EPSILON. False otherwise.
		 */
		bool Normalize();

		/**
		 * @brief Check to see if all of the components are less than or equal to an epsilon value
		 * @param epsilon the epsilon value (very small)
		 * @return True if the components are <= epsilon. False otherwise.
		 */
		bool IsEpsilon(float epsilon = EPSILON) const;

		/* ********** Static Operations ********** */

		/**
		 * @brief Get a copy of the normalized vector
		 * @param v the vector
		 * @return a copy of the vector normalized
		 */
		static inline Vector<dims> Normalize(const Vector<dims>& v);

		/**
		 * @brief Get a copy of the vector with all of its components being >= 0
		 * @param v the vector
		 * @return the absolute valued vector copy
		 */
		static inline Vector<dims> AbsCopy(const Vector<dims>& v);

		/**
		 * @brief Get the dot product between two vectors
		 * @param lhs The left vector
		 * @param rhs The right vector
		 * @return the dot product
		 */
		static inline float Dot(const Vector<dims>& lhs, const Vector<dims>& rhs);

		/**
		 * @brief Get the cross product between two vectors
		 * @param lhs The left vector
		 * @param rhs The right vector
		 * @return the cross product
		 */
		static inline Vector<dims> Cross(const Vector<dims>& lhs, const Vector<dims>& rhs);

		/**
		 * @brief Get the outgoing, reflected vector
		 * @param in The incident vector
		 * @param normal The normal vector
		 * @return the reflected vector
		 */
		static inline Vector<dims> Reflect(const Vector<dims>& in, const Vector<dims>& normal);

		/**
		 * @brief Get the distance from `from` to `to`
		 * @param from The source vector
		 * @param to The destination vector
		 * @return the distance between the two vectors
		 */
		static inline float Distance(const Vector<dims>& from, const Vector<dims>& to);

		/**
		 * @brief Get the square distance from `from` to `to`
		 * @param from The source vector
		 * @param to The destination vector
		 * @return the square distance between the two vectors
		 */
		static inline float SquareDistance(const Vector<dims>& from, const Vector<dims>& to);

		/**
		 * @brief Get the linearly interpolated vector between the start and end vectors
		 * @param start The start vector
		 * @param end The end vector
		 * @param t The amount to lerp by with range [0, 1]
		 * @return The vector between the start and end (inclusive)
		 */
		static inline Vector<dims> Lerp(const Vector<dims>& start, const Vector<dims>& end, float t);

		/**
		 * @brief Get the angle between the two vector
		 * @param from The source vector
		 * @param to The destination vector
		 * @return the angle between the two vectors
		 */
		static inline Radian Angle(const Vector<dims>& from, const Vector<dims>& to);


		/* ********** Useful Vectors ********** */

		/**
		 * @brief Create the zero vector
		 * @return the Zero vector
		 */
		static Vector<dims> Zero();

		/**
		 * @brief Create the unit vector
		 * @return the Unit vector
		 */
		static Vector<dims> One();

		/**
		 * @brief Create the up vector (+y is up)
		 * @return the Up vector
		 */
		static Vector<dims> Up();

		/**
		 * @brief Create the down vector (-y is down)
		 * @return the Down vector
		 */
		static Vector<dims> Down();

		/**
		 * @brief Create the right vector (+x is right)
		 * @return the Right vector
		 */
		static Vector<dims> Right();

		/**
		 * @brief Create the left vector (-x is left)
		 * @return the Left vector
		 */
		static Vector<dims> Left();

		/**
		 * @brief Create the forward vector (+z is forward)
		 * @return the Forward vector
		 */
		static Vector<dims> Forward();

		/**
		 * @brief Create the back vector (-z is back)
		 * @return the Back vector
		 */
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