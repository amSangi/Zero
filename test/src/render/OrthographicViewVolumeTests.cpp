#include "render/ViewVolumeBuilder.hpp"
#include "render/Components.hpp"
#include <gtest/gtest.h>
#include <memory>

using namespace zero;
using namespace zero::render;

class TestOrthographicViewVolume : public ::testing::Test {
protected:

    TestOrthographicViewVolume()
    : camera_(Camera::ProjectionType::ORTHOGRAPHIC)
    , volume_(nullptr)
    {}

    void SetUp() override {
        std::shared_ptr<IViewVolume> view_volume = ViewVolumeBuilder::create(camera_);
        volume_ = std::static_pointer_cast<OrthographicViewVolume>(view_volume);
    }

    Camera camera_;
    std::shared_ptr<OrthographicViewVolume> volume_;
};


TEST_F(TestOrthographicViewVolume, IsCulled_Point_InVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Point_OutOfVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_InVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_OutOfVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Sphere_IntersectBox) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_InVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_OutOfVolume) {
    FAIL();
}

TEST_F(TestOrthographicViewVolume, IsCulled_Box_IntersectBox) {
    FAIL();
}