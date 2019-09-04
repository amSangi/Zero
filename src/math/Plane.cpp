#include "math/Plane.hpp"
#include "math/Matrix3x3.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Vector4.hpp"

using namespace zero::math;

Plane::Plane(const Vec4f& plane)
   : normal_(Vec3f::Normalize(Vec3f(plane.x_, plane.y_, plane.z_))), d_(plane.w_) {}

Plane::Plane(const Vec3f& normal, float d)
   : normal_(normal), d_(d) {}

Plane::Plane(const Vec3f& normal, const Vec3f& point)
   : normal_(normal), d_(Vec3f::Dot(normal, point)) {}

Plane::Plane(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3)
   : normal_(Vec3f::Normalize(Vec3f::Cross(p2 - p1, p3 - p1))), d_(-1.0F * Vec3f::Dot(normal_, p1)) {}

Plane::Plane(const Vec3f& normal)
   : normal_(normal), d_(0.0F) {}

bool Plane::operator==(const Plane& other) const {
	return (normal_ == other.normal_) && Equal(d_, other.d_);
}

bool Plane::operator!=(const Plane& other) const {
	return !operator==(other);
}

Vec4f Plane::ToVector4() const {
	return Vec4f(normal_.x_, normal_.y_, normal_.z_, d_);
}


/* ********** Intersection Tests ********** */
bool Plane::Intersects(const Plane& other, float epsilon) const {
	return !( Vec3f::Cross(normal_, other.normal_).IsEpsilon(epsilon) );
}


/* ********** Transform Operations ********** */
void Plane::Transform(const Matrix3x3& matrix) {
	Transform(Matrix4x4(matrix));
}

void Plane::Transform(const Matrix4x4& matrix) {
	*this = Plane(matrix.Inverse().Transpose() * ToVector4());
}


/* ********** Plane Operations ********** */
Vec3f Plane::Project(const Vec3f& point) const {
	return point - (Distance(point) * normal_);
}

Vec3f Plane::Reflect(const Vec3f& incident) const {
	return Vec3f::Reflect(incident, normal_);
}

float Plane::Distance(const Vec3f& point) const {
	return Vec3f::Dot(normal_, point) + d_;
}

bool Plane::Normalize() {
	return normal_.Normalize();
}

Plane Plane::Flip() const {
	return Plane(normal_ * -1.0F, -d_);
}

/* ********** Static Operations ********** */
Plane Plane::Transform(const Plane& plane, const Matrix3x3& matrix) {
	Plane plane_copy(plane);
	plane_copy.Transform(matrix);
	return plane_copy;
}

Plane Plane::Transform(const Plane& plane, const Matrix4x4& matrix) {
	Plane plane_copy(plane);
	plane_copy.Transform(matrix);
	return plane_copy;
}

Plane Plane::Up() {
	return Plane(Vec3f::Up());
}

Plane Plane::Right() {
	return Plane(Vec3f::Right());
}

Plane Plane::Forward() {
	return Plane(Vec3f::Forward());
}
