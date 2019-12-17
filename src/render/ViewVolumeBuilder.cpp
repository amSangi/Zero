#include "render/ViewVolumeBuilder.hpp"

using namespace zero::render;

std::unique_ptr<IViewVolume> ViewVolumeBuilder::create(const zero::render::Camera& camera) {
    switch (camera.projection_)
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
            // Get the min/max corners of the near/far planes
            math::Vec3f near_bottom_left;
            math::Vec3f near_top_right;
            math::Vec3f far_bottom_left;
            math::Vec3f far_top_right;
            camera.GetNearClipCoordinates(near_bottom_left, near_top_right);
            camera.GetFarClipCoordinates(far_bottom_left, far_top_right);

            // Construct the other four corners of the near/far planes
            math::Vec3f near_bottom_right{near_top_right.x_,   near_bottom_left.y_, near_bottom_left.z_};
            math::Vec3f near_top_left    {near_bottom_left.x_, near_top_right.y_,   near_bottom_left.z_};
            math::Vec3f far_bottom_right {far_top_right.x_,    far_bottom_left.y_,  far_bottom_left.z_};
            math::Vec3f far_top_left     {far_bottom_left.x_,  far_top_right.y_,    far_bottom_left.z_};

            // Construct the bounding planes of the Frustrum
            // The normal of each plane must point towards the inside of the Frustrum
            math::Plane left   {near_bottom_left,  far_bottom_left,   near_top_left};
            math::Plane right  {near_top_right,    far_bottom_right,  near_bottom_right};
            math::Plane bottom {near_bottom_right, far_bottom_right,  near_bottom_left};
            math::Plane top    {near_top_left,     far_top_left,      near_top_right};
            math::Plane near   {near_top_right,    near_bottom_right, near_top_left};
            math::Plane far    {far_top_left,      far_bottom_left,   far_top_right};

            return std::make_unique<PerspectiveViewVolume>(left, right, bottom, top, near, far);
        }
    }
}