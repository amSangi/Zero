#include "math/Angle.hpp"

using namespace zero::math;

bool Radian::operator==(const Degree& d) const {
    return operator==(d.ToRadian());
}

bool Radian::operator!=(const Degree& d) const {
    return operator!=(d.ToRadian());
}

bool Radian::operator>=(const Degree& d) const {
    return operator>=(d.ToRadian());
}

bool Radian::operator>(const Degree& d) const {
    return operator>(d.ToRadian());
}

bool Radian::operator<=(const Degree& d) const {
    return operator<=(d.ToRadian());
}

bool Radian::operator<(const Degree& d) const {
    return operator<(d.ToRadian());
}

Radian Radian::operator+(const Degree& d) const {
    return Radian(rad_ + d.ToRadian().rad_);
}

Radian Radian::operator-(const Degree& d) const {
    return Radian(rad_ - d.ToRadian().rad_);
}

Radian Radian::operator*(const Degree& d) const {
    return Radian(rad_ * d.ToRadian().rad_);
}

Radian Radian::operator/(const Degree& d) const {
    return Radian(rad_ / d.ToRadian().rad_);
}

Radian& Radian::operator+=(const Degree& d) {
    rad_ += d.ToRadian().rad_;
    return *this;
}

Radian& Radian::operator-=(const Degree& d) {
    rad_ -= d.ToRadian().rad_;
    return *this;
}

Radian& Radian::operator*=(const Degree& d) {
    rad_ *= d.ToRadian().rad_;
    return *this;
}

Radian& Radian::operator/=(const Degree& d) {
    rad_ /= d.ToRadian().rad_;
    return *this;
}

Degree Radian::ToDegree() const {
    return Degree(rad_ * kRadianToDegree);
}