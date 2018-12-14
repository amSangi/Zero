#pragma once

#include "ZeroMath.h"

namespace Zero {

	class Vector3 {
	public:
		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3(const Vector3& other) = default;
		explicit Vector3(const float* data) : x(data[0]), y(data[1]), z(data[2]) {}
		~Vector3() = default;

		Vector3& operator=(const Vector3& other) = default;

		bool operator==(const Vector3& o) const                { return x == o.x && y == o.y && z == o.z; }
		bool operator!=(const Vector3& o) const                { return !(operator==(o)); }

		/* ********** Scalar Operations ********** */
		Vector3 operator+(float scalar) const                  { return Vector3(x + scalar, y + scalar, z + scalar); }
		Vector3 operator-(float scalar) const                  { return Vector3(x - scalar, y - scalar, z - scalar); }
		Vector3 operator*(float scalar) const                  { return Vector3(x * scalar, y * scalar, z * scalar); }
		Vector3 operator/(float scalar) const                  { return Vector3(x / scalar, y / scalar, z / scalar); }

		Vector3& operator+=(float scalar)                      { x += scalar; y += scalar; z += scalar; return *this; }
		Vector3& operator-=(float scalar)                      { x -= scalar; y -= scalar; z -= scalar; return *this; }
		Vector3& operator*=(float scalar)                      { x *= scalar; y *= scalar; z *= scalar; return *this; }
		Vector3& operator/=(float scalar)                      { x /= scalar; y /= scalar; z /= scalar; return *this; }

		/* **** Component-Wise Vector Operations **** */
		Vector3 operator+(const Vector3& rhs) const            { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z ); }
		Vector3 operator-(const Vector3& rhs) const            { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z ); }
		Vector3 operator*(const Vector3& rhs) const            { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z ); }
		Vector3 operator/(const Vector3& rhs) const            { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z ); }

		Vector3& operator+=(const Vector3& rhs)                { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
		Vector3& operator-=(const Vector3& rhs)                { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
		Vector3& operator*=(const Vector3& rhs)                { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
		Vector3& operator/=(const Vector3& rhs)                { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

		/* ********** Vector Operations ********** */
		float Magnitude() const                                { return sqrt(SquareMagnitude()); }

		float SquareMagnitude() const                          { return (x * x + y * y + z * z); }

		/* ********** Static Operations ********** */
        static Vector3 Normalize(const Vector3& v)             { return v / v.Magnitude(); }

        static Vector3 Abs(const Vector3& v)                   { return Vector3(abs(v.x), abs(v.y), abs(v.z)); }

        static float Dot(const Vector3& lhs,
						 const Vector3& rhs)                   { return   lhs.x * rhs.x
																		+ lhs.y * rhs.y
																		+ lhs.z * rhs.z; }

		static Vector3 Reflect(const Vector3& in,
							   const Vector3& normal)          { return in - (normal * 2 * Dot(in, normal)); }

		static Vector3 Cross(const Vector3& lhs,
							 const Vector3& rhs)               { return Vector3((lhs.y * rhs.z - lhs.z * rhs.y),
																				(lhs.z * rhs.x - lhs.x * rhs.z),
																				(lhs.x * rhs.y - lhs.y * rhs.x)); }

		static float Distance(const Vector3& from,
		                             const Vector3& to)        { return (from - to).Magnitude(); }

		static float SquareDistance(const Vector3& from,
									const Vector3& to)         { return (from - to).SquareMagnitude(); }

		static Vector3 Lerp(const Vector3& start,
							const Vector3& end,
							float t)                           { return (start * (1.0f - t)) + (end * t);}


		static float Angle(const Vector3& from,
		                   const Vector3& to)                  {
			float dot = Dot(from, to);
			float square_mag_from = from.SquareMagnitude();
			float square_mag_to = to.SquareMagnitude();
			return acos( dot / sqrt(square_mag_from * square_mag_to) );
		}


		/* ********** Common Vectors ********** */
		static Vector3 Up()                                    { return Vector3(0.0f, 1.0f, 0.0f);  }
		static Vector3 Down()                                  { return Vector3(0.0f, -1.0f, 0.0f); }
		static Vector3 Right()                                 { return Vector3(1.0f, 0.0f, 0.0f);  }
		static Vector3 Left()                                  { return Vector3(-1.0f, 0.0f, 0.0f); }
		static Vector3 Front()                                 { return Vector3(0.0f, 0.0f, 1.0f);  }
		static Vector3 Back()                                  { return Vector3(0.0f, 0.0f, -1.0f); }
		static Vector3 One()                                   { return Vector3(1.0f, 1.0f, 1.0f);  }

		float x;
		float y;
		float z;

	}; // class Vector3


	// Scalar Vector
	inline Vector3 operator+(float scalar, const Vector3& v)   { return v + scalar;  }
	inline Vector3 operator-(float scalar, const Vector3& v)   { return v - scalar;  }
	inline Vector3 operator*(float scalar, const Vector3& v)   { return v * scalar;  }
	inline Vector3 operator/(float scalar, const Vector3& v)   { return v / scalar;  }

} // namespace Zero