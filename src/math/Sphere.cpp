#include "Sphere.hpp"
#include "Box.hpp"

using namespace zero::math;

Sphere::Sphere(float r)
   : center_(Vec3f::Zero()), radius_(r) {}

Sphere::Sphere(const Vec3f& c)
   : center_(c), radius_(0.0f) {}

Sphere::Sphere(const Vec3f& c, float r)
   : center_(c), radius_(r) {}

bool Sphere::operator==(const Sphere& other) const {
	return (center_ == other.center_) && Equal(radius_, other.radius_);
}

bool Sphere::operator!=(const Sphere& other) const {
	return !operator==(other);
}


/* ********** Intersection Tests ********** */
bool Sphere::Contains(const Box& box) const {
	return Contains(box.min_) && Contains(box.max_);
}

bool Sphere::Contains(const Sphere& other) const {
	if (other.radius_ > radius_) {
		return false;
	}

	float radius_difference = radius_ - other.radius_;
	return Vec3f::SquareDistance(center_, other.center_) <= (radius_difference * radius_difference);
}

bool Sphere::Contains(const Vec3f& point) const {
	return Vec3f::SquareDistance(center_, point) <= (radius_ * radius_);
}

bool Sphere::Intersects(const Sphere& other) const {
	float max = Max(0.0f, radius_ + other.radius_);
	return Vec3f::SquareDistance(center_, other.center_) <= (max * max);
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
		Vec3f direction = center_ - other.center_;
		float distance = direction.Magnitude();
		// Normalize direction
		if (distance > SMALL_EPSILON) direction /= distance;

		// Half of diameter of enclosing circle
		float merged_radius = (distance + radius_ + other.radius_) * 0.5f;

		// Compute sphere end points
		Vec3f sphere_end_point = center_ - (radius_ * direction);
		Vec3f other_end_point = other.center_ + (other.radius_ * direction);

		// Linearly interpolate half-way between end points
		Vec3f merged_center = (sphere_end_point + other_end_point) * 0.5f;

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
	return Sphere(Vec3f::Zero(), 0.0f);
}
