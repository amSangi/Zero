#pragma once

#include "Angle.hpp"
#include "Vector3.hpp"
#include "ZMath.hpp"

namespace zero {
namespace math {

	/**
	 * @brief A Quaternion used for orientations and rotations
	 */
	class Quaternion {
	public:
		Quaternion() : w_(1.0f), x_(0.0f), y_(0.0f), z_(0.0f) {}
		Quaternion(float w, float x, float y, float z) : w_(w), x_(x), y_(y), z_(z) {}
		Quaternion(const Quaternion& other) = default;
		~Quaternion() = default;

		Quaternion& operator=(const Quaternion& other) = default;

		/**
		 * @brief Check if this quaternion is equal to another
		 * @param other The other quaternion
		 * @return True if all of the components are equal. False otherwise.
		 */
		bool operator==(const Quaternion& other) const;

		/**
		 * @brief Check if this quaternion is not equal to another
		 * @param other The other quaternion
		 * @return True if one of the components are not equal. False otherwise.
		 */
		bool operator!=(const Quaternion& other) const;


		/* ********** Scalar Operations ********** */

		/**
		 * @brief Add the scalar to all the components
		 * @param scalar The scalar
		 * @return A new Quaternion containing the sums
		 */
		Quaternion operator+(float scalar);

		/**
		 * @brief Subtract the components by the scalar
		 * @param scalar The scalar
		 * @return A new Quaternion containing the differences
		 */
		Quaternion operator-(float scalar);

		/**
		 * @brief Multiply the components by the scalar
		 * @param scalar The scalar
		 * @return A new Quaternion containing the products
		 */
		Quaternion operator*(float scalar);

		/**
		 * @brief Divide the components by the scalar
		 * @param scalar The scalar
		 * @return A new Quaternion containing the quotients
		 */
		Quaternion operator/(float scalar);

		/**
		 * @brief Add the scalar to all the components
		 * @param scalar The scalar
		 * @return The same Quaternion containing the sums
		 */
		Quaternion& operator+=(float scalar);

		/**
		 * @brief Subtract the components by the scalar
		 * @param scalar The scalar
		 * @return The same Quaternion containing the differences
		 */
		Quaternion& operator-=(float scalar);

		/**
		 * @brief Multiply the components by the scalar
		 * @param scalar The scalar
		 * @return The same Quaternion containing the products
		 */
		Quaternion& operator*=(float scalar);

		/**
		 * @brief Divide the components by the scalar
		 * @param scalar The scalar
		 * @return The same Quaternion containing the quotients
		 */
		Quaternion& operator/=(float scalar);


		/* ********** Quaternion/Quaternion Operations ********** */

		/**
		 * @brief Add the components of the two quaternions together
		 * @param rhs The right quaternion
		 * @return A new Quaternion containing the component-wise sums
		 */
		Quaternion operator+(const Quaternion& rhs);

		/**
		 * @brief Subtract the components of this by the components of rhs
		 * @param rhs The right quaternion
		 * @return A new Quaternion containing the component-wise differences
		 */
		Quaternion operator-(const Quaternion& rhs);

		/**
		 * @brief Perform Quaternion-Quaternion multiplication
		 * @param rhs The right quaternion
		 * @return A new resulting Quaternion
		 */
		Quaternion operator*(const Quaternion& rhs);

		/**
		 * @brief Add the components of the two quaternions together
		 * @param rhs The right quaternion
		 * @return The same Quaternion containing the component-wise sums
		 */
		Quaternion& operator+=(const Quaternion& rhs);

		/**
		 * @brief Subtract the components of this by the components of rhs
		 * @param rhs The right quaternion
		 * @return The same Quaternion containing the component-wise differences
		 */
		Quaternion& operator-=(const Quaternion& rhs);

		/**
		 * @brief Perform Quaternion-Quaternion multiplication
		 * @param rhs The right quaternion
		 * @return The same Quaternion containing the result
		 */
		Quaternion& operator*=(const Quaternion& rhs);

		/* ********** Quaternion/Vector3 Operations ********** */

		/**
		 * @brief Perform Quaternion-Vector3 multiplication
		 * @param v The vector3
		 * @return The resulting Vector3
		 */
		Vector3 operator*(const Vector3& v) const;

		/* ********** Quaternion Operations ********** */

		/**
		 * @brief Compute the norm of this Quaternion
		 * @return The norm
		 */
		float Norm() const;

