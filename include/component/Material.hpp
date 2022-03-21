#pragma once

#include <string>
#include "component/Component.hpp"
#include "math/Vector3.hpp"

namespace zero
{

    /**
     * @brief A material component for rendering
     */
    struct Material : public Component
    {
        /**
         * @brief Shader files used by the material
         */
        struct Shaders
        {
            /**
             * @brief The shader file name
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
             * @brief The texture map file name
             */
            ///@{
            std::string diffuse_map_;
            std::string normal_map_;
            std::string specular_map_;
            ///@}

        }; // struct TextureMap

        Material();

        /**
         * @brief Get the texture map
         * @return the texture map
         */
        [[nodiscard]] const TextureMap& GetTextureMap() const;

        /**
         * @brief Get the shaders
         * @return the shaders
         */
        [[nodiscard]] const Shaders& GetShaders() const;

        /**
         * @brief Get the unique texture identifier
         */
        [[nodiscard]] uint32 GetTextureID() const;

        /**
         * @brief Get the unique shader identifier
         */
        [[nodiscard]] uint32 GetShaderID() const;

        /**
         * @brief Set the texture map
         * @param texture_map the texture map
         */
        void SetTextureMap(const TextureMap& texture_map);

        /**
         * @brief Set the shaders
         * @param shaders the shaders
         */
        void SetShaders(const Shaders& shaders) ;

    public:

        /**
         * @brief The name of the material.
         */
        std::string name_;

        /**
         * @brief The diffuse color. Essential color of an object under direct white light.
         */
        math::Vec3f diffuse_color_;

        /**
         * @brief How much specular contributes to the final color
         */
        float specular_intensity_;

        /**
         * @brief The shininess of an entity.
         */
        float specular_exponent_;

        /**
         * @brief Is the entity two sided?
         * This will disable backface culling for the entity
         */
        bool two_sided_;

        /**
         * @brief Is the entity in wireframe mode?
         */
        bool wireframe_enabled_;

        /**
         * @brief Is the entity and its children visible? Only visible entities are rendered.
         */
        bool visible_;

    private:

        /**
         * @brief Generate the texture identifier by hashing the texture map
         */
        void ComputeTextureID();

        /**
         * @brief Generate the shader identifier by hashing the shaders
         */
        void ComputeShaderID();

        /**
         * The texture map
         */
        TextureMap texture_map_;

        /**
         * @brief The shaders to use
         */
        Shaders shaders_;

        /**
         * @brief The unique texture identifier
         */
        uint32 texture_id_;

        /**
         * @brief The unique shader identifier
         */
        uint32 shader_id_;

    }; // struct Material

} // namespace zero
