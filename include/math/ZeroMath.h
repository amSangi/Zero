#pragma once

#include <cmath>
#include <cfloat>

namespace Zero {

    /* ********** Forward Declarations ********** */
    class Degree;
    class Matrix3;
    class Matrix4;
    class Quaternion;
    class Radian;
    template<int dims> class Vector;
    using Vector2 = Vector<2>;
    using Vector3 = Vector<3>;
    using Vector4 = Vector<4>;

    /* ********** Constants ********** */
    static const float PI = 3.141593;
    static const float DEGREE_TO_RADIAN = PI / 180;
    static const float RADIAN_TO_DEGREE = 180 / PI;
    static const float EPSILON = FLT_EPSILON;

    /* ********** Types ********** */
    using size_t = std::size_t;

    /* ********** Basic Math Functions ********** */

    inline float sqrt(float value)                         { return std::sqrtf(value);   }
    inline float abs(float value)                          { return std::fabsf(value);   }
    inline float sin(float value)                          { return std::sinf(value);    }
    inline float cos(float value)                          { return std::cosf(value);    }
    inline float tan(float value)                          { return std::tanf(value);    }
    inline float acos(float value)                         { return std::acosf(value);   }
    inline float asin(float value)                         { return std::asinf(value);   }
    inline float atan(float value)                         { return std::atanf(value);   }
    inline float atan2(float a, float b)                   { return std::atan2f(a, b); }
    inline float copysign(float a, float b)                { return std::copysignf(a, b); }
    inline bool equal(float a, float b)                    { return std::abs(a - b) < EPSILON; }
    inline float max(float a, float b)                     { return fmax(a, b); }
    inline float min(float a, float b)                     { return fmin(a, b); }

    /* ********** Basic Classes ********** */

    class Radian {
    public:
        Radian() : rad(0.0f) {}
        explicit Radian(float r) : rad(r) {}
        Radian(const Radian& other) = default;
        ~Radian() = default;

        Radian& operator=(const Radian& other) = default;


        /* ********** Comparison Operators ********** */
        bool operator==(const Radian& r) const             { return equal(rad, r.rad); }
        bool operator!=(const Radian& r) const             { return !equal(rad, r.rad); }
        bool operator>=(const Radian& r) const             { return rad >= r.rad; }
        bool operator>(const Radian& r)  const             { return rad > r.rad;  }
        bool operator<=(const Radian& r) const             { return rad <= r.rad; }
        bool operator<(const Radian& r)  const             { return rad < r.rad;  }

        bool operator==(const Degree& d) const;
        bool operator!=(const Degree& d) const;
        bool operator>=(const Degree& d) const;
        bool operator>(const Degree& d)  const;
        bool operator<=(const Degree& d) const;
        bool operator<(const Degree& d)  const;

        /* ********** Math Operators ********** */
        Radian operator+(const Radian& r) const            { return Radian(rad + r.rad); }
        Radian operator-(const Radian& r) const            { return Radian(rad - r.rad); }
        Radian operator*(const Radian& r) const            { return Radian(rad * r.rad); }
        Radian operator/(const Radian& r) const            { return Radian(rad / r.rad); }

        Radian& operator+=(const Radian& r)                { rad += r.rad; return *this; }
        Radian& operator-=(const Radian& r)                { rad -= r.rad; return *this; }
        Radian& operator*=(const Radian& r)                { rad *= r.rad; return *this; }
        Radian& operator/=(const Radian& r)                { rad /= r.rad; return *this; }

        Radian operator+(const Degree& d) const;
        Radian operator-(const Degree& d) const;
        Radian operator*(const Degree& d) const;
        Radian operator/(const Degree& d) const;

        Radian& operator+=(const Degree& d);
        Radian& operator-=(const Degree& d);
        Radian& operator*=(const Degree& d);
        Radian& operator/=(const Degree& d);

        Degree ToDegree() const;


        float rad;

    }; // class Radian

    class Degree {
    public:
        Degree() : deg(0.0f) {}
        explicit Degree(float d) : deg(d) {}
        Degree(const Degree& other) = default;
        ~Degree() = default;

        Degree& operator=(const Degree& other) = default;


        /* ********** Comparison Operators ********** */
        bool operator==(const Degree& d) const             { return equal(deg, d.deg); }
        bool operator!=(const Degree& d) const             { return !equal(deg, d.deg); }
        bool operator>=(const Degree& d) const             { return deg >= d.deg; }
        bool operator>(const Degree& d)  const             { return deg > d.deg;  }
        bool operator<=(const Degree& d) const             { return deg <= d.deg; }
        bool operator<(const Degree& d)  const             { return deg < d.deg;  }

        bool operator==(const Radian& r) const             { return operator==(r.ToDegree()); }
        bool operator!=(const Radian& r) const             { return operator!=(r.ToDegree()); }
        bool operator>=(const Radian& r) const             { return operator>=(r.ToDegree()); }
        bool operator>(const Radian& r)  const             { return operator>(r.ToDegree());  }
        bool operator<=(const Radian& r) const             { return operator<=(r.ToDegree()); }
        bool operator<(const Radian& r)  const             { return operator<(r.ToDegree());  }

        /* ********** Math Operators ********** */
        Degree operator+(const Degree& d) const            { return Degree(deg + d.deg); }
        Degree operator-(const Degree& d) const            { return Degree(deg - d.deg); }
        Degree operator*(const Degree& d) const            { return Degree(deg * d.deg); }
        Degree operator/(const Degree& d) const            { return Degree(deg / d.deg); }

        Degree& operator+=(const Degree& d)                { deg += d.deg; return *this; }
        Degree& operator-=(const Degree& d)                { deg -= d.deg; return *this; }
        Degree& operator*=(const Degree& d)                { deg *= d.deg; return *this; }
        Degree& operator/=(const Degree& d)                { deg /= d.deg; return *this; }

        Degree operator+(const Radian& r) const            { return Degree(deg + r.ToDegree().deg); }
        Degree operator-(const Radian& r) const            { return Degree(deg - r.ToDegree().deg); }
        Degree operator*(const Radian& r) const            { return Degree(deg * r.ToDegree().deg); }
        Degree operator/(const Radian& r) const            { return Degree(deg / r.ToDegree().deg); }

        Degree& operator+=(const Radian& r)                { deg += r.ToDegree().deg; return *this; }
        Degree& operator-=(const Radian& r)                { deg -= r.ToDegree().deg; return *this; }
        Degree& operator*=(const Radian& r)                { deg *= r.ToDegree().deg; return *this; }
        Degree& operator/=(const Radian& r)                { deg /= r.ToDegree().deg; return *this; }

        Radian ToRadian() const                            { return Radian(deg * DEGREE_TO_RADIAN); }


        float deg;

    }; // class Degree

} // namespace Zero