		/**
		 * @brief Convert to a Quaternion of Unit size
		 * @return the same Quaternion (this)
		 */
		Quaternion& Unit();

		/**
		 * @brief Convert to the conjugate Quaternion
		 * @return the same Quaternion (this)
		 */
		Quaternion& Conjugate();

		/**
		 * @brief Convert to the inverse Quaternion
		 * @return the same Quaternion (this)
		 */
		Quaternion& Inverse();

		/**
		 * @brief Convert to the negation
		 * @return the same Quaternion (this)
		 */
		Quaternion& Negate();

		/**
		 * @brief Create the Unit Quaternion of this
		 * @return a copy of this of Unit size
		 */
		Quaternion UnitCopy() const;

		/**
		 * @brief Create the Conjugate Quaternion of this
		 * @return a copy the conjugate of this
		 */
		Quaternion ConjugateCopy() const;

		/**
		 * @brief Create the Inverse Quaternion of this
		 * @return a copy of the inverse of this
		 */
		Quaternion InverseCopy() const;

		/**
		 * @brief Get the Euler representation of this
		 * @return A Vector3 containing the Euler angles
		 */
		Vector3 GetEulerAngles() const;

		/**
		 * @brief Get the x, y, z components of this
		 * @return A Vector3 containing only the x, y, z components
		 */
		Vector3 XYZ() const;

		/**
		 * @brief Get the Rotation Matrix3 representation of this
		 * @return A Matrix3 rotation
		 */
		Matrix3 GetRotationMatrix() const;


		/* ********** Static Operations ********** */

		/**
		 * @brief Compute the dot product between lhs and rhs
		 * @param lhs The left Quaternion
		 * @param rhs The right Quaternion
		 * @return The dot product of the two Quaternions
		 */
		static float Dot(const Quaternion& lhs, const Quaternion& rhs);

		/**
		 * @brief Get a Quaternion representing a rotation of angle degrees around an axis
		 * @param axis The axis
		 * @param angle The angle in radians
		 * @return A new Quaternion representing the rotation
		 */
		static Quaternion FromAngleAxis(const Vector3& axis, Radian angle);

		/**
		 * @brief Get a Quaternion representing a rotation of angle degrees around an axis
		 * @param axis The axis
		 * @param angle The angle in degrees
		 * @return A new Quaternion representing the rotation
		 */
		static Quaternion FromAngleAxis(const Vector3& axis, Degree angle);

		/**
		 * @brief Get a Quaternion representing a rotation in the 3 different axes
		 * @param xAxis The x-axis vector
		 * @param yAxis The y-axis vector
		 * @param zAxis The z-axis vector
		 * @return A new Quaternion representing the rotation
		 */
		static Quaternion FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

		/**
		 * @brief Get a Quaternion rotating in the x, y, z axes by their given angles
		 * @param x The angle to rotate in the x axis
		 * @param y The angle to rotate in the y axis
		 * @param z The angle to rotate in the z axis
		 * @return A new Quaternion representing the rotation
		 */
		static Quaternion FromEuler(Radian x, Radian y, Radian z);

		/**
		 * @brief Get a Quaternion representing a matrix3 rotation
		 * @param matrix the matrix3 rotation
		 * @return A new Quaternion representing the rotation
		 */
		static Quaternion FromMatrix3(const Matrix3& matrix);

		/**
		 * @brief Get a Quaternion with the given direction and up vector
		 * @param direction The direction to rotate towards
		 * @param up The up vector
		 * @return A new Quaternion representing the rotation
		 */
		static Quaternion LookRotation(const Vector3& direction, const Vector3& up = Vector3::Up());

		/**
		 * @brief Get a Quaternion representing a rotation from `from` to `to`
		 * @param from The source to rotate from
		 * @param to The direction to rotate towards
		 * @return A new Quaternion representing the rotation
		 */
		static Quaternion FromToRotation(const Vector3& from, const Vector3& to);

		/* ********** Useful Quaternions ********** */

		/**
		 * @brief Create the identity quaternion
		 * @return The Identity Quaternion
		 */
		static Quaternion Identity();

		/**
		 * @brief Create a zero quaternion (all components are 0)
		 * @return The Zero Quaternion
		 */
		static Quaternion Zero();

		/**
		 * @brief The w component
		 */
		float w_;

		/**
		 * @brief The x component
		 */
		float x_;

		/**
		 * @brief The y component
		 */
		float y_;

		/**
		 * @brief The z component
		 */
		float z_;

	}; // class Quaternion

} // namespace math
} // namespace zero