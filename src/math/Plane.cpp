#include "Plane.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector4.h"

using namespace Zero;

Plane::Plane(const Vector4& plane)
   : normal(Vector3::Normalize(Vector3(plane.x, plane.y, plane.z))), d(plane.w) {}

Plane::Plane(const Vector3& normal, float d)
   : normal(normal), d(d) {}

Plane::Plane(const Vector3& normal, const Vector3& point)
   : normal(normal), d(Vector3::Dot(normal, point)) {}

Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
   : normal(Vector3::Normalize(Vector3::Cross(p2 - p1, p3 - p1))), d(-1.0f * Vector3::Dot(normal, p1)) {}

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
bool Plane::Intersects(const Plane& other, float epsilon) const {
	return !( Vector3::Cross(normal, other.normal).IsEpsilon(epsilon) );
}


/* ********** Transform Operations ********** */
void Plane::Transform(const Matrix3& matrix) {
	Transform(Matrix4(matrix));
}

void Plane::Transform(const Matrix4& matrix) {
	*this = Plane(matrix.Inverse().Transpose() * ToVector4());
}


/* ********** Plane Operations ********** */
Vector3 Plane::Project(const Vector3& point) const {
	return point - (Distance(point) * normal);
}

Vector3 Plane::Reflect(const Vector3& incident) const {
	return Vector3::Reflect(incident, normal);
}

float Plane::Distance(const Vector3& point) const {
	return Vector3::Dot(normal, point) + d;
}

bool Plane::Normalize() {
	return normal.Normalize();
}

Plane Plane::Flip() const {
	return Plane(normal * -1.0f, -d);
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

Plane Plane::Up() {
	return Plane(Vector3::Up());
}

Plane Plane::Right() {
	return Plane(Vector3::Right());
}

Plane Plane::Forward() {
	return Plane(Vector3::Forward());
}
