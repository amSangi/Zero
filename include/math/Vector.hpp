#pragma once

#include "Angle.hpp"
#include "ZMath.hpp"
#include <string>
#include <sstream>

namespace zero::math
{

namespace internal
{

    /**
     * @brief A simple base container for the vector
     * @tparam dims The dimensions of the vector
     * @tparam T The vector element type
     */
    template<int dims, class T>
    class VectorBase
    {
    public:
        VectorBase() = default;
        explicit VectorBase(T value)
        {
            for (uint32 i = 0; i < dims; ++i)
            {
                data_[i] = value;
            }
        }

        inline T* Data() { return data_; }
        inline const T* Data() const { return data_; }

        T data_[dims];

    }; // template class VectorBase

    template<class T>
    class VectorBase<2, T>
    {
    public:
        VectorBase() = default;
        VectorBase(T x, T y) : x_(x), y_(y) {}
        explicit VectorBase(T value) : x_(value), y_(value) {}

        inline T* Data() { return &x_; }
        inline const T* Data() const { return &x_; }

        T x_, y_;

    }; // partial template specialization class VectorBase<2>


    template<class T>
    class VectorBase<3, T>
    {
    public:
        VectorBase() = default;
        VectorBase(T x, T y) : x_(x), y_(y), z_(0.0F) {}
        VectorBase(T x, T y, T z) : x_(x), y_(y), z_(z) {}
        explicit VectorBase(T value) : x_(value), y_(value), z_(value) {}

        inline T* Data() { return &x_; }
        inline const T* Data() const { return &x_; }

        T x_, y_, z_;

    }; // partial template specialization class VectorBase<3>


    template<class T>
    class VectorBase<4, T>
    {
    public:
        VectorBase() = default;
        VectorBase(T x, T y) : x_(x), y_(y), z_(0.0F), w_(0.0F) {}
        VectorBase(T x, T y, T z) : x_(x), y_(y), z_(z), w_(0.0F) {}
        VectorBase(T x, T y, T z, T w) : x_(x), y_(y), z_(z), w_(w) {}
        explicit VectorBase(T value) : x_(value), y_(value), z_(value), w_(value) {}

        inline T* Data() { return &x_; }
        inline const T* Data() const { return &x_; }

        T x_, y_, z_, w_;

    }; // partial template specialization class VectorBase<4>

} // namespace internal


    /**
     * @brief A dims sized mathematical vector
     * @tparam dims The vector dimensions
     * @tparam T The vector element type
     */
    template<uint16 dims, class T>
    class Vector  : public internal::VectorBase<dims, T>
    {
    public:
        using VectorBase = internal::VectorBase<dims, T>;
        using VectorBase::Data;
        using Vec = Vector<dims, T>;

        Vector<dims, T>() = default;
        Vector<dims, T>(const Vec& other) = default;
        explicit Vector<dims, T>(T value) : VectorBase(value) {}
        Vector<dims, T>(T x, T y) : VectorBase(x, y) {};
        Vector<dims, T>(T x, T y, T z) : VectorBase(x, y, z) {}
        Vector<dims, T>(T x, T y, T z, T w) : VectorBase(x, y, z, w) {}

        ~Vector<dims, T>() = default;
        Vector<dims, T>& operator=(const Vec& other) = default;

        /**
         * @brief Get the component at the index
         * @param index The index
         * @return the component
         */
        T operator[](size_t index);

        /**
         * @brief Get the component at the index
         * @param index The index
         * @return the component
         */
        T operator[](size_t index) const;

        /**
         * @brief Check if the two given vectors are equal
         * @param o The other vector
         * @return True if the components are equal. False otherwise.
         */
        inline bool operator==(const Vec& o) const;

        /**
         * @brief Check if the two given vectors are not equal
         * @param o The other vector
         * @return True if the components are not equal. False otherwise.
         */
        inline bool operator!=(const Vec& o) const;

        /* ********** Scalar Operations ********** */

        /**
         * @brief Add the components by the given scalar
         * @param scalar The scalar
         * @return A new Vector containing the sums
         */
        Vec operator+(T scalar) const;

        /**
         * @brief Subtract the components by the given scalar
         * @param scalar The scalar
         * @return A new Vector containing the differences
         */
        Vec operator-(T scalar) const;

