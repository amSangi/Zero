#pragma once

#include "component/Component.hpp"
#include "math/Vector3.hpp"

namespace zero
{
    /**
     * @brief A SkyDome of the world
     * Multiple sky domes may exist during a game but only the first arbitrarily chosen active sky dome is rendered.
     * It is the responsibility of a Game System to ensure that the sky dome to be rendered is the only active sky dome.
     */
    struct SkyDome : public Component
    {
        SkyDome();

        ~SkyDome() = default;
	    /**
		 * @brief Set the sky dome shaders
		 * @param vertex_shader the vertex shader
		 * @param fragment_shader the fragment shader
		 */
	    void SetShaders(const std::string& vertex_shader, const std::string& fragment_shader);

		/**
		 * @brief Retrieve the vertex shader
		 * @return the vertex shader
		 */
		[[nodiscard]] const std::string& GetVertexShader() const;

		/**
		 * @brief Retrieve the fragment shader
		 * @return the fragment shader
		 */
		[[nodiscard]] const std::string& GetFragmentShader() const;

	    /**
		 * @brief Get the unique shader identifier
		 */
	    [[nodiscard]] uint32 GetShaderID() const;

        /**
         * @brief Is the sky dome active?
         * A game may have multiple sky domes and switch between them.
         * The first active sky dome will be rendered. There is no particular order so a GameSystem must manage
         * which one sky dome to enable at any given moment.
         */
        bool is_active_;

        /**
         * @brief The color at the top of the sky dome
         */
        math::Vec3f apex_color_;

        /**
         * @brief The color at the center of the sky dome
         */
        math::Vec3f center_color_;

	private:
	    /**
		 * @brief Generate the shader identifier by hashing the shaders
		 */
	    void ComputeShaderID();

	    /**
		 * @brief The unique shader identifier
		 */
	    uint32 shader_id_;

	    /**
		 * @brief The vertex shader file
		 */
	    std::string vertex_shader_;

	    /**
		 * @brief The fragment shader file
		 */
	    std::string fragment_shader_;

    }; // struct SkyDome

} // namespace zero