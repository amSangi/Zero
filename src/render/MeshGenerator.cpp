#include "render/MeshGenerator.hpp"
#include <array>

using namespace zero::render;

Mesh MeshGenerator::GenerateSphere(const Sphere& sphere) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    std::vector<Vertex> temp_vertices;

    const uint32 latitude_count = sphere.latitude_count_ < 16 ? 16 : sphere.latitude_count_;
    const uint32 longitude_count = sphere.longitude_count_ < 16 ? 16 : sphere.longitude_count_;
    float latitude_step = math::kPi / latitude_count;
    float longitude_step = math::kTwoPi / longitude_count;

    // Generate vertices
    for (uint32 i = 0; i <= latitude_count; ++i) {
        // starting from pi/2 to -pi/2
        float latitude_angle = math::kHalfPi - i * latitude_step;
        float xz = math::Cos(latitude_angle);       // r * cos(u)
        float y = math::Sin(latitude_angle);        // r * sin(u)

        for (uint32 j = 0; j <= longitude_count; ++j) {
            // starting from 0 to 2pi
            float longitude_angle = static_cast<float>(j) * longitude_step;

            Vertex vertex{};
            vertex.position_ = math::Vec3f(xz * math::Sin(longitude_angle),
                                           y,
                                           xz * math::Cos(longitude_angle));
            vertex.normal_ = vertex.position_;
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

Mesh MeshGenerator::GeneratePlane(const Plane& plane) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    vertices.reserve((plane.width_ + 1) * (plane.height_ + 1));
    indices.resize(plane.width_ * plane.height_ * 6);

    const auto u_axis = plane.u_axis_ == math::Vec3f::Zero() ? math::Vec3f::Right() : plane.u_axis_;
    const auto v_axis = plane.v_axis_ == math::Vec3f::Zero() ? math::Vec3f::Forward() : plane.v_axis_;

    // Generate vertices
    auto normal = math::Vec3f::Normalize(math::Vec3f::Cross(v_axis, u_axis));
    for (uint32 i = 0; i <= plane.width_; ++i) {
        auto u_component = u_axis * i;
        float u_texture_coordinate = static_cast<float>(i) / plane.width_;
        for (uint32 j = 0; j <= plane.height_; ++j) {
            Vertex vertex{};
            vertex.position_ = u_component + (v_axis * j);
            vertex.normal_ = normal;
            vertex.texture_coordinate_ = math::Vec2f(u_texture_coordinate, static_cast<float>(j) / plane.height_);
            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (uint32 ti = 0, vi = 0, i = 0; i < plane.width_; i++, vi++) {
        for (uint32 j = 0; j < plane.height_; j++, ti += 6, vi++) {
            indices[ti] = vi;
            indices[ti + 3] = indices[ti + 2] = vi + 1;
            indices[ti + 4] = indices[ti + 1] = vi + plane.height_ + 1;
            indices[ti + 5] = vi + plane.height_ + 2;
        }
    }

    return Mesh(std::move(vertices), std::move(indices));
}

Mesh MeshGenerator::GenerateBox() {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    vertices.reserve(24);
    indices.reserve(36);

    // Generate vertices
    // v3 --- v2      ^ -z
    // |   -y  |      |
    // v0 --- v1      |-----> +x
    Vertex v0{};
    v0.position_ = math::Vec3f(0.0F, 0.0F, 1.0F);
    Vertex v1{};
    v1.position_ = math::Vec3f(1.0F, 0.0F, 1.0F);
    Vertex v2{};
    v2.position_ = math::Vec3f(1.0F, 0.0F, 0.0F);
    Vertex v3{};
    v3.position_ = math::Vec3f(0.0F, 0.0F, 0.0F);
    // v7 --- v6      ^ -z
    // |   +y  |      |
    // v4 --- v5      |-----> +x
    Vertex v4{};
    v4.position_ = math::Vec3f(0.0F, 1.0F, 1.0F);
    Vertex v5{};
    v5.position_ = math::Vec3f(1.0F, 1.0F, 1.0F);
    Vertex v6{};
    v6.position_ = math::Vec3f(1.0F, 1.0F, 0.0F);
    Vertex v7{};
    v7.position_ = math::Vec3f(0.0F, 1.0F, 0.0F);

    // Bottom
    v0.texture_coordinate_ = math::Vec2f(0.0F, 0.0F);
    v1.texture_coordinate_ = math::Vec2f(1.0F, 0.0F);
    v2.texture_coordinate_ = math::Vec2f(1.0F, 1.0F);
    v3.texture_coordinate_ = math::Vec2f(0.0F, 1.0F);
    v0.normal_ = v1.normal_ = v2.normal_ = v3.normal_ = math::Vec3f::Down();
    vertices.push_back(v0); vertices.push_back(v1);
    vertices.push_back(v2); vertices.push_back(v3);

    // Left
    v0.texture_coordinate_ = math::Vec2f(1.0F, 0.0F);
    v4.texture_coordinate_ = math::Vec2f(1.0F, 1.0F);
    v7.texture_coordinate_ = math::Vec2f(0.0F, 1.0F);
    v3.texture_coordinate_ = math::Vec2f(0.0F, 0.0F);
    v0.normal_ = v4.normal_ = v7.normal_ = v3.normal_ = math::Vec3f::Left();
    vertices.push_back(v0); vertices.push_back(v4);
    vertices.push_back(v7); vertices.push_back(v3);

    // Forward
    v0.texture_coordinate_ = math::Vec2f(0.0F, 0.0F);
    v1.texture_coordinate_ = math::Vec2f(1.0F, 0.0F);
    v5.texture_coordinate_ = math::Vec2f(1.0F, 1.0F);
    v4.texture_coordinate_ = math::Vec2f(0.0F, 1.0F);
    v0.normal_ = v1.normal_ = v5.normal_ = v4.normal_ = math::Vec3f::Forward();
    vertices.push_back(v0); vertices.push_back(v1);
    vertices.push_back(v5); vertices.push_back(v4);

    // Back
    v3.texture_coordinate_ = math::Vec2f(1.0F, 0.0F);
    v2.texture_coordinate_ = math::Vec2f(0.0F, 0.0F);
    v6.texture_coordinate_ = math::Vec2f(0.0F, 1.0F);
    v7.texture_coordinate_ = math::Vec2f(1.0F, 1.0F);
    v3.normal_ = v2.normal_ = v6.normal_ = v7.normal_ = math::Vec3f::Back();
    vertices.push_back(v3); vertices.push_back(v2);
    vertices.push_back(v6); vertices.push_back(v7);

    // Right
    v1.texture_coordinate_ = math::Vec2f(0.0F, 0.0F);
    v2.texture_coordinate_ = math::Vec2f(1.0F, 0.0F);
    v6.texture_coordinate_ = math::Vec2f(1.0F, 1.0F);
    v5.texture_coordinate_ = math::Vec2f(0.0F, 1.0F);
    v1.normal_ = v2.normal_ = v6.normal_ = v5.normal_ = math::Vec3f::Right();
    vertices.push_back(v1); vertices.push_back(v2);
    vertices.push_back(v6); vertices.push_back(v5);

    // Top
    v4.texture_coordinate_ = math::Vec2f(0.0F, 0.0F);
    v5.texture_coordinate_ = math::Vec2f(1.0F, 0.0F);
    v6.texture_coordinate_ = math::Vec2f(1.0F, 1.0F);
    v7.texture_coordinate_ = math::Vec2f(0.0F, 1.0F);
    v4.normal_ = v5.normal_ = v6.normal_ = v7.normal_ = math::Vec3f::Up();
    vertices.push_back(v4); vertices.push_back(v5);
    vertices.push_back(v6); vertices.push_back(v7);

    // Generate indices
    for (uint32 i = 0; i < 6; ++i) {
        indices.push_back(3 + 4 * i); indices.push_back(1 + 4 * i); indices.push_back(0 + 4 * i);
        indices.push_back(3 + 4 * i); indices.push_back(2 + 4 * i); indices.push_back(1 + 4 * i);
    }

    return Mesh(std::move(vertices), std::move(indices));
}

Mesh MeshGenerator::GenerateTorus(const Torus& torus) {
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    uint32 radial_segments = torus.radial_segments_ < 16 ? 16 : torus.radial_segments_;
    uint32 tubular_segments = torus.tubular_segments_ < 16 ? 16 : torus.tubular_segments_;
    vertices.reserve((radial_segments + 1) * (tubular_segments + 1));
    indices.reserve((radial_segments + 1) * (tubular_segments + 1) * 6);

    // Generate vertices
    for (uint32 i = 0; i <= radial_segments; ++i) {
        float v = static_cast<float>(i) / radial_segments;
        float radial_segment = v * math::kTwoPi;
        float cos_radial_segment = math::Cos(radial_segment);
        float sin_radial_segment = math::Sin(radial_segment);

        for (uint32 j = 0; j <= tubular_segments; ++j) {
            float u = static_cast<float>(j) / tubular_segments;
            float tubular_segment = u * math::kTwoPi;
            float cos_tubular_segment = math::Cos(tubular_segment);
            float sin_tubular_segment = math::Sin(tubular_segment);
            Vertex vertex{};
            vertex.position_ = math::Vec3f((torus.radius_ + torus.tube_radius_ * cos_radial_segment) * cos_tubular_segment,
                                           (torus.radius_ + torus.tube_radius_ * cos_radial_segment) * sin_tubular_segment,
                                            torus.radius_ * sin_radial_segment);
            auto center = math::Vec3f(torus.radius_ * cos_tubular_segment, torus.radius_ * sin_tubular_segment, 0.0F);
            vertex.normal_ = math::Vec3f::Normalize(vertex.position_ - center);
            vertex.texture_coordinate_ = math::Vec2f(u, v);
            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (uint32 i = 1; i <= radial_segments; ++i) {
        for (uint32 j = 1; j <= tubular_segments; ++j) {
            uint32 a = (tubular_segments + 1) * i + j - 1;
            uint32 b = (tubular_segments + 1) * (i - 1) + j - 1;
            uint32 c = (tubular_segments + 1) * (i - 1) + j;
            uint32 d = (tubular_segments + 1) * i + j;

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }
    
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