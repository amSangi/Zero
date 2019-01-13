#pragma once

#include "Vector3.hpp"

namespace Zero {

	/**
	 * @brief A plane represented as Ax + By + Cz + D = 0
	 */
	class Plane {
	public:
		Plane() = default;
		Plane(const Vector3& normal, float d);
		Plane(const Vector3& normal, const Vector3& point);
		Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);
		explicit Plane(const Vector4& plane);
		explicit Plane(const Vector3& normal);
		Plane(const Plane& other) = default;

		~Plane() = default;
		Plane& operator=(const Plane& other) = default;

		bool operator==(const Plane& other) const;
		bool operator!=(const Plane& other) const;

		/**
		 * @return a vector4 representing this plane
		 */
		Vector4 ToVector4() const;

		/* ********** Intersection Tests ********** */

		/**
		 * @brief Check if another plane and this plane intersect
		 * @param other The other plane
		 * @return True if the other plane intersects with this. Otherwise false.
		 */
		bool Intersects(const Plane& other, float epsilon = EPSILON) const;

		/* ********** Transform Operations ********** */

		/**
		 * @brief apply a matrix3 transformation to this plane
		 * @param matrix the matrix3 transformation
		 */
		void Transform(const Matrix3& matrix);

		/**
		 * @brief apply a matrix4 transformation to this plane
		 * @param matrix the matrix4 transformation
		 */
		void Transform(const Matrix4& matrix);

		/* ********** Plane Operations ********** */
		/**
		 * @brief Return the projected point on this plane of a given point
		 * @param point the given point
		 * @return the projected point
		 */
		Vector3 Project(const Vector3& point) const;

		/**
		 * @brief Compute the reflection ray of a given incident ray
		 * @param incident the incident ray
		 * @return the reflection ray
		 */
		Vector3 Reflect(const Vector3& incident) const;

		/**
		 * @brief Compute the distance from the plant to a point
		 * @param point the point
		 * @return the distance from this to the given point
		 */
		float Distance(const Vector3& point) const;

		/**
		 * @brief Normalize the plane
		 * @return True if the magnitude is not zero. Otherwise False.
		 */
		bool Normalize();

		/**
		 * @brief Flip the plane to face the opposite direction
		 * @return a new Plane that has been flipped
		 */
		Plane Flip() const;

		/* ********** Static Operations ********** */
		static Plane Transform(const Plane& plane, const Matrix3& matrix);

		static Plane Transform(const Plane& plane, const Matrix4& matrix);

		/**
		 * @return a plane facing the Vector3 up direction
		 */
		static Plane Up();

		/**
		 * @return a plane facing the Vector3 right direction
		 */
		static Plane Right();

		/**
		 * @return a plane facing the Vector3 forward direction
		 */
		static Plane Forward();

		Vector3 normal;
		float d;

	}; // class Plane

} // namespace Zero