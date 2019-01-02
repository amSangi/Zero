#include "Plane.h"
#include "Vector4.h"

using namespace Zero;

Plane::Plane(const Vector4& plane)
   : normal(plane.x, plane.y, plane.z), d(plane.w) {}

Plane::Plane(const Vector3& normal, float d)
   : normal(normal), d(d) {}

Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
	// TODO: Finish Implementation
}

Plane::Plane(const Vector3& normal)
   : normal(normal), d(0.0f) {}

bool Plane::operator==(const Plane& other) const {
	return (normal == other.normal) && Zero::Equal(d, other.d);
}

bool Plane::operator!=(const Plane& other) const {
	return !operator==(other);
}

Vector4 Plane::ToVector4() const {
	return Vector4(normal.x, normal.y, normal.z, d);
}


/* ********** Intersection Tests ********** */
bool Plane::Contains(const Vector3& point) const {
	// TODO: Finish Implementation
	return false;
}

bool Plane::Intersects(const Plane& other) const {
	// TODO: Finish Implementation
	return false;
}


/* ********** Transform Operations ********** */
void Plane::Transform(const Matrix3& matrix) {
	// TODO: Finish Implementation
}

void Plane::Transform(const Matrix4& matrix) {
	// TODO: Finish Implementation
}


/* ********** Plane Operations ********** */
Vector3 Plane::Project(const Vector3& point) const {
	// TODO: Finish Implementation
	return Zero::Vector3();
}

Vector3 Plane::Reflect(const Vector3& point) const {
	// TODO: Finish Implementation
	return Zero::Vector3();
}

float Plane::Distance(const Vector3& point) const {
	// TODO: Finish Implementation
	return 0;
}


/* ********** Static Operations ********** */
Plane Plane::Transform(const Plane& plane, const Matrix3& matrix) {
	Plane plane_copy(plane);
	plane_copy.Transform(matrix);
	return plane_copy;
}

Plane Plane::Transform(const Plane& plane, const Matrix4& matrix) {
	Plane plane_copy(plane);
	plane_copy.Transform(matrix);
	return plane_copy;
}
