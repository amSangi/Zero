#pragma once

#include "Vector3.h"

namespace Zero {

	class Sphere {
	public:
		Sphere() = default;
		explicit Sphere(float radius);
		explicit Sphere(const Vector3& center);
		Sphere(const Vector3& center, float radius);

		~Sphere() = default;
		Sphere& operator=(const Sphere& o) = default;

		bool operator==(const Sphere& o) const;
		bool operator!=(const Sphere& o) const;

		/* ********** Intersection Tests ********** */
		/**
		 * @brief Check if a box is inside the sphere
		 * @param box The box
		 * @return True if box is inside this. Otherwise false.
		 */
		bool Contains(const Box& box) const;

		/**
		 * @brief Check if a Frustrum is inside the sphere
		 * @param frustrum The frustrum
		 * @return True if frustrum is inside this. Otherwise false.
		 */
		bool Contains(const Frustrum& frustrum) const;

		/**
		 * @brief Check if a ray is inside the sphere
		 * @param ray The ray
		 * @return True if ray is inside this. Otherwise false.
		 */
		bool Contains(const Ray& ray) const;

		/**
		 * @brief Check if another sphere is inside the sphere
		 * @param other The other sphere
		 * @return True if the other sphere is inside this. Otherwise false.
		 */
		bool Contains(const Sphere& other) const;

		/**
		 * @brief Check if a point is inside the sphere
		 * @param point The point
		 * @return True if the point is inside this. Otherwise false.
		 */
		bool Contains(const Vector3& point) const;

		/* ********** Merge ********** */
		/**
		 * @brief Merge this sphere with another sphere
		 * @param other The other sphere
		 */
		void Merge(const Sphere& other);

		/* ********** Static Operations ********** */

		/**
		 * @brief Create a new sphere that contains the two given spheres
		 * @param lhs The first sphere
		 * @param rhs The second sphere
		 * @return A new sphere that contains both lhs and rhs
		 */
		static Sphere Merge(const Sphere& lhs, const Sphere& rhs);

		static Sphere Zero();

	public:
		Vector3 center;
		float radius;

	}; // class Sphere

} // namespace Zero