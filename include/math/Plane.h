#pragma once

#include "Vector3.h"
#include "Vector4.h"

namespace Zero {

	class Plane {
	public:
		Plane() = default;
		Plane(const Vector4& plane);
		Plane(const Vector3& normal, float d);
		explicit Plane(const Vector3& normal);
		Plane(const Plane& other) = default;

		~Plane() = default;
		Plane& operator=(const Plane& other) = default;

		bool operator==(const Plane& other) const;
		bool operator!=(const Plane& other) const;

		Vector4 ToVector4() const;

		/* ********** Intersection Tests ********** */


		/* ********** Transform Operations ********** */


		/* ********** Plane Operations ********** */

		

	public:
		Vector3 normal;
		float d;

	}; // class Plane

} // namespace Zero