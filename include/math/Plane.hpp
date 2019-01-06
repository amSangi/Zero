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

		Vector4 ToVector4() const;

		/* ********** Intersection Tests ********** */
		/**
		 * @brief Check if another plane and this plane intersect
		 * @param other The other plane
		 * @return True if the other plane intersects with this. Otherwise false.
		 */
		bool Intersects(const Plane& other, float epsilon = EPSILON) const;

		/* ********** Transform Operations ********** */
		void Transform(const Matrix3& matrix);

		void Transform(const Matrix4& matrix);

		/* ********** Plane Operations ********** */
		Vector3 Project(const Vector3& point) const;

		Vector3 Reflect(const Vector3& incident) const;

		float Distance(const Vector3& point) const;

		bool Normalize();

		Plane Flip() const;

		/* ********** Static Operations ********** */
		static Plane Transform(const Plane& plane, const Matrix3& matrix);

		static Plane Transform(const Plane& plane, const Matrix4& matrix);

		static Plane Up();

		static Plane Right();

		static Plane Forward();

	public:
		Vector3 normal;
		float d;

	}; // class Plane

} // namespace Zero