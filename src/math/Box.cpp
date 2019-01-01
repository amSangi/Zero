#include "Box.h"

using namespace Zero;

Box::Box(const Vector3& min, const Vector3& max)
   : min(min), max(max) {}

bool Box::operator==(const Box& other) const {
    return (min == other.min) && (max == other.max);
}

bool Box::operator!=(const Box& other) const {
    return !operator==(other);
}

bool Box::Contains(const Box& other) const {
    return false;
}

bool Box::Contains(const Frustrum& frustrum) const {
    return false;
}

bool Box::Contains(const Ray& ray) const {
    return false;
}

bool Box::Contains(const Sphere& sphere) const {
    return false;
}

bool Box::Contains(const Vector3& point) const {
    return false;
}

void Box::Merge(const Box& other) {

}

Box Box::Merge(const Box& lhs, const Box& rhs) {
    return Box();
}