        /**
         * @brief Multiply the components by the given scalar
         * @param scalar The scalar
         * @return A new Vector containing the products
         */
        Vec operator*(T scalar) const;

        /**
         * @brief Divide the components by the given scalar
         * @param scalar The scalar
         * @return A new Vector containing the quotients
         */
        Vec operator/(T scalar) const;

        /**
         * @brief Add the components by the given scalar
         * @param scalar The scalar
         * @return The same Vector containing the sums
         */
        Vec& operator+=(T scalar);

        /**
         * @brief Subtract the components by the given scalar
         * @param scalar The scalar
         * @return The same Vector containing the differences
         */
        Vec& operator-=(T scalar);

        /**
         * @brief Multiply the components by the given scalar
         * @param scalar The scalar
         * @return The same Vector containing the products
         */
        Vec& operator*=(T scalar);

        /**
         * @brief Divide the components by the given scalar
         * @param scalar The scalar
         * @return The same Vector containing the quotients
         */
        Vec& operator/=(T scalar);

        /* **** Component-Wise Vector Operations **** */

        /**
         * @brief Add the components of this with the components of rhs
         * @param rhs The right vector
         * @return A new Vector containing the sums
         */
        Vec operator+(const Vec& rhs) const;

        /**
         * @brief Subtract the components of this by the components of rhs
         * @param rhs The right vector
         * @return A new Vector containing the differences
         */
        Vec operator-(const Vec& rhs) const;

        /**
         * @brief Multiply the components of this by the components of rhs
         * @param rhs The right vector
         * @return A new Vector containing the products
         */
        Vec operator*(const Vec& rhs) const;

        /**
         * @brief Divide the components of this by the components of rhs
         * @param rhs The right vector
         * @return A new Vector containing the quotients
         */
        Vec operator/(const Vec& rhs) const;

        /**
         * @brief Add the components of this with the components of rhs
         * @param rhs The right vector
         * @return The same Vector containing the sums
         */
        Vec& operator+=(const Vec& rhs);

        /**
         * @brief Subtract the components of this by the components of rhs
         * @param rhs The right vector
         * @return The same Vector containing the differences
         */
        Vec& operator-=(const Vec& rhs);

        /**
         * @brief Multiply the components of this by the components of rhs
         * @param rhs The right vector
         * @return The same Vector containing the products
         */
        Vec& operator*=(const Vec& rhs);

        /**
         * @brief Divide the components of this by the components of rhs
         * @param rhs The right vector
         * @return The same Vector containing the quotients
         */
        Vec& operator/=(const Vec& rhs);

        /* ********** Vector Operations ********** */

        /**
         * @brief Get the magnitude
         * @return the magnitude
         */
        [[nodiscard]] float Magnitude() const;

        /**
         * @brief Get the squared magnitude
         * @return the square magnitude
         */
        [[nodiscard]] float SquareMagnitude() const;

        /**
         * @brief Take the absolute value of all the components
         * @return The same vector (this)
         */
        Vec& Abs();

        /**
         * @brief Normalize the vector if it's possible
         * @return the magnitude
         */
        float Normalize();

        /**
         * @brief Check to see if all of the components are less than or equal to an epsilon value
         * @param epsilon the epsilon value (very small)
         * @return True if the components are <= epsilon. False otherwise.
         */
        [[nodiscard]] bool IsEpsilon(float epsilon = kEpsilon) const;

        /**
         * @brief Helper to convert to a 3D Vector of the same type
         * @return a 3D vector of the same type containing the x, y, and z coordinates.
         *     If the calling vector has less dimensions, the extra coordinates will default to 0.
         */
        Vector<3, T> XYZ() const;

        /**
         * @brief Helper to convert to a 2D Vector of the same type
         * @return a 2D vector of the same type containing the x and y coordinates.
         *     If the calling vector has less dimensions, the extra coordinates will default to 0.
         */
        Vector<2, T> XY() const;

        /**
         * @brief Convert the vector into the string form: <x, y, ...>
         * @return a string representation of the vector.
         */
        [[nodiscard]] std::string ToString() const;

        /* ********** Static Operations ********** */

