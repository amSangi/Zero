#include "Intersection.hpp"
#include "Box.hpp"
#include "Sphere.hpp"


using namespace zero;

bool Intersection::BoxSphereIntersect(const Box& box, const Sphere& sphere) {
	// Arvo's Box/Sphere Intersection Test

	float square_distance = 0.0f;
	float difference;

	// Get square x distance
	if (sphere.center_.x_ < box.min.x_) {
		difference = sphere.center_.x_ - box.min.x_;
		square_distance += (difference * difference);
	}
	else if (sphere.center_.x_ > box.max.x_) {
		difference = sphere.center_.x_ - box.max.x_;
		square_distance += (difference * difference);
	}

	// Get square y distance
	if (sphere.center_.y_ < box.min.y_) {
		difference = sphere.center_.y_ - box.min.y_;
		square_distance += (difference * difference);
	}
	else if (sphere.center_.y_ > box.max.y_) {
		difference = sphere.center_.y_ - box.max.y_;
		square_distance += (difference * difference);
	}

	// Get square z distance
	if (sphere.center_.z_ < box.min.z_) {
		difference = sphere.center_.z_ - box.min.z_;
		square_distance += (difference * difference);
	}
	else if (sphere.center_.z_ > box.max.z_) {
		difference = sphere.center_.z_ - box.min.z_;
		square_distance += (difference * difference);
	}

	return square_distance <= (sphere.radius_ * sphere.radius_);
}
