#pragma once

#include "Vector3.h"
#include "ZeroMath.h"

namespace Zero {

	class Vector4 {
	public:
		Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		explicit Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}
		explicit Vector4(const float* data) : x(data[0]), y(data[1]), z(data[2]), w(data[3]) {}
		Vector4(const Vector4& other) = default;
		~Vector4() = default;

		Vector4& operator=(const Vector4& other) = default;

		bool operator==(const Vector4& o) const                      { return x == o.x && y == o.y
		                                                                        && z == o.z && w == o.w; }

	 	bool operator!=(const Vector4& o) const                      { return !(operator==(o)); }

		/* ********** Scalar Operations ********** */
		Vector4 operator+(float scalar) const                        { return Vector4(x + scalar, y + scalar,
																					  z + scalar, w + scalar); }

	 	Vector4 operator-(float scalar) const                        { return Vector4(x - scalar, y - scalar,
																					  z - scalar, w - scalar); }

	 	Vector4 operator*(float scalar) const                        { return Vector4(x * scalar, y * scalar,
																					  z * scalar, w * scalar); }

	 	Vector4 operator/(float scalar) const                        { return Vector4(x / scalar, y / scalar,
																					  z / scalar, w / scalar); }

		Vector4& operator+=(float scalar)                            { x += scalar; y += scalar;
		                                                               z += scalar; w += scalar;
		                                                               return *this; }

		Vector4& operator-=(float scalar)                            { x -= scalar; y -= scalar;
		                                                               z -= scalar; w -= scalar;
		                                                               return *this; }

		Vector4& operator*=(float scalar)                            { x *= scalar; y *= scalar;
		                                                               z *= scalar; w *= scalar;
		                                                               return *this; }

		Vector4& operator/=(float scalar)                            { x /= scalar; y /= scalar;
		                                                               z /= scalar; w /= scalar;
		                                                               return *this; }

		/* **** Component-Wise Vector Operations **** */
		Vector4 operator+(const Vector4& rhs) const                  { return Vector4(x + rhs.x, y + rhs.y,
																					  z + rhs.z, w + rhs.z); }

	 	Vector4 operator-(const Vector4& rhs) const                  { return Vector4(x - rhs.x, y - rhs.y,
																					  z - rhs.z, w - rhs.z); }

	 	Vector4 operator*(const Vector4& rhs) const                  { return Vector4(x * rhs.x, y * rhs.y,
																					  z * rhs.z, w * rhs.z); }

	 	Vector4 operator/(const Vector4& rhs) const                  { return Vector4(x / rhs.x, y / rhs.y,
																					  z / rhs.z, w / rhs.z); }

		Vector4& operator+=(const Vector4& rhs)                      { x += rhs.x; y += rhs.y;
		                                                               z += rhs.z; w += rhs.w;
		                                                               return *this; }

		Vector4& operator-=(const Vector4& rhs)                      { x -= rhs.x; y -= rhs.y;
		                                                               z -= rhs.z; w -= rhs.w;
		                                                               return *this; }

		Vector4& operator*=(const Vector4& rhs)                      { x *= rhs.x; y *= rhs.y;
		                                                               z *= rhs.z; w *= rhs.w;
		                                                               return *this; }

		Vector4& operator/=(const Vector4& rhs)                      { x /= rhs.x; y /= rhs.y;
		                                                               z /= rhs.z; w /= rhs.w;
		                                                               return *this; }

		/* ********** Vector Operations ********** */
	 	float Magnitude() const                                      { return sqrt(SquareMagnitude()); }

	 	float SquareMagnitude() const                                { return (x * x + y * y + z * z + w * w); }

		/* ********** Static Operations ********** */
		static Vector4 Normalize(const Vector4& v)                   { return v / v.Magnitude(); }


		static Vector4 Abs(const Vector4& v)                         { return Vector4(abs(v.x), abs(v.y),
																					  abs(v.z), abs(v.w)); }

		static float Dot(const Vector4& lhs,
						 const Vector4& rhs)                         { return lhs.x * rhs.x + lhs.y * rhs.y
																			+ lhs.z * rhs.z  + lhs.w * rhs.w; }

	 	static Vector4 Cross(const Vector4& lhs,
							 const Vector4& rhs)                     { return Vector4(); }

		static float Distance(const Vector4& from,
							  const Vector4& to)                     { return (from - to).Magnitude(); }

		static float SquareDistance(const Vector4& from,
									const Vector4& to)               { return (from - to).SquareMagnitude(); }

		static Vector4 Lerp(const Vector4& start,
							const Vector4& end,
							float t)                                 { return (start * (1.0f - t) + (end * t)); }


		/* ********** Common Vectors ********** */
		static Vector4 One()                                         { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }


		float x;
		float y;
		float z;
		float w;

	}; // class Vector4


	// Scalar Vector
	inline Vector4 operator+(float scalar, const Vector4& v)         { return v + scalar;  }
	inline Vector4 operator-(float scalar, const Vector4& v)         { return v - scalar;  }
	inline Vector4 operator*(float scalar, const Vector4& v)         { return v * scalar;  }
	inline Vector4 operator/(float scalar, const Vector4& v)         { return v / scalar;  }

} // namespace Zero