#include "render/MeshGenerator.hpp"

using namespace zero::render;

void MeshGenerator::Generate(const math::Sphere& sphere,
                             std::vector<Vertex>& vertices,
                             std::vector<uint32>& indices) {

    std::vector<Vertex> temp_vertices;

    uint32 stack_count = 18;
    uint32 sector_count = 36;

    // Generate vertices
    float stack_step = math::kPi / stack_count;
    float sector_step = math::kTwoPi / sector_count;

    for (uint32 i = 0; i <= stack_count; ++i) {
        // starting from pi/2 to -pi/2
        float stack_angle = math::kHalfPi - i * stack_step;
        float xz = math::Cos(stack_angle);       // r * cos(u)
        float y = math::Sin(stack_angle);        // r * sin(u)

        for (uint32 j = 0; j <= sector_count; ++j) {
            // starting from 0 to 2pi
            float sector_angle = static_cast<float>(j) * sector_step;

            Vertex vertex{};
            vertex.normal_ = math::Vec3f(xz * math::Sin(sector_angle),
                                         y,
                                         xz * math::Cos(sector_angle));
            vertex.position_ = (vertex.normal_ * sphere.radius_) + sphere.center_;
            vertex.texture_coordinate_ = math::Vec2f(static_cast<float>(j) / sector_count,
                                                     static_cast<float>(i) / stack_count);
            temp_vertices.push_back(vertex);
        }
    }

    // Generate indices
    uint32 index = 0;
    for (uint32 i = 0; i < stack_count; ++i) {

        uint32 vi1 = i * (sector_count + 1);
        uint32 vi2 = (i + 1) * (sector_count + 1);

        for (uint32 j = 0; j < sector_count; ++j, ++vi1, ++vi2) {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            auto& v1 = temp_vertices[vi1];
            auto& v2 = temp_vertices[vi2];
            auto& v3 = temp_vertices[vi1 + 1];
            auto& v4 = temp_vertices[vi2 + 1];

            if (i == 0) {
                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v4);

                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(index + 2);

                index += 3;
            }
            else if (i == (stack_count - 1)) {
                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v3);

                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(index + 2);

                index += 3;
            }
            else {
                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v3);
                vertices.push_back(v4);

                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(index + 2);

                indices.push_back(index + 2);
                indices.push_back(index + 1);
                indices.push_back(index + 3);

                index += 4;
            }
        }
    }
}

void MeshGenerator::Generate(const math::Box& box,
                             std::vector<Vertex>& vertices,
                             std::vector<uint32>& indices) {
    // TODO: Finish Implementation
}