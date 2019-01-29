#include "Plane.hpp"
#include "Matrix3.hpp"
#include "Matrix4.hpp"
#include "Vector4.hpp"

using namespace zero::math;

Plane::Plane(const Vector4& plane)
   : normal_(Vector3::Normalize(Vector3(plane.x_, plane.y_, plane.z_))), d_(plane.w_) {}

Plane::Plane(const Vector3& normal, float d)
   : normal_(normal), d_(d) {}

Plane::Plane(const Vector3& normal, const Vector3& point)
   : normal_(normal), d_(Vector3::Dot(normal, point)) {}

Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
   : normal_(Vector3::Normalize(Vector3::Cross(p2 - p1, p3 - p1))), d_(-1.0f * Vector3::Dot(normal_, p1)) {}

Plane::Plane(const Vector3& normal)
   : normal_(normal), d_(0.0f) {}

bool Plane::operator==(const Plane& other) const {
	return (normal_ == other.normal_) && Equal(d_, other.d_);
}

bool Plane::operator!=(const Plane& other) const {
	return !operator==(other);
}

Vector4 Plane::ToVector4() const {
	return Vector4(normal_.x_, normal_.y_, normal_.z_, d_);
}


/* ********** Intersection Tests ********** */
bool Plane::Intersects(const Plane& other, float epsilon) const {
	return !( Vector3::Cross(normal_, other.normal_).IsEpsilon(epsilon) );
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
	return point - (Distance(point) * normal_);
}

Vector3 Plane::Reflect(const Vector3& incident) const {
	return Vector3::Reflect(incident, normal_);
}

float Plane::Distance(const Vector3& point) const {
	return Vector3::Dot(normal_, point) + d_;
}

bool Plane::Normalize() {
	return normal_.Normalize();
}

Plane Plane::Flip() const {
	return Plane(normal_ * -1.0f, -d_);
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
