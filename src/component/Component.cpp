#include "component/Component.hpp"

namespace zero
{

HierarchyComponent::HierarchyComponent()
: Component()
, parent_(NullEntity)
, children_()
, state_(State::IDLE)
, keep_children_alive_(false)
{
}

HierarchyComponent::HierarchyComponent(Entity parent)
: Component()
, parent_(parent)
, children_()
, state_(State::IDLE)
, keep_children_alive_(false)
{
}

} // namespace zero

