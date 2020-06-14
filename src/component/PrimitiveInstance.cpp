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
: type_(Type::BOX)
, primitive_(box)
{
}

PrimitiveInstance::PrimitiveInstance(const Sphere& sphere)
: type_(Type::SPHERE)
, primitive_(sphere)
{
}

PrimitiveInstance::PrimitiveInstance(const Plane& plane)
: type_(Type::PLANE)
, primitive_(plane)
{
}

PrimitiveInstance::PrimitiveInstance(const Cone& cone)
: type_(Type::CONE)
, primitive_(cone)
{
}

PrimitiveInstance::PrimitiveInstance(const Cylinder& cylinder)
: type_(Type::CYLINDER)
, primitive_(cylinder)
{
}

PrimitiveInstance::PrimitiveInstance(const Torus& torus)
: type_(Type::TORUS)
, primitive_(torus)
{
}

PrimitiveInstance::Type PrimitiveInstance::GetType() const
{
    return type_;
}

void PrimitiveInstance::Set(const Box& box)
{
    type_ = Type::BOX;
    primitive_.box_ = box;
}

void PrimitiveInstance::Set(const Sphere& sphere)
{
    type_ = Type::SPHERE;
    primitive_.sphere_ = sphere;
}

void PrimitiveInstance::Set(const Plane& plane)
{
    type_ = Type::PLANE;
    primitive_.plane_ = plane;
}

void PrimitiveInstance::Set(const Cone& cone)
{
    type_ = Type::CONE;
    primitive_.cone_ = cone;
}

void PrimitiveInstance::Set(const Cylinder& cylinder)
{
    type_ = Type::CYLINDER;
    primitive_.cylinder_ = cylinder;
}

void PrimitiveInstance::Set(const Torus& torus)
{
    type_ = Type::TORUS;
    primitive_.torus_ = torus;
}

Box PrimitiveInstance::GetBox() const
{
    if (type_ != Type::BOX)
    {
        return Box();
    }
    return primitive_.box_;
}

Sphere PrimitiveInstance::GetSphere() const
{
    if (type_ != Type::SPHERE)
    {
        return Sphere();
    }
    return primitive_.sphere_;
}

Plane PrimitiveInstance::GetPlane() const
{
    if (type_ != Type::PLANE)
    {
        return Plane();
    }
    return primitive_.plane_;
}

Cone PrimitiveInstance::GetCone() const
{
    if (type_ != Type::CONE)
    {
        return Cone();
    }
    return primitive_.cone_;
}

Cylinder PrimitiveInstance::GetCylinder() const
{
    if (type_ != Type::CYLINDER)
    {
        return Cylinder();
    }
    return primitive_.cylinder_;
}

Torus PrimitiveInstance::GetTorus() const
{
    if (type_ != Type::TORUS)
    {
        return Torus();
    }
    return primitive_.torus_;
}

} // namespace zero