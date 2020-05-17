#include "render/Shapes.hpp"
#include <gtest/gtest.h>

using namespace zero::render;

TEST(TestShapes, Box_Equality_Equal)
{
    EXPECT_EQ(Box(), Box());
}

TEST(TestShapes, Box_Equality_Different_Height)
{
    Box box{};
    box.height_ += 1;
    EXPECT_NE(box, Box());
}

TEST(TestShapes, Box_Equality_Different_Width)
{
    Box box{};
    box.width_ += 1;
    EXPECT_NE(box, Box());
}

TEST(TestShapes, Box_Equality_Different_Depth)
{
    Box box{};
    box.depth_ += 1;
    EXPECT_NE(box, Box());
}


TEST(TestShapes, Plane_Equality_Equal)
{
    EXPECT_EQ(Plane(), Plane());
}

TEST(TestShapes, Plane_Equality_Different_Width)
{
    Plane plane{};
    plane.width_ += 1;
    EXPECT_NE(plane, Plane());
}

TEST(TestShapes, Plane_Equality_Different_Height)
{
    Plane plane{};
    plane.height_ += 1;
    EXPECT_NE(plane, Plane());
}

TEST(TestShapes, Plane_Equality_Different_UAxis)
{
    Plane plane{};
    plane.u_axis_ *= -1.0F;
    EXPECT_NE(plane, Plane());
}

TEST(TestShapes, Plane_Equality_Different_VAxis)
{
    Plane plane{};
    plane.v_axis_ *= -1.0F;
    EXPECT_NE(plane, Plane());
}


TEST(TestShapes, Cone_Equality_Equal)
{
    EXPECT_EQ(Cone(), Cone());
}

TEST(TestShapes, Cone_Equality_Small_Epsilon_Radius_Difference)
{
    Cone default_cone{};
    Cone cone{};
    cone.radius_ += zero::math::kSmallEpsilon;
    EXPECT_EQ(cone, Cone());
    cone = default_cone;
    cone.radius_ -= zero::math::kSmallEpsilon;
    EXPECT_EQ(cone, default_cone);
}

TEST(TestShapes, Cone_Equality_Epsilon_Radius_Difference)
{
    Cone default_cone{};
    Cone cone{};
    cone.radius_ += zero::math::kEpsilon;
    EXPECT_EQ(cone, Cone());
    cone = default_cone;
    cone.radius_ -= zero::math::kEpsilon;
    EXPECT_EQ(cone, default_cone);
}

TEST(TestShapes, Cone_Equality_Different_Height)
{
    Cone cone{};
    cone.height_ += 1;
    EXPECT_NE(cone, Cone());
}

TEST(TestShapes, Cone_Equality_Different_Radial_Segments)
{
    Cone cone{};
    cone.radial_segments_ += 1;
    EXPECT_NE(cone, Cone());
}

TEST(TestShapes, Cone_Equality_Different_Height_Segments)
{
    Cone cone{};
    cone.height_segments_ += 1;
    EXPECT_NE(cone, Cone());
}

TEST(TestShapes, Cone_Equality_Different_Is_Open_Ended)
{
    Cone cone{};
    cone.is_open_ended_ = !cone.is_open_ended_;
    EXPECT_NE(cone, Cone());
}


TEST(TestShapes, Cylinder_Equality_Equal)
{
    EXPECT_EQ(Cylinder(), Cylinder());
}

TEST(TestShapes, Cylinder_Equality_Small_Epsilon_Top_Radius_Difference)
{
    Cylinder default_cylinder{};
    Cylinder cylinder{};
    cylinder.top_radius_ += zero::math::kSmallEpsilon;
    EXPECT_EQ(cylinder, default_cylinder);
    cylinder = default_cylinder;
    cylinder.top_radius_ -= zero::math::kSmallEpsilon;
    EXPECT_EQ(cylinder, default_cylinder);
}

TEST(TestShapes, Cylinder_Equality_Epsilon_Top_Radius_Difference)
{
    Cylinder default_cylinder{};
    Cylinder cylinder{};
    cylinder.top_radius_ += zero::math::kEpsilon;
    EXPECT_EQ(cylinder, default_cylinder);
    cylinder = default_cylinder;
    cylinder.top_radius_ -= zero::math::kEpsilon;
    EXPECT_EQ(cylinder, default_cylinder);
}

