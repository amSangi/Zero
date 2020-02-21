#pragma once

#include "Vector3.hpp"

namespace zero::math {

	/**
	 * @brief A plane represented as Ax + By + Cz + D = 0
	 */
	class Plane {
	public:
		Plane() = default;
		Plane(const Vec3f& normal, float d);
		Plane(const Vec3f& normal, const Vec3f& point);
		Plane(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3);
		explicit Plane(const Vec4f& plane);
		explicit Plane(const Vec3f& normal);
		Plane(const Plane& other) = default;

		~Plane() = default;
		Plane& operator=(const Plane& other) = default;

		/**
		 * @brief Check if the plane is equal to another plane
		 * @param other The other plane
		 * @return True if the normal and D are equal. False otherwise.
		 */
		bool operator==(const Plane& other) const;

		/**
		 * @brief Check if the plane is not equal to another plane
		 * @param other The other plane
		 * @return True if the normal or D are not equal. False otherwise.
		 */
		bool operator!=(const Plane& other) const;

		/**
		 * @return a vec4f representing this plane
		 */
		Vec4f ToVector4() const;

		/* ********** Intersection Tests ********** */

		/**
		 * @brief Check if another plane and this plane intersect
		 * @param other The other plane
		 * @return True if the other plane intersects with this. Otherwise false.
		 */
		bool Intersects(const Plane& other, float epsilon = kEpsilon) const;

		/* ********** Transform Operations ********** */

		/**
		 * @brief apply a matrix3 transformation to this plane
		 * @param matrix the matrix3 transformation
		 */
		void Transform(const Matrix3x3& matrix);

		/**
		 * @brief apply a matrix4 transformation to this plane
		 * @param matrix the matrix4 transformation
		 */
		void Transform(const Matrix4x4& matrix);

		/* ********** Plane Operations ********** */
		/**
		 * @brief Return the projected point on this plane of a given point
		 * @param point the given point
		 * @return the projected point
		 */
        [[nodiscard]] Vec3f Project(const Vec3f& point) const;

		/**
		 * @brief Compute the reflection ray of a given incident ray
		 * @param incident the incident ray
		 * @return the reflection ray
		 */
        [[nodiscard]] Vec3f Reflect(const Vec3f& incident) const;

		/**
		 * @brief Compute the distance from the plant to a point
		 * @param point the point
		 * @return the distance from this to the given point
		 */
        [[nodiscard]] float Distance(const Vec3f& point) const;

		/**
		 * @brief Normalize the plane
		 * @return True if the magnitude is not zero. Otherwise False.
		 */
		bool Normalize();

		/**
		 * @brief Flip the plane to face the opposite direction
		 * @return a new Plane that has been flipped
		 */
        [[nodiscard]] Plane Flip() const;

		/* ********** Static Operations ********** */

		/**
		 * @brief Apply a matrix3 transformation to a given plane
		 * @param plane The plane
		 * @param matrix The transformation
		 * @return A transformed plane
		 */
		static Plane Transform(const Plane& plane, const Matrix3x3& matrix);

		/**
		 * @brief Apply a matrix4 transformation to a given plane
		 * @param plane The plane
		 * @param matrix The transformation
		 * @return A transformed plane
		 */
		static Plane Transform(const Plane& plane, const Matrix4x4& matrix);

		/**
		 * @return a plane facing the Vec3f up direction
		 */
		static Plane Up();

		/**
		 * @return a plane facing the Vec3f right direction
		 */
		static Plane Right();

		/**
		 * @return a plane facing the Vec3f forward direction
		 */
		static Plane Forward();

		/**
		 * @brief The normal of the plane <A, B, C>: Ax + By + Cz + D = 0
		 */
		Vec3f normal_;

		/**
		 * @brief The D component of the plane equation: Ax + By + Cz + D = 0
		 */
		float d_;

	}; // class Plane

} // namespace zero::math