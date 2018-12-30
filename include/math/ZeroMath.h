#pragma once

#include <cmath>

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
    static const float PI = 3.14159265359f;
    static const float PI_2 = 3.14159265359f * 0.5f;
    static const float DEGREE_TO_RADIAN = PI / 180.0f;
    static const float RADIAN_TO_DEGREE = 180.0f / PI;
    static const float EPSILON = 1e-06f;

    /* ********** Types ********** */
    using size_t = std::size_t;

    /* ********** Basic Math Functions ********** */

    inline float sqrt(float value)                         { return std::sqrtf(value);     }
    inline float abs(float value)                          { return std::fabsf(value);     }
    inline float sin(float value)                          { return std::sinf(value);      }
    inline float cos(float value)                          { return std::cosf(value);      }
    inline float tan(float value)                          { return std::tanf(value);      }
    inline float acos(float value)                         { return std::acosf(value);     }
    inline float asin(float value)                         { return std::asinf(value);     }
    inline float atan(float value)                         { return std::atanf(value);     }
    inline float atan2(float a, float b)                   { return std::atan2f(a, b);     }
    inline float copysign(float a, float b)                { return std::copysignf(a, b);  }
    inline float max(float a, float b)                     { return std::fmax(a, b);       }
    inline float min(float a, float b)                     { return std::fmin(a, b);       }
    inline float ceil(float a)                             { return std::ceil(a);          }
    inline float floor(float a)                            { return std::floor(a);         }
    inline bool equal(float a,
                      float b,
                      float eps = EPSILON)                 { return std::abs(a - b) < eps; }

} // namespace Zero



