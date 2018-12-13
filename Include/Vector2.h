#pragma once

#include "ZeroMath.h"

namespace Zero {

    class Vector2 {
    public:
        Vector2() : x(0.0f), y(0.0f) {}
        Vector2(float x, float y) : x(x), y(y) {}
        Vector2(const Vector2& other) = default;
        explicit Vector2(const float* data) : x(data[0]), y(data[1]) {}
        ~Vector2() = default;
        Vector2& operator=(const Vector2& other) = default;

        bool operator==(const Vector2& o) const                       { return x == o.x && y == o.y; }
        bool operator!=(const Vector2& o) const                       { return !(operator==(o)); }

        /* ********** Scalar Operations ********** */
        Vector2 operator+(float scalar) const                         { return Vector2(x + scalar, y + scalar); }
        Vector2 operator-(float scalar) const                         { return Vector2(x - scalar, y - scalar); }
        Vector2 operator*(float scalar) const                         { return Vector2(x * scalar, y * scalar); }
        Vector2 operator/(float scalar) const                         { return Vector2(x / scalar, y / scalar); }

        Vector2& operator+=(float scalar)                             { x += scalar; y += scalar; return *this; }
        Vector2& operator-=(float scalar)                             { x -= scalar; y -= scalar; return *this; }
        Vector2& operator*=(float scalar)                             { x *= scalar; y *= scalar; return *this; }
        Vector2& operator/=(float scalar)                             { x /= scalar; y /= scalar; return *this; }

        /* **** Component-Wise Vector Operations **** */
        Vector2 operator+(const Vector2& rhs) const                   { return Vector2(x + rhs.x, y + rhs.y); }
        Vector2 operator-(const Vector2& rhs) const                   { return Vector2(x - rhs.x, y - rhs.y); }
        Vector2 operator*(const Vector2& rhs) const                   { return Vector2(x * rhs.x, y * rhs.y); }
        Vector2 operator/(const Vector2& rhs) const                   { return Vector2(x / rhs.x, y / rhs.y); }

        Vector2& operator+=(const Vector2& rhs)                       { x += rhs.x; y += rhs.y; return *this; }
        Vector2& operator-=(const Vector2& rhs)                       { x -= rhs.x; y -= rhs.y; return *this; }
        Vector2& operator*=(const Vector2& rhs)                       { x *= rhs.x; y *= rhs.y; return *this; }
        Vector2& operator/=(const Vector2& rhs)                       { x /= rhs.x; y /= rhs.y; return *this; }

        /* ********** Vector Operations ********** */
        float Magnitude() const                                       { return sqrt(SquareMagnitude()); }
        float SquareMagnitude() const                                 { return (x * x + y * y); }

        /* ********** Static Operations ********** */
        static Vector2 Normalize(const Vector2& v)                    { return v / v.Magnitude(); }

        static Vector2 Abs(const Vector2& v)                          { return Vector2(abs(v.x), abs(v.y)); }

        static float Dot(const Vector2& lhs,
                         const Vector2& rhs)                          { return lhs.x * rhs.x + lhs.y * rhs.y; }

        static Vector2 Reflect(const Vector2& in,
                               const Vector2& normal)                 { return in - (normal * 2 * Dot(in, normal)); }

        static float Distance(const Vector2& from,
                              const Vector2& to)                      { return (from - to).Magnitude(); }

        static float SquareDistance(const Vector2& from,
                                    const Vector2& to)                { return (from - to).SquareMagnitude(); }

        static Vector2 Lerp(const Vector2& start,
                            const Vector2& end,
                            float t)                                  { return (start * (1.0f - t)) + (end * t); }

        static Radian Angle(const Vector2& from, const Vector2& to)   {
            float dot = Dot(from, to);
            float det = from.x * to.y - from.y * to.x;
            return Radian(atan2(det, dot));
        }


        /* ********** Common Vectors ********** */
        static Vector2 Up()                                           { return Vector2(0.0f, 1.0f);  }
        static Vector2 Down()                                         { return Vector2(0.0f, -1.0f); }
        static Vector2 Right()                                        { return Vector2(1.0f, 0.0f);  }
        static Vector2 Left()                                         { return Vector2(-1.0f, 0.0f); }
        static Vector2 One()                                          { return Vector2(1.0f, 1.0f); }

        float x;
        float y;

    }; // class Vector2


    // Scalar Vector
    inline Vector2 operator+(float scalar, const Vector2& v)           { return v + scalar;  }
    inline Vector2 operator-(float scalar, const Vector2& v)           { return v - scalar;  }
    inline Vector2 operator*(float scalar, const Vector2& v)           { return v * scalar;  }
    inline Vector2 operator/(float scalar, const Vector2& v)           { return v / scalar;  }

} // namespace Zero