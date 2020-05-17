#pragma once

#include <string>
#include "core/Component.hpp"
#include "math/Vector3.hpp"

namespace zero::render
{

    /**
     * @brief A material component for rendering
     */
    struct Material : public Component
    {

        Material();

        /**
         * @brief Shader files used by the material
         */
        struct Shaders
        {
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
        struct TextureMap
        {

            /**
             * @brief The fully qualified texture map file name
             */
            ///@{
            std::string diffuse_map_;
            std::string specular_map_;
            std::string normal_map_;
            ///@}

        }; // struct TextureMap

        /**
         * @brief The name of the material.
         */
        std::string name_;

        /**
         * @brief The texture map.
         */
        TextureMap texture_map_;

        /**
         * @brief The shaders to use.
         */
        Shaders shaders_;

        /**
         * @brief The diffuse color. Essential color of an object under direct white light.
         */
        math::Vec3f diffuse_color_;

        /**
         * @brief The shininess of a phong-shaded entity.
         */
        float specular_exponent_;

        /**
         * @brief Is the entity in wireframe mode?
         */
        bool wireframe_enabled_;

        /**
         * @brief Is the entity and its children visible? Only visible entities are rendered.
         */
        bool visible_;

    }; // struct Material

} // namespace zero::render
