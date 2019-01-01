#include "Intersection.h"
#include "Box.h"
#include "Sphere.h"


using namespace Zero;

bool Intersection::BoxSphereIntersect(const Box& box, const Sphere& sphere) {
	// Arvo's Box/Sphere Intersection Test

	float square_distance = 0.0f;
	float difference;

	// Get square x distance
	if (sphere.center.x < box.min.x) {
		difference = sphere.center.x - box.min.x;
		square_distance += (difference * difference);
	}
	else if (sphere.center.x > box.max.x) {
		difference = sphere.center.x - box.max.x;
		square_distance += (difference * difference);
	}

	// Get square y distance
	if (sphere.center.y < box.min.y) {
		difference = sphere.center.y - box.min.y;
		square_distance += (difference * difference);
	}
	else if (sphere.center.y > box.max.y) {
		difference = sphere.center.y - box.max.y;
		square_distance += (difference * difference);
	}

	// Get square z distance
	if (sphere.center.z < box.min.z) {
		difference = sphere.center.z - box.min.z;
		square_distance += (difference * difference);
	}
	else if (sphere.center.z > box.max.z) {
		difference = sphere.center.z - box.min.z;
		square_distance += (difference * difference);
	}

	return square_distance <= (sphere.radius * sphere.radius);
}
