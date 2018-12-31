#include "Sphere.h"

using namespace Zero;

Sphere::Sphere()
   : radius_(0.0f), center_(0.0f) {}

Sphere::Sphere(float radius)
   : radius_(radius), center_(0.0f) {}

Sphere::Sphere(const Vector3& center)
   : radius_(0.0f), center_(center) {}

Sphere::Sphere(const Vector3& center, float radius)
   : center_(center), radius_(radius) {}

bool Sphere::operator==(const Sphere& o) {
	return center_ == o.center_ && Zero::Equal(radius_, o.radius_);
}

bool Sphere::operator!=(const Sphere& o) {
	return !operator==(o);
}

void Sphere::SetCenter(Vector3 center) {
	center_ = center;
}

void Sphere::SetRadius(float radius) {
	radius_ = radius;
}

Vector3 Sphere::GetCenter() const {
	return center_;
}

float Sphere::GetRadius() const {
	return radius_;
}

bool Sphere::Intersects(const Vector3& point) const {
	// TODO: Implement
	return false;
}

bool Sphere::Intersects(const Ray& ray) const {
	// TODO: Implement
	return false;
}

bool Sphere::Intersects(const Box& box) const {
	// TODO: Implement
	return false;
}

bool Sphere::Intersects(const Sphere& sphere) const {
	// TODO: Implement
	return false;
}

bool Sphere::Intersects(const Cone& cone) const {
	// TODO: Implement
	return false;
}

bool Sphere::Intersects(const Frustrum& frustrum) const {
	// TODO: Implement
	return false;
}

void Sphere::Merge(const Sphere& other) {
	// TODO: Implement
}

Sphere Sphere::Merge(const Sphere& lhs, const Sphere& rhs) {
	// TODO: Implement
	return Sphere();
}
