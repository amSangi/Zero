#include "Box.h"
#include "Sphere.h"

using namespace Zero;

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
	return (min.x <= point.x) &&
           (min.y <= point.y) &&
           (min.z <= point.z) &&
           (point.x <= max.x) &&
           (point.y <= max.y) &&
           (point.z <= max.z);
}

bool Box::Intersects(const Box& other) const {
	return !((min.x > other.max.x || other.min.x > max.x) ||
             (min.y > other.max.y || other.min.y > max.y) ||
             (min.z > other.max.z || other.min.z > max.z));
}


/* ********** Merge ********** */
void Box::Merge(const Box& other) {
	min.x = Zero::Min(min.x, other.min.x);
	min.y = Zero::Min(min.y, other.min.y);
	min.z = Zero::Min(min.z, other.min.z);

	max.x = Zero::Max(max.x, other.max.x);
	max.y = Zero::Max(max.y, other.max.y);
	max.z = Zero::Max(max.z, other.max.z);
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

