#pragma once

#include "render/IMesh.hpp"
#include "render/Model.hpp"

namespace zero::render
{

    class IMeshLoader
    {
    public:
        virtual ~IMeshLoader() = default;
        virtual std::unique_ptr<IMesh> LoadMesh(std::unique_ptr<Mesh> mesh_data) = 0;
    };

} // namespace zero::render