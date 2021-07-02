#pragma once

#include <memory>
#include <string>
#include <vector>
#include "component/Volume.hpp"
#include "math/Matrix4x4.hpp"
#include "render/EntityPrototype.hpp"

namespace zero::render
{

    class Node
    {
    public:
        Node(const std::string& name, const math::Matrix4x4& transformation);

        const std::string& GetName() const;

        void SetParent(std::shared_ptr<Node> parent);
        std::shared_ptr<Node> GetParent() const;

        void AddChild(std::shared_ptr<Node> node);
        const std::vector<std::shared_ptr<Node>>& GetChildren() const;

        void AddEntityPrototype(std::unique_ptr<EntityPrototype> entity_prototype);
        EntityPrototype* GetEntityPrototype(uint32 prototype_index) const;
        uint32 GetEntityPrototypeCount() const;

        const math::Matrix4x4& GetTransformation() const;
        const Volume& GetVolume() const;

        void SetBone(std::shared_ptr<Bone> bone);
        std::shared_ptr<Bone> GetBone();

    private:
        std::string name_;
        std::shared_ptr<Node> parent_;
        std::vector<std::shared_ptr<Node>> children_;
        std::shared_ptr<Bone> bone_;
        std::vector<std::unique_ptr<EntityPrototype>> entity_prototypes_;
        math::Matrix4x4 transformation_;
        Volume volume_;
    };

} // namespace zero::render