#include "render/ViewVolumeBuilder.hpp"
#include "render/OrthographicViewVolume.hpp"
#include "render/PerspectiveViewVolume.hpp"
#include "math/Matrix4x4.hpp"

namespace zero::render
{

std::unique_ptr<IViewVolume> ViewVolumeBuilder::create(const Camera& camera)
{
    switch (camera.GetProjectionType())
    {
        case Camera::ProjectionType::ORTHOGRAPHIC:
        {
            math::Vec3f near_bottom_left;
            math::Vec3f near_top_right;
            camera.GetNearClipCoordinates(near_bottom_left, near_top_right);
            math::Vec3f far_bottom_left;
            math::Vec3f far_top_right;
            camera.GetFarClipCoordinates(far_bottom_left, far_top_right);
            return std::make_unique<OrthographicViewVolume>(math::Vec3f(near_bottom_left.x_,
                                                                        near_bottom_left.y_,
                                                                        far_bottom_left.z_),
                                                            near_top_right);
        }
        default:
        {
            auto result = camera.GetProjectionMatrix() * camera.GetViewMatrix();
            math::Plane left{};
            left.normal_ = math::Vec3f(result[3][0] + result[0][0],
                                       result[3][1] + result[0][1],
                                       result[3][2] + result[0][2]);
            left.d_ = (result[3][3] + result[0][3]) / left.normal_.Normalize();

            math::Plane right{};
            right.normal_ = math::Vec3f(result[3][0] - result[0][0],
                                        result[3][1] - result[0][1],
                                        result[3][2] - result[0][2]);
            right.d_ = (result[3][3] - result[0][3]) / right.normal_.Normalize();

            math::Plane top{};
            top.normal_ = math::Vec3f(result[3][0] - result[1][0],
                                      result[3][1] - result[1][1],
                                      result[3][2] - result[1][2]);
            top.d_ = (result[3][3] - result[1][3]) / top.normal_.Normalize();

            math::Plane bottom{};
            bottom.normal_ = math::Vec3f(result[3][0] + result[1][0],
                                         result[3][1] + result[1][1],
                                         result[3][2] + result[1][2]);
            bottom.d_ = (result[3][3] + result[1][3]) / bottom.normal_.Normalize();

            math::Plane near{};
            near.normal_ = math::Vec3f(result[3][0] + result[2][0],
                                       result[3][1] + result[2][1],
                                       result[3][2] + result[2][2]);
            near.d_ = (result[3][3] + result[2][3]) / near.normal_.Normalize();

            math::Plane far{};
            far.normal_ = math::Vec3f(result[3][0] - result[2][0],
                                      result[3][1] - result[2][1],
                                      result[3][2] - result[2][2]);
            far.d_ = (result[3][3] - result[2][3]) / far.normal_.Normalize();

            return std::make_unique<PerspectiveViewVolume>(left, right, bottom, top, near, far);
        }
    }
}

} // namespace zero::render