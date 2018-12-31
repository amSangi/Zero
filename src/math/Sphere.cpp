
#include <math/Sphere.h>

#include "Sphere.h"

using namespace Zero;

Sphere::Sphere(float radius)
   : radius(radius), center(0.0f) {}

Sphere::Sphere(const Vector3& center)
   : radius(0.0f), center(center) {}

Sphere::Sphere(const Vector3& center, float radius)
   : center(center), radius(radius) {}

bool Sphere::operator==(const Sphere& o) const {
	return center == o.center && Zero::Equal(radius, o.radius);
}

bool Sphere::operator!=(const Sphere& o) const {
	return !operator==(o);
}


/* ********** Intersection Tests ********** */
bool Sphere::Contains(const Box& box) const {
	return false; // stub
}

bool Sphere::Contains(const Cone& cone) const {
	return false; // stub
}

bool Sphere::Contains(const Frustrum& frustrum) const {
	return false; // stub
}

bool Sphere::Contains(const Ray& ray) const {
	return false; // stub
}

bool Sphere::Contains(const Sphere& other) const {
	if (other.radius > radius) {
		return false;
	}

	float radius_difference = radius - other.radius;
	return (other.center - center).SquareMagnitude() <= (radius_difference * radius_difference);
}

bool Sphere::Contains(const Vector3& point) const {
	return (point - center).SquareMagnitude() <= (radius * radius);
}


/* ********** Merge ********** */
void Sphere::Merge(const Sphere& other) {
	if (radius <= EPSILON || other.Contains(*this)) {
		*this = other;
	}
	else if (Contains(other)) {
		return;
	}
	else {
		Vector3 direction = center - other.center;
		float distance = direction.Magnitude();
		// Normalize direction
		direction /= distance;

		// Half of diameter of enclosing circle
		float merged_radius = (distance + radius + other.radius) * 0.5f;

		// Compute sphere end points
		Vector3 sphere_end_point = center - (radius * direction);
		Vector3 other_end_point = other.center + (other.radius * direction);

		// Linearly interpolate half-way between end points
		Vector3 merged_center = (sphere_end_point + other_end_point) * 0.5f;

		radius = merged_radius;
		center = merged_center;
	}
}


/* ********** Static Operations ********** */
Sphere Sphere::Merge(const Sphere& lhs, const Sphere& rhs) {
	Sphere lhs_copy = lhs;
	lhs_copy.Merge(rhs);
	return lhs_copy;
}

Sphere Sphere::Zero() {
	return Sphere(Vector3::Zero(), 0.0f);
}
