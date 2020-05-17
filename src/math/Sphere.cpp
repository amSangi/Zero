#include "math/Sphere.hpp"
#include "math/Box.hpp"

namespace zero::math
{

/**
 * The radius of a sphere is increased by an additional epsilon value when
 * converting from an Axis-Aligned Bounding Box (AABB) to make up for floating point precision errors.
 * This will ensure the resulting sphere contains the original AABB.
 */
constexpr float kAABBConversionEpsilon = 1.0F;

/**
 * The radius of a merged sphere is increased by an additional epsilon value when
 * merging two spheres to make up for floating point precision errors. This will ensure the
 * merged sphere contains both spheres when performing multiple merges.
 */
constexpr float kMergeExpansionEpsilon = 1.0F;

Sphere::Sphere()
: Sphere(1.0F)
{
}

Sphere::Sphere(float r)
: center_(Vec3f::Zero())
, radius_(r)
{
}

Sphere::Sphere(const Vec3f& c)
: center_(c)
, radius_(0.0F)
{
}

Sphere::Sphere(const Vec3f& c, float r)
: center_(c)
, radius_(r)
{
}

Sphere::Sphere(const Vec3f& min, const Vec3f& max)
: center_((min + max) * 0.5F)
, radius_(((max - min).Magnitude() * 0.5F) + kAABBConversionEpsilon)
{
}

bool Sphere::operator==(const Sphere& other) const
{
	return (center_ == other.center_) && Equal(radius_, other.radius_);
}

bool Sphere::operator!=(const Sphere& other) const
{
	return !operator==(other);
}

bool Sphere::Contains(const Box& box) const
{
	return Contains(box.min_) && Contains(box.max_);
}

bool Sphere::Contains(const Sphere& other) const
{
	if (other.radius_ > radius_)
	{
		return false;
	}

    auto radius_difference = radius_ - other.radius_;
    auto square_radius_difference = radius_difference * radius_difference;
	auto square_distance = Vec3f::SquareDistance(center_, other.center_);
	return (square_distance - square_radius_difference) <= kEpsilon;
}

bool Sphere::Contains(const Vec3f& point) const
{
	return Vec3f::SquareDistance(center_, point) <= (radius_ * radius_);
}

bool Sphere::Intersects(const Sphere& other) const
{
    auto max = Max(0.0F, radius_ + other.radius_);
	return Vec3f::SquareDistance(center_, other.center_) <= (max * max);
}

void Sphere::Merge(const Sphere& other)
{
	if (radius_ <= kEpsilon || other.Contains(*this))
	{
		*this = other;
	}
	else if (Contains(other))
	{
		return;
	}
	else
    {
	    // Direction toward calling sphere's center
		Vec3f direction = center_ - other.center_;
		float distance = direction.Magnitude();
		// Normalize direction
		if (distance > kSmallEpsilon)
        {
		    direction /= distance;
        }

		// Half of diameter of enclosing circle
		float merged_radius = (distance + radius_ + other.radius_) * 0.5F;

		// Compute sphere end points
		Vec3f sphere_end_point = center_ + (radius_ * direction);
		Vec3f other_end_point = other.center_ - (other.radius_ * direction);

		// Linearly interpolate half-way between the end points
        center_ = (sphere_end_point + other_end_point) * 0.5F;
        // Increase actual radius by kMergeExpansionEpsilon to guard against floating point precision loss
		radius_ = merged_radius + kMergeExpansionEpsilon;
	}
}

Sphere Sphere::Merge(const Sphere& lhs, const Sphere& rhs)
{
	Sphere lhs_copy = lhs;
	lhs_copy.Merge(rhs);
	return lhs_copy;
}

Sphere Sphere::Zero()
{
	return Sphere(Vec3f::Zero(), 0.0F);
}

} // namespace zero::math