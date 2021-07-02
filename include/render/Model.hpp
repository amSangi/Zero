#pragma once

#include "render/Node.hpp"

namespace zero::render
{

    class Model
    {
    public:
        Model(const std::string& name, std::shared_ptr<Node> root_node);
        ~Model() = default;

        void Initialize(std::unique_ptr<Animator> animator);
        const std::string& GetName() const;
        std::shared_ptr<Node> GetRootNode() const;
        std::shared_ptr<Node> GetNode(const std::string& node_name) const;
        Animator* GetAnimator();
    private:
        std::string model_name_;
        std::shared_ptr<Node> root_node_;
        std::unordered_map<std::string, std::shared_ptr<Node>> nodes_;
        std::unique_ptr<Animator> animator_;
    }; // class Model

} // namespace zero::render