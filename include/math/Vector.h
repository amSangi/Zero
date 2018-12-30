#pragma once

#include "Angle.h"
#include "ZeroMath.h"

namespace Zero {


	template<int dims>
	class VectorBase {
	public:
		VectorBase() = default;
		explicit VectorBase(float value) {
			for (int i = 0; i < dims; ++i) {
				data[i] = value;
			}
		}
		VectorBase(float x1, float y1) {
			static_assert(dims > 1, "Require at least 2 dimensions");
			data[0] = x1;
			data[1] = y1;
		}
		VectorBase(float x1, float y1, float z1) {
			static_assert(dims > 2, "Require at least 3 dimensions");
			data[0] = x1;
			data[1] = y1;
			data[2] = z1;
		}
		VectorBase(float x1, float y1, float z1, float w1) {
			static_assert(dims > 3, "Require at least 4 dimensions");
			data[0] = x1;
			data[1] = y1;
			data[2] = z1;
			data[3] = w1;
		}

		inline float* Data()                  { return data; }
		inline const float* Data() const      { return data; }

	public:
		float data[dims];

	}; // template class VectorBase

	template<>
	class VectorBase<2> {
	public:
		VectorBase() = default;
		VectorBase(float x1, float y1)   : x(x1), y(y1) {}
		explicit VectorBase(float value) : x(value), y(value) {}


		inline float* Data()                  { return &x; }
		inline const float* Data() const      { return &x; }

	public:
		float x, y;

	}; // template specialization class VectorBase<2>


	template<>
	class VectorBase<3> {
	public:
		VectorBase() = default;
		VectorBase(float x1, float y1)            : x(x1), y(y1), z(0.0f) {}
		VectorBase(float x1, float y1, float z1)  : x(x1), y(y1), z(z1) {}
		explicit VectorBase(float value)          : x(value), y(value), z(value) {}


		inline float* Data()                  { return &x; }
		inline const float* Data() const      { return &x; }

	public:
		float x, y, z;

	}; // template specialization class VectorBase<3>


	template<>
	class VectorBase<4> {
	public:
		VectorBase() = default;
		VectorBase(float x1, float y1)                     : x(x1), y(y1), z(0.0f), w(0.0f) {}
		VectorBase(float x1, float y1, float z1)           : x(x1), y(y1), z(z1), w(0.0f) {}
		VectorBase(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1) {}
		explicit VectorBase(float value)                   : x(value), y(value), z(value), w(value) {}

		inline float* Data()                  { return &x; }
		inline const float* Data() const      { return &x; }

	public:
		float x, y, z, w;

	}; // template specialization class VectorBase<4>



	template<int dims>
	class Vector  : public VectorBase<dims> {
		using VectorBase<dims>::Data;
	public:
		Vector<dims>() = default;
		Vector<dims>(const Vector<dims>& other) = default;
		explicit Vector<dims>(float value) : VectorBase<dims>(value) {}
		Vector<dims>(float x1, float y1) : VectorBase<dims>(x1, y1) {};
		Vector<dims>(float x1, float y1, float z1) : VectorBase<dims>(x1, y1, z1) {}
		Vector<dims>(float x1, float y1, float z1, float w1) : VectorBase<dims>(x1, y1, z1, w1) {}

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

	public:

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
		return Zero::sqrt(SquareMagnitude());
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
			data[i] = Zero::abs(data[i]);
		}
		return *this;
	}

	template<int dims>
	bool Vector<dims>::Normalize() {
		float magnitude = Magnitude();

		float inv_magnitude = 1.0f / magnitude;
		if (magnitude > 0.0f) {
			operator*=(inv_magnitude);
			return true;
		}

		return false;
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
		return Vector<dims>(lhs.y * rhs.z - lhs.z * rhs.y,
                            lhs.z * rhs.x - lhs.x * rhs.z,
                            lhs.x * rhs.y - lhs.y * rhs.x);
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
		float inv_sqrt = 1.0f / sqrt(square_mag_from * square_mag_to);
		float angle = Zero::acos( dot * inv_sqrt );
		return Radian(angle);
	}


	/* ********** Operator Overload Implementation ********** */

	template<int dims>
	bool Vector<dims>::operator==(const Vector<dims>& o) const {
		const float* data = Data();
		const float* rhs_data = o.Data();
		for (int i = 0; i < dims; ++i) {
			if (!equal(data[i], rhs_data[i])) return false;
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
		Vector<dims> v = *this;
		v += scalar;
		return v;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator-(float scalar) const {
		Vector<dims> v = *this;
		v -= scalar;
		return v;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator*(float scalar) const {
		Vector<dims> v = *this;
		v *= scalar;
		return v;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator/(float scalar) const {
		Vector<dims> v = *this;
		v /= scalar;
		return v;
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
		Vector<dims> v = *this;
		float* data = v.Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			data[i] += rhs_data[i];
		}
		return v;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator-(const Vector<dims>& rhs) const {
		Vector<dims> v = *this;
		float* data = v.Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			data[i] -= rhs_data[i];
		}
		return v;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator*(const Vector<dims>& rhs) const {
		Vector<dims> v = *this;
		float* data = v.Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			data[i] *= rhs_data[i];
		}
		return v;
	}

	template<int dims>
	Vector<dims> Vector<dims>::operator/(const Vector<dims>& rhs) const {
		Vector<dims> v = *this;
		float* data = v.Data();
		const float* rhs_data = rhs.Data();
		for (int i = 0; i < dims; ++i) {
			data[i] /= rhs_data[i];
		}
		return v;
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

} // namespace Zero