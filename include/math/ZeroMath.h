#pragma once

#include <cmath>

namespace Zero {

    /* ********** Forward Declarations ********** */
    class BoundingBox;
    class Cone;
    class Degree;
    class Frustrum;
    class Line;
    class Matrix3;
    class Matrix4;
    class Plane;
    class Quaternion;
    class Radian;
    class Ray;
    class Sphere;
    template<int dims> class Vector;
    using Vector2 = Vector<2>;
    using Vector3 = Vector<3>;
    using Vector4 = Vector<4>;

    /* ********** Constants ********** */
    static const float PI = 3.14159265359f;
    static const float PI_2 = 3.14159265359f * 0.5f;
    static const float DEGREE_TO_RADIAN = PI / 180.0f;
    static const float RADIAN_TO_DEGREE = 180.0f / PI;
    static const float SMALL_EPSILON = 1.19209290E-07F;
    static const float EPSILON = 1e-06f;

    /* ********** Types ********** */
    using size_t = std::size_t;

    /* ********** Basic Math Functions ********** */

    inline float Sqrt(float value)                         { return sqrtf(value);          }
    inline float Abs(float value)                          { return fabsf(value);          }
    inline float Sin(float value)                          { return sinf(value);           }
    inline float Cos(float value)                          { return cosf(value);           }
    inline float Tan(float value)                          { return tanf(value);           }
    inline float Acos(float value)                         { return acosf(value);          }
    inline float Asin(float value)                         { return asinf(value);          }
    inline float Atan(float value)                         { return atanf(value);          }
    inline float Atan2(float a, float b)                   { return atan2f(a, b);          }
    inline float Copysign(float a, float b)                { return copysignf(a, b);       }
    inline float Max(float a, float b)                     { return std::fmax(a, b);       }
    inline float Min(float a, float b)                     { return std::fmin(a, b);       }
    inline float Ceil(float a)                             { return std::ceil(a);          }
    inline float Floor(float a)                            { return std::floor(a);         }
    inline bool Equal(float a,
                      float b,
                      float eps = EPSILON)                 { return std::abs(a - b) < eps; }

} // namespace Zero



