#pragma once

#include <algorithm>
#include <cmath>
#include "core/ZeroBase.hpp"

namespace zero::math
{

    /* ********** Forward Declarations ********** */
    class Box;
    class Degree;
    class Matrix3x3;
    class Matrix4x4;
    class Plane;
    class Quaternion;
    class Radian;
    class Sphere;
    template<uint16 dims, class T> class Vector;
    using Vec2f = Vector<2, float>;
    using Vec3f = Vector<3, float>;
    using Vec4f = Vector<4, float>;
    using Vec2i = Vector<2, int32>;
    using Vec3i = Vector<3, int32>;
    using Vec4i = Vector<4, int32>;

    /* ********** Constants ********** */
    static constexpr float kPi = 3.14159265359F;
    static constexpr float kTwoPi = kPi * 2.0F;
    static constexpr float kHalfPi = kPi * 0.5F;
    static constexpr float kDegreeToRadian = kPi / 180.0F;
    static constexpr float kRadianToDegree = 180.0F / kPi;
    static constexpr float kSmallEpsilon = 1.19209290E-07F;
    static constexpr float kEpsilon = 1e-06F;

    /* ********** Basic Math Functions ********** */
    inline double FMod(double numer, double denom)             { return std::fmod(numer, denom);      }
    inline float Sqrt(float value)                             { return sqrtf(value);                 }
    inline float Abs(float value)                              { return fabsf(value);                 }
    inline float Sin(float value)                              { return sinf(value);                  }
    inline float Cos(float value)                              { return cosf(value);                  }
    inline float Tan(float value)                              { return tanf(value);                  }
    inline float Acos(float value)                             { return acosf(value);                 }
    inline float Asin(float value)                             { return asinf(value);                 }
    inline float Atan(float value)                             { return atanf(value);                 }
    inline float Atan2(float a, float b)                       { return atan2f(a, b);                 }
    inline float Copysign(float a, float b)                    { return copysignf(a, b);              }
    inline float Max(float a, float b)                         { return std::fmax(a, b);              }
    inline float Min(float a, float b)                         { return std::fmin(a, b);              }
    inline float Ceil(float a)                                 { return std::ceil(a);                 }
    inline float Floor(float a)                                { return std::floor(a);                }
    inline float Clamp(float value, float low, float high)     { return std::clamp(value, low, high); }
    inline bool  Equal(float a, float b, float eps = kEpsilon) { return std::abs(a - b) < eps;        }
    inline float Pow(float base, float exponent)               { return std::pow(base, exponent);     }
    inline float Lerp(float a, float b, float t)               { return (a * (1.0F - t)) + (b * t);   }

} // namespace zero::math



