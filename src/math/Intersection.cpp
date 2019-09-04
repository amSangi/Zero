#include "math/Intersection.hpp"
#include "math/Box.hpp"
#include "math/Sphere.hpp"


using namespace zero::math;

bool Intersection::BoxSphereIntersect(const Box& box, const Sphere& sphere) {
	// Arvo's Box/Sphere Intersection Test

	float square_distance = 0.0F;
	float difference;

	// Get square x distance
	if (sphere.center_.x_ < box.min_.x_) {
		difference = sphere.center_.x_ - box.min_.x_;
		square_distance += (difference * difference);
	}
	else if (sphere.center_.x_ > box.max_.x_) {
		difference = sphere.center_.x_ - box.max_.x_;
		square_distance += (difference * difference);
	}

	// Get square y distance
	if (sphere.center_.y_ < box.min_.y_) {
		difference = sphere.center_.y_ - box.min_.y_;
		square_distance += (difference * difference);
	}
	else if (sphere.center_.y_ > box.max_.y_) {
		difference = sphere.center_.y_ - box.max_.y_;
		square_distance += (difference * difference);
	}

	// Get square z distance
	if (sphere.center_.z_ < box.min_.z_) {
		difference = sphere.center_.z_ - box.min_.z_;
		square_distance += (difference * difference);
	}
	else if (sphere.center_.z_ > box.max_.z_) {
		difference = sphere.center_.z_ - box.min_.z_;
		square_distance += (difference * difference);
	}

	return square_distance <= (sphere.radius_ * sphere.radius_);
}
