#include "Box.hpp"
#include "Sphere.hpp"

using namespace zero::math;

Box::Box(const Vector3& min, const Vector3& max)
   : min(min), max(max) {}

bool Box::operator==(const Box& other) const {
    return (min == other.min) && (max == other.max);
}

bool Box::operator!=(const Box& other) const {
    return !operator==(other);
}

/* ********** Intersection Tests ********** */
bool Box::Contains(const Box& other) const {
	return Contains(other.min) && Contains(other.max);
}

bool Box::Contains(const Vector3& point) const {
	return (min.x_ <= point.x_) &&
           (min.y_ <= point.y_) &&
           (min.z_ <= point.z_) &&
           (point.x_ <= max.x_) &&
           (point.y_ <= max.y_) &&
           (point.z_ <= max.z_);
}

bool Box::Intersects(const Box& other) const {
	return !((min.x_ > other.max.x_ || other.min.x_ > max.x_) ||
             (min.y_ > other.max.y_ || other.min.y_ > max.y_) ||
             (min.z_ > other.max.z_ || other.min.z_ > max.z_));
}


/* ********** Merge ********** */
void Box::Merge(const Box& other) {
	min.x_ = Min(min.x_, other.min.x_);
	min.y_ = Min(min.y_, other.min.y_);
	min.z_ = Min(min.z_, other.min.z_);

	max.x_ = Max(max.x_, other.max.x_);
	max.y_ = Max(max.y_, other.max.y_);
	max.z_ = Max(max.z_, other.max.z_);
}


/* ********** Box Operations ********** */
Vector3 Box::Size() const {
	return max - min;
}

Vector3 Box::Center() const {
	return (min + max) * 0.5f;
}


/* ********** Static Methods ********** */
Box Box::Merge(const Box& lhs, const Box& rhs) {
    Box lhs_copy(lhs);
    lhs_copy.Merge(rhs);
    return lhs_copy;
}

Box Box::Unit() {
	return Box(Vector3::Zero(), Vector3::One());
}

