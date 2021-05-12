#include "render/Node.hpp"

namespace zero::render
{

Node::Node(const std::string& name, const math::Matrix4x4& transformation)
: name_(name)
, parent_(nullptr)
, children_()
, entity_prototypes_()
, transformation_(transformation)
, volume_()
{
}

const std::string& Node::GetName() const
{
    return name_;
}

void Node::SetParent(std::shared_ptr<Node> parent)
{
    parent_ = std::move(parent);
}

std::shared_ptr<Node> Node::GetParent() const
{
    return parent_;
}

void Node::AddChild(std::shared_ptr<Node> node)
{
    children_.push_back(std::move(node));
    volume_.Engulf(node->GetVolume());
}

const std::vector<std::shared_ptr<Node>>& Node::GetChildren() const
{
    return children_;
}

void Node::AddEntityPrototype(std::unique_ptr<EntityPrototype> entity_prototype)
{
    volume_.Engulf(entity_prototype->GetVolume());
    entity_prototypes_.push_back(std::move(entity_prototype));
}

EntityPrototype* Node::GetEntityPrototype(uint32 prototype_index) const
{
    if (prototype_index >= entity_prototypes_.size())
    {
        return entity_prototypes_.back().get();
    }

    return entity_prototypes_[prototype_index].get();
}

uint32 Node::GetEntityPrototypeCount() const
{
    return entity_prototypes_.size();
}

const math::Matrix4x4& Node::GetTransformation() const
{
    return transformation_;
}

const Volume& Node::GetVolume() const
{
    return volume_;
}


} // namespace zero::render