TEST(TestShapes, Cylinder_Equality_Small_Epsilon_Bottom_Radius_Difference)
{
    Cylinder default_cylinder{};
    Cylinder cylinder{};
    cylinder.bottom_radius_ += zero::math::kSmallEpsilon;
    EXPECT_EQ(cylinder, default_cylinder);
    cylinder = default_cylinder;
    cylinder.bottom_radius_ -= zero::math::kSmallEpsilon;
    EXPECT_EQ(cylinder, default_cylinder);
}

TEST(TestShapes, Cylinder_Equality_Epsilon_Bottom_Radius_Difference)
{
    Cylinder default_cylinder{};
    Cylinder cylinder{};
    cylinder.bottom_radius_ += zero::math::kEpsilon;
    EXPECT_EQ(cylinder, default_cylinder);
    cylinder = default_cylinder;
    cylinder.bottom_radius_ -= zero::math::kEpsilon;
    EXPECT_EQ(cylinder, default_cylinder);
}

TEST(TestShapes, Cylinder_Equality_Different_Height)
{
    Cylinder cylinder{};
    cylinder.height_ += 1;
    EXPECT_NE(cylinder, Cylinder());
}

TEST(TestShapes, Cylinder_Equality_Different_Radial_Segments)
{
    Cylinder cylinder{};
    cylinder.radial_segments_ += 1;
    EXPECT_NE(cylinder, Cylinder());
}

TEST(TestShapes, Cylinder_Equality_Different_Height_Segments)
{
    Cylinder cylinder{};
    cylinder.height_segments_ += 1;
    EXPECT_NE(cylinder, Cylinder());
}

TEST(TestShapes, Cylinder_Equality_Different_Is_Open_Ended)
{
    Cylinder cylinder{};
    cylinder.is_open_ended_ = !cylinder.is_open_ended_;
    EXPECT_NE(cylinder, Cylinder());
}


TEST(TestShapes, Sphere_Equality_Equal)
{
    EXPECT_EQ(Sphere(), Sphere());
}

TEST(TestShapes, Sphere_Equality_Different_Latitude)
{
    Sphere sphere{};
    sphere.latitude_count_ += 1;
    EXPECT_NE(sphere, Sphere());
}

TEST(TestShapes, Sphere_Equality_Different_Longitude)
{
    Sphere sphere{};
    sphere.longitude_count_ += 1;
    EXPECT_NE(sphere, Sphere());
}


TEST(TestShapes, Torus_Equality_Equal)
{
    EXPECT_EQ(Torus(), Torus());
}

TEST(TestShapes, Torus_Equality_Small_Epsilon_Radius_Difference)
{
    Torus default_torus{};
    Torus torus{};
    torus.radius_ += zero::math::kSmallEpsilon;
    EXPECT_EQ(torus, default_torus);
    torus = default_torus;
    torus.radius_ -= zero::math::kSmallEpsilon;
    EXPECT_EQ(torus, default_torus);
}

TEST(TestShapes, Torus_Equality_Epsilon_Radius_Difference)
{
    Torus default_torus{};
    Torus torus{};
    torus.radius_ += zero::math::kEpsilon;
    EXPECT_EQ(torus, default_torus);
    torus = default_torus;
    torus.radius_ -= zero::math::kEpsilon;
    EXPECT_EQ(torus, default_torus);
}

TEST(TestShapes, Torus_Equality_Small_Epsilon_Tube_Radius_Difference)
{
    Torus default_torus{};
    Torus torus{};
    torus.tube_radius_ += zero::math::kSmallEpsilon;
    EXPECT_EQ(torus, default_torus);
    torus = default_torus;
    torus.tube_radius_ -= zero::math::kSmallEpsilon;
    EXPECT_EQ(torus, default_torus);
}

TEST(TestShapes, Torus_Equality_Epsilon_Tube_Radius_Difference)
{
    Torus default_torus{};
    Torus torus{};
    torus.tube_radius_ += zero::math::kEpsilon;
    EXPECT_EQ(torus, default_torus);
    torus = default_torus;
    torus.tube_radius_ -= zero::math::kEpsilon;
    EXPECT_EQ(torus, default_torus);
}

TEST(TestShapes, Torus_Equality_Different_Radial_Segments)
{
    Torus torus{};
    torus.radial_segments_ += 1;
    EXPECT_NE(torus, Torus());
}

TEST(TestShapes, Torus_Equality_Different_Tubular_Segments)
{
    Torus torus{};
    torus.tubular_segments_ += 1;
    EXPECT_NE(torus, Torus());
}