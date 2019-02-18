#include "Box.hpp"
#include "Sphere.hpp"

using namespace zero::math;

Box::Box(const Vector3& min, const Vector3& max)
   : min_(min), max_(max) {}

bool Box::operator==(const Box& other) const {
    return (min_ == other.min_) && (max_ == other.max_);
}

bool Box::operator!=(const Box& other) const {
    return !operator==(other);
}

/* ********** Intersection Tests ********** */
bool Box::Contains(const Box& other) const {
	return Contains(other.min_) && Contains(other.max_);
}

bool Box::Contains(const Vector3& point) const {
	return (min_.x_ <= point.x_) &&
           (min_.y_ <= point.y_) &&
           (min_.z_ <= point.z_) &&
           (point.x_ <= max_.x_) &&
           (point.y_ <= max_.y_) &&
           (point.z_ <= max_.z_);
}

bool Box::Intersects(const Box& other) const {
	return !((min_.x_ > other.max_.x_ || other.min_.x_ > max_.x_) ||
             (min_.y_ > other.max_.y_ || other.min_.y_ > max_.y_) ||
             (min_.z_ > other.max_.z_ || other.min_.z_ > max_.z_));
}


/* ********** Merge ********** */
void Box::Merge(const Box& other) {
	min_.x_ = Min(min_.x_, other.min_.x_);
	min_.y_ = Min(min_.y_, other.min_.y_);
	min_.z_ = Min(min_.z_, other.min_.z_);

	max_.x_ = Max(max_.x_, other.max_.x_);
	max_.y_ = Max(max_.y_, other.max_.y_);
	max_.z_ = Max(max_.z_, other.max_.z_);
}


/* ********** Box Operations ********** */
Vector3 Box::Size() const {
	return max_ - min_;
}

Vector3 Box::Center() const {
	return (min_ + max_) * 0.5f;
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