        /**
         * @brief Get a copy of the normalized vector
         * @param v the vector
         * @return a copy of the vector normalized
         */
        static inline Vec Normalize(const Vec& v);

        /**
         * @brief Get a copy of the vector with all of its components being >= 0
         * @param v the vector
         * @return the absolute valued vector copy
         */
        static inline Vec AbsCopy(const Vec& v);

        /**
         * @brief Get the dot product between two vectors
         * @param lhs The left vector
         * @param rhs The right vector
         * @return the dot product
         */
        static inline float Dot(const Vec& lhs, const Vec& rhs);

        /**
         * @brief Get the cross product between two vectors
         * @param lhs The left vector
         * @param rhs The right vector
         * @return the cross product
         */
        static inline Vec Cross(const Vec& lhs, const Vec& rhs);

        /**
         * @brief Get the outgoing, reflected vector
         * @param in The incident vector
         * @param normal The normal vector
         * @return the reflected vector
         */
        static inline Vec Reflect(const Vec& in, const Vec& normal);

        /**
         * @brief Get the distance from `from` to `to`
         * @param from The source vector
         * @param to The destination vector
         * @return the distance between the two vectors
         */
        static inline float Distance(const Vec& from, const Vec& to);

        /**
         * @brief Get the square distance from `from` to `to`
         * @param from The source vector
         * @param to The destination vector
         * @return the square distance between the two vectors
         */
        static inline float SquareDistance(const Vec& from, const Vec& to);

        /**
         * @brief Get the linearly interpolated vector between the start and end vectors
         * @param start The start vector
         * @param end The end vector
         * @param t The amount to lerp by with range [0, 1]
         * @return The vector between the start and end (inclusive)
         */
        static inline Vec Lerp(const Vec& start, const Vec& end, float t);

        /**
         * @brief Get the angle between the two vector
         * @param from The source vector
         * @param to The destination vector
         * @return the angle between the two vectors
         */
        static inline Radian Angle(const Vec& from, const Vec& to);


        /* ********** Useful Vectors ********** */

        /**
         * @brief Create the zero vector
         * @return the Zero vector
         */
        static Vec Zero();

        /**
         * @brief Create the unit vector
         * @return the Unit vector
         */
        static Vec One();

        /**
         * @brief Create the up vector (+y is up)
         * @return the Up vector
         */
        static Vec Up();

        /**
         * @brief Create the down vector (-y is down)
         * @return the Down vector
         */
        static Vec Down();

        /**
         * @brief Create the right vector (+x is right)
         * @return the Right vector
         */
        static Vec Right();

        /**
         * @brief Create the left vector (-x is left)
         * @return the Left vector
         */
        static Vec Left();

        /**
         * @brief Create the forward vector (+z is forward and out of the screen)
         * @return the Forward vector
         */
        static Vec Forward();

        /**
         * @brief Create the back vector (-z is back and into the screen)
         * @return the Back vector
         */
        static Vec Back();

    }; // template class Vector

    /* ********** Vectors ********** */
    using Vec2f = Vector<2, float>;
    using Vec3f = Vector<3, float>;
    using Vec4f = Vector<4, float>;
    using Vec2i = Vector<2, int32>;
    using Vec3i = Vector<3, int32>;
    using Vec4i = Vector<4, int32>;

