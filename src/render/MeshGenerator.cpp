#include "render/MeshGenerator.hpp"

using namespace zero::render;

Mesh MeshGenerator::GenerateSphere(zero::uint32 latitude_count,
                                   zero::uint32 longitude_count) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    std::vector<Vertex> temp_vertices;

    // Generate vertices
    float latitude_step = math::kPi / latitude_count;
    float longitude_step = math::kTwoPi / longitude_count;

    for (uint32 i = 0; i <= latitude_count; ++i) {
        // starting from pi/2 to -pi/2
        float latitude_angle = math::kHalfPi - i * latitude_step;
        float xz = math::Cos(latitude_angle);       // r * cos(u)
        float y = math::Sin(latitude_angle);        // r * sin(u)

        for (uint32 j = 0; j <= longitude_count; ++j) {
            // starting from 0 to 2pi
            float longitude_angle = static_cast<float>(j) * longitude_step;

            Vertex vertex{};
            vertex.normal_ = math::Vec3f(xz * math::Sin(longitude_angle),
                                         y,
                                         xz * math::Cos(longitude_angle));
            vertex.position_ = vertex.normal_;
            vertex.texture_coordinate_ = math::Vec2f(static_cast<float>(j) / longitude_count,
                                                     static_cast<float>(i) / latitude_count);
            temp_vertices.push_back(vertex);
        }
    }

    // Generate indices
    uint32 index = 0;
    for (uint32 i = 0; i < latitude_count; ++i) {

        uint32 vi1 = i * (longitude_count + 1);
        uint32 vi2 = (i + 1) * (longitude_count + 1);

        for (uint32 j = 0; j < longitude_count; ++j, ++vi1, ++vi2) {
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
            else if (i == (latitude_count - 1)) {
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

    return Mesh(std::move(vertices), std::move(indices));
}

Mesh MeshGenerator::GeneratePlane(const math::Vec3f& u_axis,
                                  const math::Vec3f& v_axis,
                                  uint32 row_count,
                                  uint32 column_count) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    vertices.reserve((row_count + 1) * (column_count + 1));

    // Generate vertices
    float row_increment = 1.0F;
    float column_increment = 1.0F;
    auto normal = math::Vec3f::Normalize(math::Vec3f::Cross(v_axis, u_axis));
    for (uint32 i = 0; i <= row_count; ++i) {
        for (uint32 j = 0; j <= column_count; ++j) {
            float u = i * row_increment;
            float v = j * column_increment;
            Vertex vertex{};
            vertex.position_ = ((u - 0.5F) * u_axis) + ((v - 0.5F) * v_axis);
            vertex.normal_ = normal;
            vertex.texture_coordinate_ = math::Vec2f(u, v);
            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (uint32 i = 0; i < row_count; ++i) {
        for (uint32 j = 0; j < column_increment; ++j) {
            const uint32 index = i * (row_count + 1) + j;

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 1 + column_count + 1);

            indices.push_back(index + column_count + 1);
            indices.push_back(index + 1);
            indices.push_back(index + column_count + 2);
        }
    }

    return Mesh(std::move(vertices), std::move(indices));
}

Mesh MeshGenerator::GenerateBox(const math::Box& box) {
    // TODO: Finish Implementation
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    return Mesh(std::move(vertices), std::move(indices));
}

Mesh MeshGenerator::GenerateTorus() {
    // TODO: Finish Implementation
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    return Mesh(std::move(vertices), std::move(indices));
}

Mesh MeshGenerator::GenerateCone() {
    // TODO: Finish Implementation
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    return Mesh(std::move(vertices), std::move(indices));
}

Mesh MeshGenerator::GenerateCylinder() {
    // TODO: Finish Implementation
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    return Mesh(std::move(vertices), std::move(indices));
}