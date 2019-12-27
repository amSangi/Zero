#pragma once

#include <string>
#include <vector>
#include "core/ZBase.hpp"
#include "core/Component.hpp"
#include "math/Sphere.hpp"
#include "math/Vector2.hpp"
#include "math/Vector3.hpp"
#include "math/Matrix4x4.hpp"
#include "math/Quaternion.hpp"

namespace zero::render {

    /**
     * @brief A volume component encapsulating an entity
     */
    struct Volume : public Component {

        /**
         * @brief Default constructor
         */
        Volume()
        : Component()
        , bounding_volume_()
        {}

        /**
         * @brief Construct a volume with a given position and radius
         * @param position the position of the volume
         * @param radius the radius of the volume
         */
        Volume(const math::Vec3f& position, float radius)
        : Component()
        , bounding_volume_(position, radius)
        {}

        /**
         * @brief Construct a volume from a Axis-Aligned Bounding Box
         * @param min the minimum point of the box
         * @param max the maximum point of the box
         */
        Volume(const math::Vec3f& min, const math::Vec3f& max)
        : Component()
        , bounding_volume_(min, max)
        {}

        /**
         * @brief Engulf the other volume so that the other volume is inside
         * @param other the other volume
         */
        void Engulf(const Volume& other);

        /**
         * @brief Apply a matrix transformation to the volume
         * @param transformation the transformation matrix to apply
         */
        void Transform(const math::Matrix4x4& transformation);

        /**
         * @brief The bounding sphere of the entity
         */
        math::Sphere bounding_volume_;
    }; // struct Volume

    /**
     * @brief A material component for rendering
     */
    struct Material : public Component {

        /**
         * @brief Shader files used by the material
         */
        struct Shaders {
            /**
             * @brief The fully qualified shader file name
             */
            ///@{
            std::string vertex_shader_;
            std::string fragment_shader_;
            std::string geometry_shader_;
            std::string tessellation_ctrl_shader_;
            std::string tessellation_eval_shader_;
            std::string compute_shader_;
            ///@}
        }; // struct Shaders

        /**
         * @brief Texture files used by the material
         */
        struct TextureMap {

            /**
             * @brief The fully qualified texture map file name
             */
            ///@{
            std::string alpha_map_;
            std::string ambient_map_;
            std::string diffuse_map_;
            std::string displacement_map_;
            std::string normal_map_;
            ///@}

        }; // struct TextureMap

        /**
         * @brief The name of the material.
         */
        std::string name_;

        /**
         * @brief The ambient color. The color an object reflects when illuminated by ambient light.
         */
        math::Vec3f ambient_color_;

        /**
         * @brief The diffuse color. Essential color of an object under direct white light.
         */
        math::Vec3f diffuse_color_;

        /**
         * @brief The emissive color. This is the self-illumination color of an object.
         */
        math::Vec3f emissive_color_;

        /**
         * @brief The specular color. The color of the light of a specular reflection.
         */
        math::Vec3f specular_color_;

        /**
         * @brief The transparent color. The color to be scaled with the color of translucent light.
         */
        math::Vec3f transparent_color_;

        /**
         * @brief The shininess of a phong-shaded entity.
         */
        float specular_exponent_;

        /**
         * @brief The opacity of the entity in the range [0, 1].
         */
        float opacity_;

        /**
         * @brief The texture map.
         */
        TextureMap texture_map_;

        /**
         * @brief The shaders to use.
         */
         Shaders shaders_;

         /**
          * @brief Is the entity in wireframe mode?
          */
         bool wireframe_enabled_;

    }; // struct Material

    /**
     * @brief An instance of a 3D model.
     */
    struct ModelInstance : public Component {

        /**
         * @brief The fully qualified filename of the 3D model. Empty string for child models.
         */
        std::string filename_;

        /**
         * @brief Identifier for a child of a 3D model. 0 for root models.
         */
        uint32 child_identifier_;

    }; // struct ModelInstance

    /**
     * @brief Camera component that manages properties used for displaying the world on a viewport
     * A camera can not change projection types.
     */
    struct Camera : public Component {

        /**
         * @brief The projection type of the camera
         */
        enum class ProjectionType {
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

        [[nodiscard]] ProjectionType GetProjectionType() const;

        void LookAt(const math::Vec3f& target);

        void GetNearClipCoordinates(math::Vec3f& bottom_left,
                                    math::Vec3f& top_right) const;

        void GetFarClipCoordinates(math::Vec3f& bottom_left,
                                   math::Vec3f& top_right) const;

        /**
         * @brief Get the direction the camera is looking at
         * @return the normalized viewing direction
         */
        [[nodiscard]] math::Vec3f GetViewDirection() const;

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
         * @brief The up vector of the camera in 3D space
         */
        math::Vec3f up_;
        /**
         * @brief The camera position in 3D space.
         *
         * Also known as the "eye" position.
         */
        math::Vec3f position_;
        /**
         * @brief The point the camera is looking at
         */
        math::Vec3f target_;
        /**
         * @brief The camera orientation in 3D space
         */
        math::Quaternion orientation_;
        /**
         * @brief The camera viewport
         */
        Viewport viewport_;

    private:

        /**
         * @brief Create a perspective projection matrix
         *
         * @param vertical_fov the vertical field of view
         * @param aspect_ratio the aspect ratio of the camera viewport
         * @param near the z-distance to the near clipping plane
         * @param far the z-distance to the far clipping plane
         *
         * @return a perspective projection matrix
         */
        static math::Matrix4x4 Perspective(math::Radian vertical_fov,
                                           float aspect_ratio,
                                           float near,
                                           float far);

        /**
         * @brief Create a orthographic projection matrix
         *
         * @param left the left boundary (x)
         * @param right the right boundary (x)
         * @param bottom the bottom boundary (y)
         * @param top the top boundary (y)
         * @param near the near boundary (z)
         * @param far the far boundary (z)
         *
         * @return a orthographic projection matrix
         */
        static math::Matrix4x4 Orthographic(float left,
                                            float right,
                                            float bottom,
                                            float top,
                                            float near,
                                            float far);

        /**
         * @brief The camera projection type. Set once during construction.
         */
        ProjectionType projection_;

    }; // struct Camera

} // namespace zero::render