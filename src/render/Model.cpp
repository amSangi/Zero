#include "render/Model.hpp"
#include <queue>

namespace zero::render
{

Model::Model(const std::string& name, std::shared_ptr<Node> root_node)
: model_name_(name)
, root_node_(std::move(root_node))
, nodes_()
, animator_(nullptr)
{
}

void Model::Initialize(std::unique_ptr<Animator> animator)
{
    animator_ = std::move(animator);

    std::queue<std::shared_ptr<Node>> node_queue{};
    node_queue.push(root_node_);

    while (!node_queue.empty())
    {
        std::shared_ptr<Node> node = node_queue.front();
        node_queue.pop();

        auto search = nodes_.find(node->GetName());
        if (search != nodes_.end())
        {
            // Node has already been visited
            continue;
        }

        nodes_.emplace(node->GetName(), node);
        for (std::shared_ptr<Node> child_node : node->GetChildren())
        {
            node_queue.push(child_node);
        }
    }
}

const std::string& Model::GetName() const
{
    return model_name_;
}

std::shared_ptr<Node> Model::GetRootNode() const
{
    return root_node_;
}

std::shared_ptr<Node> Model::GetNode(const std::string& node_name) const
{
    auto node_search = nodes_.find(node_name);
    if (node_search == nodes_.end())
    {
        return nullptr;
    }
    return node_search->second;
}

Animator* Model::GetAnimator()
{
    return animator_.get();
}


} // namespace zero::render

