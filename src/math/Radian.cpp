#include "ZeroMath.h"

using namespace Zero;

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
    return Radian(rad + d.ToRadian().rad);
}

Radian Radian::operator-(const Degree& d) const {
    return Radian(rad - d.ToRadian().rad);
}

Radian Radian::operator*(const Degree& d) const {
    return Radian(rad * d.ToRadian().rad);
}

Radian Radian::operator/(const Degree& d) const {
    return Radian(rad / d.ToRadian().rad);
}

Radian& Radian::operator+=(const Degree& d) {
    rad += d.ToRadian().rad;
    return *this;
}

Radian& Radian::operator-=(const Degree& d) {
    rad -= d.ToRadian().rad;
    return *this;
}

Radian& Radian::operator*=(const Degree& d) {
    rad *= d.ToRadian().rad;
    return *this;
}

Radian& Radian::operator/=(const Degree& d) {
    rad /= d.ToRadian().rad;
    return *this;
}

Degree Radian::ToDegree() const {
    return Degree(rad * RADIAN_TO_DEGREE);
}