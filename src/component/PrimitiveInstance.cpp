#include "component/PrimitiveInstance.hpp"

namespace zero
{

PrimitiveInstance::Primitive::Primitive(const Box& box)
: box_(box)
{
}

PrimitiveInstance::Primitive::Primitive(const Sphere& sphere)
: sphere_(sphere)
{
}

PrimitiveInstance::Primitive::Primitive(const Plane& plane)
: plane_(plane)
{
}

PrimitiveInstance::Primitive::Primitive(const Cone& cone)
: cone_(cone)
{
}

PrimitiveInstance::Primitive::Primitive(const Cylinder& cylinder)
: cylinder_(cylinder)
{
}

PrimitiveInstance::Primitive::Primitive(const Torus& torus)
: torus_(torus)
{
}

PrimitiveInstance::PrimitiveInstance()
: PrimitiveInstance(Box())
{
}

PrimitiveInstance::PrimitiveInstance(const Box& box)
: Component()
, type_(Type::BOX)
, primitive_(box)
{
}

PrimitiveInstance::PrimitiveInstance(const Sphere& sphere)
: Component()
, type_(Type::SPHERE)
, primitive_(sphere)
{
}

PrimitiveInstance::PrimitiveInstance(const Plane& plane)
: Component()
, type_(Type::PLANE)
, primitive_(plane)
{
}

PrimitiveInstance::PrimitiveInstance(const Cone& cone)
: Component()
, type_(Type::CONE)
, primitive_(cone)
{
}

PrimitiveInstance::PrimitiveInstance(const Cylinder& cylinder)
: Component()
, type_(Type::CYLINDER)
, primitive_(cylinder)
{
}

PrimitiveInstance::PrimitiveInstance(const Torus& torus)
: Component()
, type_(Type::TORUS)
, primitive_(torus)
{
}

PrimitiveInstance::Type PrimitiveInstance::GetType() const
{
    return type_;
}

Box PrimitiveInstance::GetBox() const
{
    if (type_ != Type::BOX)
    {
        return {};
    }
    return primitive_.box_;
}

Sphere PrimitiveInstance::GetSphere() const
{
    if (type_ != Type::SPHERE)
    {
        return {};
    }
    return primitive_.sphere_;
}

Plane PrimitiveInstance::GetPlane() const
{
    if (type_ != Type::PLANE)
    {
        return {};
    }
    return primitive_.plane_;
}

Cone PrimitiveInstance::GetCone() const
{
    if (type_ != Type::CONE)
    {
        return {};
    }
    return primitive_.cone_;
}

Cylinder PrimitiveInstance::GetCylinder() const
{
    if (type_ != Type::CYLINDER)
    {
        return {};
    }
    return primitive_.cylinder_;
}

Torus PrimitiveInstance::GetTorus() const
{
    if (type_ != Type::TORUS)
    {
        return {};
    }
    return primitive_.torus_;
}

} // namespace zero