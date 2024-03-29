#pragma once

#include "component/Component.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

namespace zero
{

    /**
     * @brief Camera component that manages properties used for displaying the world on a viewport
     * A camera can not change projection types.
     */
    struct Camera : public Component
    {

        /**
         * @brief The projection type of the camera
         */
        enum class ProjectionType
        {
            PERSPECTIVE,     ///< Perspective Projection
            ORTHOGRAPHIC     ///< Orthographic Projection
        }; // enum class ProjectionType

        /**
         * @brief The display area of the camera
         */
        struct Viewport {

            Viewport();

            /**
             * @brief Get the aspect ratio of the view port (Width / Height)
             * @return the aspect ratio
             */
            [[nodiscard]] float GetAspectRatio() const;

            /**
             * @brief The x position of the view in pixels from the left of the screen
             */
            uint32 x_;

            /**
             * @brief The y position of the view in pixels from the bottom of the screen
             */
            uint32 y_;

            /**
             * @brief The width of the view in pixels
             */
            uint32 width_;

            /**
             * @brief The height of the view in pixels
             */
            uint32 height_;

        }; // struct Viewport

        /**
         * @brief Default constructs a perspective camera
         */
        Camera();

        /**
         * @brief Default constructs a camera of a given type
         * @param projection_type the projection type of the camera
         */
        explicit Camera(ProjectionType projection_type);

        /**
         * @brief Get the projection type of the camera
         * @return the projection type
         */
        [[nodiscard]] ProjectionType GetProjectionType() const;

        /**
         * @brief Translate the position relative to the world
         * @param translation the translation
         */
        void Translate(const math::Vec3f& translation);

        /**
         * @brief Translate the position relative to the camera
         * @param translation the translation
         */
        void TranslateRelative(const math::Vec3f& translation);

        /**
         * @brief Rotate the camera relative to the world
         * @param rotation the rotation to apply
         */
        void Rotate(const math::Quaternion& rotation);

        /**
         * @brief Pitch the camera relative to its right vector
         * @param angle the angle of rotation
         */
        void PitchRelative(const math::Radian& angle);

        /**
         * @brief Yaw the camera relative to its up vector
         * @param angle the angle of rotation
         */
        void YawRelative(const math::Radian& angle);

        /**
         * @brief Roll the camera around its viewing direction
         * @param angle the angle of rotation
         */
        void RollRelative(const math::Radian& angle);

        /**
         * @brief Orientate the camera to look at the target
         * @param target the target to look at
         */
        void LookAt(const math::Vec3f& target);

        /**
         * @brief Get the near plane coordinates in world coordinates
         * @param bottom_left the bottom left corner of the near plane
         * @param top_right the top right corner of the near plane
         */
        void GetNearClipCoordinates(math::Vec3f& bottom_left,
                                    math::Vec3f& top_right) const;

        /**
         * @brief Get the far plane coordinates in world coordinates
         * @param bottom_left the bottom left corner of the far plane
         * @param top_right the top right corner of the far plane
         */
        void GetFarClipCoordinates(math::Vec3f& bottom_left,
                                   math::Vec3f& top_right) const;

        /**
         * @brief Get the relative up vector of the camera
         * @return the up vector
         */
        [[nodiscard]] math::Vec3f GetUpVector() const;

        /**
         * @brief Get the relative right vector of the camera
         * @return the right vector
         */
        [[nodiscard]] math::Vec3f GetRightVector() const;

        /**
         * @brief Get the direction the camera is looking at
         * @return the normalized viewing direction
         */
        [[nodiscard]] math::Vec3f GetViewDirection() const;

        /**
         * @brief Get the vertical field of view
         * @return the vertical fov angle in radians
         */
        [[nodiscard]] math::Radian GetVerticalFieldOfView() const;

        /**
         * @brief Get the projection matrix based on the projection type.
         *
         * @return the projection matrix
         */
        [[nodiscard]] math::Matrix4x4 GetProjectionMatrix() const;

        /**
         * @brief Get the view matrix. Converts world-space coordinates to view-space
         * coordinates.
         *
         * @return the view matrix
         */
        [[nodiscard]] math::Matrix4x4 GetViewMatrix() const;

        /**
         * @brief Get the inverse view matrix. Converts view-space coordinates to
         * world-space coordinates.
         *
         * @return the inverse view matrix
         */
        [[nodiscard]] math::Matrix4x4 GetCameraToWorldMatrix() const;

        /**
         * @brief The z-distance to the near clipping plane from the eye
         */
        float near_clip_;
        /**
         * @brief The z-distance to the far clipping plane from the eye
         */
        float far_clip_;
        /**
         * @brief The horizontal field of view
         */
        math::Degree horizontal_field_of_view_;
        /**
         * @brief The camera position in 3D space.
         *
         * Also known as the "eye" position.
         */
        math::Vec3f position_;
        /**
         * @brief The camera orientation in 3D space.
         */
        math::Quaternion orientation_;
        /**
         * @brief The camera viewport
         */
        Viewport viewport_;

        /**
         * @brief Should the bounding view volumes of the game entities be rendered?
         */
        bool render_bounding_volumes_;

    private:

        /**
         * @brief The camera projection type. Set once during construction.
         */
        ProjectionType projection_;

    }; // struct Camera

} // namespace zero
