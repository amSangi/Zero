#pragma once

#include "Vector3.hpp"

namespace zero {

	class Sphere {
	public:
		Sphere() = default;
		explicit Sphere(float radius);
		explicit Sphere(const Vector3& center);
		Sphere(const Vector3& center, float radius);

		~Sphere() = default;
		Sphere& operator=(const Sphere& other) = default;

		bool operator==(const Sphere& other) const;
		bool operator!=(const Sphere& other) const;

		/* ********** Intersection Tests ********** */
		/**
		 * @brief Check if a box is inside the sphere
		 * @param box The box
		 * @return True if box is inside this. Otherwise false.
		 */
		bool Contains(const Box& box) const;

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


		/**
		 * @brief Check if another sphere intersects this sphere
		 * @param other The other sphere
		 * @return True if the other sphere intersects this. Otherwise false.
		 */
		bool Intersects(const Sphere& other) const;

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

		Vector3 center_;
		float radius_;

	}; // class Sphere

} // namespace zero