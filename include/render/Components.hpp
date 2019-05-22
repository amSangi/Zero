#pragma once

#include <string>
#include <vector>
#include "core/ZBase.hpp"
#include "core/Component.hpp"
#include "math/Sphere.hpp"
#include "math/Vector3.hpp"
#include "math/Vector2.hpp"

namespace zero::render {

    /**
     * @brief A triangular mesh mesh component
     */
    struct Mesh : public Component {
        /**
         * @brief The bounding sphere of the mesh
         */
        math::Sphere bounding_volume_;

        /**
         * @brief The index data for the vertices
         */
        std::vector<uint32> indices_;

        /**
         * @brief The vertices of the entity
         */
        std::vector<math::Vec3f> vertices_;

        /**
         * @brief The vertex normals
         */
        std::vector<math::Vec3f> normals_;

        /**
         * @brief The UV texture coordinates of the vertices
         */
        std::vector<math::Vec2f> texture_coordinates_;
    }; // struct Mesh


    /**
     * @brief A material component for rendering
     */
    struct Material : public Component {

        /**
         * @brief Fully qualified texture map files
         */
        struct TextureMap {

            /**
             * @brief The fully qualified ambient texture map file name
             */
            std::string ambient_map_;
            /**
             * @brief The fully qualified diffuse texture map file name
             */
            std::string diffuse_map_;
            /**
             * @brief The fully qualified specular texture map file name
             */
            std::string specular_map_;
            /**
             * @brief The fully qualified alpha texture map file name
             */
            std::string alpha_map_;
            /**
             * @brief The fully qualified bump texture map file name
             */
            std::string bump_map_;

        }; // struct TextureMap

        /**
         * @brief The name of the material
         */
        std::string name_;

        /**
         * @brief The ambient colour
         */
        math::Vec3f ambient_;

        /**
         * @brief The diffuse colour
         */
        math::Vec3f diffuse_;

        /**
         * @brief The specular colour
         */
        math::Vec3f specular;

        /**
         * @brief The shininess
         */
        float specular_exponent_;

        /**
         * @brief The texture map
         */
        TextureMap texture_map_;
    }; // struct Material

    /**
     * @brief Captures and displays the world
     */
    struct Camera : public Component {

        /**
         * @brief The projection type of the camera
         */
        enum class ProjectionType {
            PROJECTION_PERSPECTIVE,     //<! Perspective Projection
            PROJECTION_ORTHOGRAPHIC     //<! Orthographic Projection
        }; // enum class ProjectionType

        /**
         * @brief The viewport of the camera
         */
        struct Viewport {

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
         * @brief The horizontal field of view in degrees
         */
        uint8 field_of_view_;
        /**
         * @brief The camera position in 3D space
         */
        math::Vec3f position_;
        /**
         * @brief The camera orientation in 3D space
         */
        math::Quaternion orientation_;
        /**
         * @brief The camera projection type
         */
        ProjectionType projection_;
        /**
         * @brief The camera viewport
         */
        Viewport viewport_;

    }; // struct Camera

} // namespace zero::render