#include "Sphere.hpp"
#include "Box.hpp"

using namespace zero::math;

Sphere::Sphere(float r)
   : center_(Vector3::Zero()), radius_(r) {}

Sphere::Sphere(const Vector3& c)
   : center_(c), radius_(0.0f) {}

Sphere::Sphere(const Vector3& c, float r)
   : center_(c), radius_(r) {}

bool Sphere::operator==(const Sphere& other) const {
	return (center_ == other.center_) && Equal(radius_, other.radius_);
}

bool Sphere::operator!=(const Sphere& other) const {
	return !operator==(other);
}


/* ********** Intersection Tests ********** */
bool Sphere::Contains(const Box& box) const {
	return Contains(box.min) && Contains(box.max);
}

bool Sphere::Contains(const Sphere& other) const {
	if (other.radius_ > radius_) {
		return false;
	}

	float radius_difference = radius_ - other.radius_;
	return Vector3::SquareDistance(center_, other.center_) <= (radius_difference * radius_difference);
}

bool Sphere::Contains(const Vector3& point) const {
	return Vector3::SquareDistance(center_, point) <= (radius_ * radius_);
}

bool Sphere::Intersects(const Sphere& other) const {
	float max = Max(0.0f, radius_ + other.radius_);
	return Vector3::SquareDistance(center_, other.center_) <= (max * max);
}


/* ********** Merge ********** */
void Sphere::Merge(const Sphere& other) {
	if (radius_ <= EPSILON || other.Contains(*this)) {
		*this = other;
	}
	else if (Contains(other)) {
		return;
	}
	else {
		Vector3 direction = center_ - other.center_;
		float distance = direction.Magnitude();
		// Normalize direction
		if (distance > SMALL_EPSILON) direction /= distance;

		// Half of diameter of enclosing circle
		float merged_radius = (distance + radius_ + other.radius_) * 0.5f;

		// Compute sphere end points
		Vector3 sphere_end_point = center_ - (radius_ * direction);
		Vector3 other_end_point = other.center_ + (other.radius_ * direction);

		// Linearly interpolate half-way between end points
		Vector3 merged_center = (sphere_end_point + other_end_point) * 0.5f;

		radius_ = merged_radius;
		center_ = merged_center;
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