    /* ********** Useful Vectors ********** */

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Zero()
    {
        return Vector<dims, T>(0.0F);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::One()
    {
        return Vector<dims, T>(1.0F);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Up()
    {
        static_assert(dims > 1, "Require at least 2 dimensions");
        return Vector<dims, T>(0.0F, 1.0F);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Down()
    {
        static_assert(dims > 1, "Require at least 2 dimensions");
        return Vector<dims, T>(0.0F, -1.0F);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Right()
    {
        static_assert(dims > 1, "Require at least 2 dimensions");
        return Vector<dims, T>(1.0F, 0.0F);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Left()
    {
        static_assert(dims > 1, "Require at least 2 dimensions");
        return Vector<dims, T>(-1.0F, 0.0F);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Forward()
    {
        static_assert(dims > 2, "Require at least 3 dimensions");
        return Vector<dims, T>(0.0F, 0.0F, 1.0F);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Back()
    {
        static_assert(dims > 2, "Require at least 3 dimensions");
        return Vector<dims, T>(0.0F, 0.0F, -1.0F);
    }


    /* ********** Vector Operations Implementation ********** */

    template<uint16 dims, class T>
    float Vector<dims, T>::Magnitude() const
    {
        return Sqrt(SquareMagnitude());
    }

    template<uint16 dims, class T>
    float Vector<dims, T>::SquareMagnitude() const
    {
        float square_mag = 0.0F;
        const T* data = Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            square_mag += (data[i] * data[i]);
        }
        return square_mag;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::Abs()
    {
        T* data = Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] = math::Abs(data[i]);
        }
        return *this;
    }

    template<uint16 dims, class T>
    float Vector<dims, T>::Normalize()
    {
        float magnitude = Magnitude();

        float inv_magnitude = 1.0F / magnitude;
        if (magnitude > kSmallEpsilon)
        {
            operator*=(inv_magnitude);
            return magnitude;
        }

        return 0.0F;
    }

    template<uint16 dims, class T>
    bool Vector<dims, T>::IsEpsilon(float epsilon) const
    {
        const T* data = Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            if (math::Abs(data[i]) > epsilon)
            {
                return false;
            }
        }

        return true;
    }

    template<uint16 dims, class T>
    Vector<3, T> Vector<dims, T>::XYZ() const
    {
        const T* data = Data();
        switch (dims)
        {
            case 1: return Vector<3, T>(data[0], 0, 0);
            case 2: return Vector<3, T>(data[0], data[1], 0);
            default: return Vector<3, T>(data[0], data[1], data[2]);
        }
    }

    template<uint16 dims, class T>
    Vector<2, T> Vector<dims, T>::XY() const
    {
        const T* data = Data();
        if constexpr(dims == 1) return Vector<2, T>(data[0], 0);
        else return Vector<2, T>(data[0], data[1]);
    }

    template<uint16 dims, class T>
    std::string Vector<dims, T>::ToString() const
    {
        std::stringstream result;
        result << "<";
        if constexpr (dims == 0)
        {
            result << ">";
            return result.str();
        }
        auto data = Data();
        for (uint32 i = 0; i < dims - 1; ++i)
        {
            result << data[i] << ", ";
        }
        result << data[dims - 1] <<  ">";
        return result.str();
    }

    template<uint16 dims, class T>
    std::ostream& operator<<(std::ostream& os, const Vector<dims, T>& vector)
    {
        return os << vector.ToString();
    }

    /* ********** Static Operations Implementation ********** */

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Normalize(const Vector<dims, T>& v)
    {
        Vector<dims, T> copy = v;
        return copy.Normalize() ? copy : Zero();
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::AbsCopy(const Vector<dims, T>& v)
    {
        Vector<dims, T> copy = v;
        copy.Abs();
        return copy;
    }

    template<uint16 dims, class T>
    float Vector<dims, T>::Dot(const Vector<dims, T>& lhs, const Vector<dims, T>& rhs)
    {
        float result = 0.0F;
        const T* lhs_data = lhs.Data();
        const T* rhs_data = rhs.Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            result += (lhs_data[i] * rhs_data[i]);
        }
        return result;
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Cross(const Vector<dims, T>& lhs, const Vector<dims, T>& rhs)
    {
        static_assert(dims == 3, "Only 3D cross product supported");
        return Vector<dims, T>(lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
                               lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
                               lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::Reflect(const Vector<dims, T>& in, const Vector<dims, T>& normal)
    {
        return in - (normal * 2 * Dot(in, normal));
    }

    template<uint16 dims, class T>
    float Vector<dims, T>::Distance(const Vector<dims, T>& from, const Vector<dims, T>& to)
    {
        return (from - to).Magnitude();
    }

    template<uint16 dims, class T>
    float Vector<dims, T>::SquareDistance(const Vector<dims, T>& from, const Vector<dims, T>& to)
    {
        return (from - to).SquareMagnitude();
    }

    template<uint16 dims, class T>
    Vector<dims, T>Vector<dims, T>::Lerp(const Vector<dims, T>& start, const Vector<dims, T>& end, float t)
    {
        return (start * (1.0F - t)) + (end * t);
    }

    template<uint16 dims, class T>
    Radian Vector<dims, T>::Angle(const Vector<dims, T>& from, const Vector<dims, T>& to)
    {
        float dot = Dot(from, to);
        float square_mag_from = from.SquareMagnitude();
        float square_mag_to = to.SquareMagnitude();
        float inv_sqrt = 1.0F / Sqrt(square_mag_from * square_mag_to);
        float angle = Acos(dot * inv_sqrt);
        return Radian(angle);
    }


    /* ********** Operator Overload Implementation ********** */

    template<uint16 dims, class T>
    bool Vector<dims, T>::operator==(const Vector<dims, T>& o) const
    {
        const T* data = Data();
        const T* rhs_data = o.Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            if (!Equal(data[i], rhs_data[i]))
            {
                return false;
            }
        }

        return true;
    }

    template<uint16 dims, class T>
    T Vector<dims, T>::operator[](size_t index)
    {
        return Data()[index];
    }

    template<uint16 dims, class T>
    T Vector<dims, T>::operator[](size_t index) const
    {
        return Data()[index];
    }

    template<uint16 dims, class T>
    bool Vector<dims, T>::operator!=(const Vector<dims, T>& o) const
    {
        return !operator==(o);
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::operator+(T scalar) const
    {
        return Vector<dims, T>(*this) += scalar;
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::operator-(T scalar) const
    {
        return Vector<dims, T>(*this) -= scalar;
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::operator*(T scalar) const
    {
        return Vector<dims, T>(*this) *= scalar;
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::operator/(T scalar) const
    {
        return Vector<dims, T>(*this) /= scalar;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::operator+=(T scalar)
    {
        T* data = Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] += scalar;
        }
        return *this;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::operator-=(T scalar)
    {
        T* data = Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] -= scalar;
        }
        return *this;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::operator*=(T scalar)
    {
        T* data = Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] *= scalar;
        }
        return *this;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::operator/=(T scalar)
    {
        T* data = Data();
        float inv_scalar = 1.0F / scalar;
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] *= inv_scalar;
        }
        return *this;
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::operator+(const Vector<dims, T>& rhs) const
    {
        return Vector<dims, T>(*this) += rhs;
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::operator-(const Vector<dims, T>& rhs) const
    {
        return Vector<dims, T>(*this) -= rhs;
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::operator*(const Vector<dims, T>& rhs) const
    {
        return Vector<dims, T>(*this) *= rhs;
    }

    template<uint16 dims, class T>
    Vector<dims, T> Vector<dims, T>::operator/(const Vector<dims, T>& rhs) const
    {
        return Vector<dims, T>(*this) /= rhs;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::operator+=(const Vector<dims, T>& rhs)
    {
        T* data = Data();
        const T* rhs_data = rhs.Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] += rhs_data[i];
        }
        return *this;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::operator-=(const Vector<dims, T>& rhs)
    {
        T* data = Data();
        const T* rhs_data = rhs.Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] -= rhs_data[i];
        }
        return *this;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::operator*=(const Vector<dims, T>& rhs)
    {
        T* data = Data();
        const T* rhs_data = rhs.Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] *= rhs_data[i];
        }
        return *this;
    }

    template<uint16 dims, class T>
    Vector<dims, T>& Vector<dims, T>::operator/=(const Vector<dims, T>& rhs)
    {
        T* data = Data();
        const T* rhs_data = rhs.Data();
        for (uint32 i = 0; i < dims; ++i)
        {
            data[i] /= rhs_data[i];
        }
        return *this;
    }


    // Scalar/Vector Operations
    template<uint16 dims, class T>
    inline Vector<dims, T> operator+(T scalar, const Vector<dims, T>& v)
    {
        return v + scalar;
    }

    template<uint16 dims, class T>
    inline Vector<dims, T> operator-(T scalar, const Vector<dims, T>& v)
    {
        return v - scalar;
    }

    template<uint16 dims, class T>
    inline Vector<dims, T> operator*(T scalar, const Vector<dims, T>& v)
    {
        return v * scalar;
    }

    template<uint16 dims, class T>
    inline Vector<dims, T> operator/(T scalar, const Vector<dims, T>& v)
    {
        return v / scalar;
    }

} // namespace zero::math