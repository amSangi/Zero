#pragma once

#include <string>
#include "render/MeshData.hpp"
#include "component/Transform.hpp"
#include "component/Material.hpp"
#include "component/Volume.hpp"

namespace zero::render
{
    struct GeometryData
    {
        GeometryData(std::unique_ptr<MeshData> mesh_data, std::unique_ptr<Material> material, const Volume& volume);
        std::unique_ptr<MeshData> mesh_data_;
        std::unique_ptr<Material> material_;
        Volume volume_;
        uint32 geometry_id_;
    };

    struct Node
    {
        Node(const Volume& volume, const math::Matrix4x4& transform);
        Volume volume_;
        math::Matrix4x4 transform_;
        std::vector<uint32> geometry_indices_;
        std::vector<std::shared_ptr<Node>> child_nodes_;
    };

    struct Model
    {
        Model(std::string model_name, std::shared_ptr<Node> root_node);
        std::string model_name_;
        std::shared_ptr<Node> root_node_;
        std::vector<GeometryData> geometries_;
    };
} // namespace zero::render