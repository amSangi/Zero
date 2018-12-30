#pragma once

#include "Vector3.h"
#include "ZeroMath.h"

namespace Zero {

	class Sphere {
	public:
		Sphere();
		explicit Sphere(float radius);
		explicit Sphere(const Vector3& center);
		Sphere(const Vector3& center, float radius);

		~Sphere() = default;
		Sphere& operator=(const Sphere& o) = default;

		bool operator==(const Sphere& o);
		bool operator!=(const Sphere& o);

		/* ********** Getter/Setters ********** */
		void SetCenter(Vector3 center);
		void SetRadius(float radius);
		Vector3 GetCenter() const;
		float GetRadius() const;


		/* ********** Intersection Tests ********** */
		bool Intersects(const Vector3& point) const;
		bool Intersects(const Ray& ray) const;
		bool Intersects(const Box& box) const;
		bool Intersects(const Sphere& sphere) const;
		bool Intersects(const Cone& cone) const;
		bool Intersects(const Frustrum& frustrum) const;

		/* ********** Merge ********** */
		void Merge(const Sphere& other);

		static Sphere Merge(const Sphere& lhs, const Sphere& rhs);

	private:
		Vector3 center_;
		float radius_;

	}; // class Sphere

} // namespace Zero