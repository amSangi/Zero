#include "render/renderer/MeshGenerator.hpp"

namespace zero::render
{

std::unique_ptr<Mesh> MeshGenerator::GenerateSphere(const Sphere& sphere)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    std::vector<Vertex> temp_vertices;

    const uint32 latitude_count = sphere.latitude_count_ < 16 ? 16 : sphere.latitude_count_;
    const uint32 longitude_count = sphere.longitude_count_ < 16 ? 16 : sphere.longitude_count_;
    float latitude_step = math::kPi / latitude_count;
    float longitude_step = math::kTwoPi / longitude_count;

    // Generate vertices
    for (uint32 i = 0; i <= latitude_count; ++i)
    {
        // starting from pi/2 to -pi/2
        float latitude_angle = math::kHalfPi - i * latitude_step;
        float xz = math::Cos(latitude_angle);       // r * cos(u)
        float y = math::Sin(latitude_angle);        // r * sin(u)

        for (uint32 j = 0; j <= longitude_count; ++j)
        {
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
    for (uint32 i = 0; i < latitude_count; ++i)
    {

        uint32 vi1 = i * (longitude_count + 1);
        uint32 vi2 = (i + 1) * (longitude_count + 1);

        for (uint32 j = 0; j < longitude_count; ++j, ++vi1, ++vi2)
        {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            auto& v1 = temp_vertices[vi1];
            auto& v2 = temp_vertices[vi2];
            auto& v3 = temp_vertices[vi1 + 1];
            auto& v4 = temp_vertices[vi2 + 1];

            if (i == 0)
            {
                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v4);

                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(index + 2);

                index += 3;
            }
            else if (i == (latitude_count - 1))
            {
                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v3);

                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(index + 2);

                index += 3;
            }
            else
            {
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

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

std::unique_ptr<Mesh> MeshGenerator::GeneratePlane(const Plane& plane)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    vertices.reserve((plane.width_ + 1) * (plane.height_ + 1));
    indices.resize(plane.width_ * plane.height_ * 6);

    const auto u_axis = plane.u_axis_ == math::Vec3f::Zero() ? math::Vec3f::Right() : plane.u_axis_;
    const auto v_axis = plane.v_axis_ == math::Vec3f::Zero() ? math::Vec3f::Forward() : plane.v_axis_;

    // Generate vertices
    auto normal = math::Vec3f::Normalize(math::Vec3f::Cross(v_axis, u_axis));
    for (uint32 i = 0; i <= plane.width_; ++i)
    {
        auto u_component = u_axis * i;
        float u_texture_coordinate = static_cast<float>(i) / plane.width_;
        for (uint32 j = 0; j <= plane.height_; ++j)
        {
            Vertex vertex{};
            vertex.position_ = u_component + (v_axis * j);
            vertex.normal_ = normal;
            vertex.texture_coordinate_ = math::Vec2f(u_texture_coordinate, static_cast<float>(j) / plane.height_);
            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (uint32 ti = 0, vi = 0, i = 0; i < plane.width_; i++, vi++)
    {
        for (uint32 j = 0; j < plane.height_; j++, ti += 6, vi++)
        {
            indices[ti] = vi;
            indices[ti + 3] = indices[ti + 2] = vi + 1;
            indices[ti + 4] = indices[ti + 1] = vi + plane.height_ + 1;
            indices[ti + 5] = vi + plane.height_ + 2;
        }
    }

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

std::unique_ptr<Mesh> MeshGenerator::GenerateBox()
{
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
    for (uint32 i = 0; i < 6; ++i)
    {
        indices.push_back(3 + 4 * i); indices.push_back(1 + 4 * i); indices.push_back(0 + 4 * i);
        indices.push_back(3 + 4 * i); indices.push_back(2 + 4 * i); indices.push_back(1 + 4 * i);
    }

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

std::unique_ptr<Mesh> MeshGenerator::GenerateTorus(const Torus& torus)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    float radius = math::Abs(torus.radius_);
    float tube_radius = math::Abs(torus.tube_radius_);
    uint32 radial_segments = torus.radial_segments_ < 16 ? 16 : torus.radial_segments_;
    uint32 tubular_segments = torus.tubular_segments_ < 16 ? 16 : torus.tubular_segments_;
    vertices.reserve((radial_segments + 1) * (tubular_segments + 1));
    indices.reserve((radial_segments + 1) * (tubular_segments + 1) * 6);

    // Generate vertices
    for (uint32 i = 0; i <= radial_segments; ++i)
    {
        float v = static_cast<float>(i) / radial_segments;
        float radial_segment = v * math::kTwoPi;
        float cos_radial_segment = math::Cos(radial_segment);
        float sin_radial_segment = math::Sin(radial_segment);

        for (uint32 j = 0; j <= tubular_segments; ++j)
        {
            float u = static_cast<float>(j) / tubular_segments;
            float tubular_segment = u * math::kTwoPi;
            float cos_tubular_segment = math::Cos(tubular_segment);
            float sin_tubular_segment = math::Sin(tubular_segment);
            Vertex vertex{};
            vertex.position_ = math::Vec3f((radius + tube_radius * cos_radial_segment) * cos_tubular_segment,
                                           (radius + tube_radius * cos_radial_segment) * sin_tubular_segment,
                                           tube_radius * sin_radial_segment);
            auto center = math::Vec3f(radius * cos_tubular_segment, radius * sin_tubular_segment, 0.0F);
            vertex.normal_ = math::Vec3f::Normalize(vertex.position_ - center);
            vertex.texture_coordinate_ = math::Vec2f(u, v);
            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (uint32 i = 1; i <= radial_segments; ++i)
    {
        for (uint32 j = 1; j <= tubular_segments; ++j)
        {
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

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

std::unique_ptr<Mesh> MeshGenerator::GenerateCone(const Cone& cone)
{
    Cylinder cylinder{};
    cylinder.top_radius_ = 0.0F;
    cylinder.bottom_radius_ = cone.radius_;
    cylinder.height_ = cone.height_;
    cylinder.radial_segments_ = cone.radial_segments_;
    cylinder.height_segments_ = cone.height_segments_;
    cylinder.is_open_ended_ = false;
    return MeshGenerator::GenerateCylinder(cylinder);
}

std::unique_ptr<Mesh> MeshGenerator::GenerateCylinder(const Cylinder& cylinder)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    // Generate Cylinder Body
    float top_radius = math::Abs(cylinder.top_radius_);
    float bottom_radius = math::Abs(cylinder.bottom_radius_);
    float theta_start = 0.0F;
    float theta_length = math::kTwoPi;
    float slope = (bottom_radius - top_radius) / cylinder.height_;
    float half_height = cylinder.height_ * 0.5F;
    uint32 index = 0;
    std::vector<std::vector<uint32>> index_matrix;

    // Vertices
    for (uint32 y = 0; y <= cylinder.height_segments_; ++y)
    {
        float v = static_cast<float>(y) / cylinder.height_segments_;
        float radius = v * (bottom_radius - top_radius) + top_radius;
        std::vector<uint32> index_row;
        for (uint32 x = 0; x <= cylinder.radial_segments_; ++x)
        {
            float u = static_cast<float>(x) / cylinder.radial_segments_;
            float theta = u * theta_length + theta_start;
            float sin_theta = math::Sin(theta);
            float cos_theta = math::Cos(theta);
            Vertex vertex{};
            vertex.position_ = math::Vec3f(radius * sin_theta,
                                           -v * cylinder.height_ + half_height,
                                           radius * cos_theta);
            vertex.normal_ = math::Vec3f::Normalize(math::Vec3f(sin_theta, slope, cos_theta));
            vertex.texture_coordinate_ = math::Vec2f(u, 1.0F - v);
            vertices.push_back(vertex);
            index_row.push_back(index++);
        }
        index_matrix.push_back(index_row);
    }

    // Indices
    for (uint32 x = 0; x < cylinder.radial_segments_; ++x)
    {
        for (uint32 y = 0; y < cylinder.height_segments_; ++y)
        {
            uint32 a = index_matrix[y][x];
            uint32 b = index_matrix[y + 1][x];
            uint32 c = index_matrix[y + 1][x + 1];
            uint32 d = index_matrix[y][x + 1];

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    // Generate and append cap meshes
    if (!cylinder.is_open_ended_)
    {
        if (cylinder.top_radius_ > 0.0F)
        {
            auto top_cap_mesh = GenerateCylinderCap(cylinder, index, true);
            const auto& top_cap_vertices = top_cap_mesh->vertices_;
            const auto& top_cap_indices = top_cap_mesh->indices_;
            vertices.insert(vertices.end(), top_cap_vertices.begin(), top_cap_vertices.end());
            indices.insert(indices.end(), top_cap_indices.begin(),top_cap_indices.end());
        }
        if (cylinder.bottom_radius_ > 0.0F)
        {
            auto bottom_cap_mesh = GenerateCylinderCap(cylinder, index, false);
            const auto& bottom_cap_vertices = bottom_cap_mesh->vertices_;
            const auto& bottom_cap_indices = bottom_cap_mesh->indices_;
            vertices.insert(vertices.end(), bottom_cap_vertices.begin(), bottom_cap_vertices.end());
            indices.insert(indices.end(), bottom_cap_indices.begin(),bottom_cap_indices.end());
        }
    }

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

std::unique_ptr<Mesh> MeshGenerator::GenerateCylinderCap(const Cylinder& cylinder, uint32& index, bool isTopCap)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    float half_height = cylinder.height_ * 0.5F;
    float radius = isTopCap ? cylinder.top_radius_ : cylinder.bottom_radius_;
    float sign = isTopCap ? 1.0F : -1.0F;
    uint32 start_center_index = index;
    // Generate center vertex data
    for (uint32 x = 1; x <= cylinder.radial_segments_; ++x)
    {
        Vertex vertex{};
        vertex.position_ = math::Vec3f(0.0F, half_height * sign, 0.0F);
        vertex.normal_ = math::Vec3f(0.0F, sign, 0.0F);
        vertex.texture_coordinate_ = math::Vec2f(0.5F, 0.5F);
        vertices.push_back(vertex);
        ++index;
    }

    uint32 end_center_index = index;

    // Generate surrounding vertices
    for (uint32 x = 0; x <= cylinder.radial_segments_; ++x)
    {
        float u = static_cast<float>(x) / cylinder.radial_segments_;
        float theta = u * math::kTwoPi;
        float cos_theta = math::Cos(theta);
        float sin_theta = math::Sin(theta);
        Vertex vertex{};
        vertex.position_ = math::Vec3f(radius * sin_theta,
                                       half_height * sign,
                                       radius * cos_theta);
        vertex.normal_ = math::Vec3f(0.0F, sign, 0.0F);
        vertex.texture_coordinate_ = math::Vec2f((cos_theta * 0.5F) + 0.5F,
                                                 (sin_theta * 0.5F * sign) + 0.5F);
        vertices.push_back(vertex);
        ++index;
    }

    // Generate indices
    for (uint32 x = 0; x < cylinder.radial_segments_; ++x)
    {
        uint32 c = start_center_index + x;
        uint32 i = end_center_index + x;
        if (isTopCap)
        {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(c);
        }
        else
        {
            indices.push_back(i + 1);
            indices.push_back(i);
            indices.push_back(c);
        }
    }

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

} // namespace zero::render