#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace zero
{
    // Forward declarations
    class Material;
    class SkyDome;

namespace render
{

    // Forward declarations
    class GLProgram;
    class GLShader;
    class ShaderStage;

    /**
     * @brief Create OpenGL graphics programs and manages GLShaders
     */
    class GLCompiler
    {
    public:

        GLCompiler();

        ~GLCompiler() = default;

        /**
         * @brief Create a graphics program from a Material component
         * @param material the material component
         * @return a graphics program
         */
        std::shared_ptr<GLProgram> CreateProgram(const Material& material);

        /**
         * @brief Create a graphics program from a SkyDome component
         * @param sky_dome the sky dome component
         * @return a graphics program
         */
        std::shared_ptr<GLProgram> CreateProgram(const SkyDome& sky_dome);

        /**
         * @brief Create a graphics program from the vertex and fragment shader file names
         * @param vertex_shader the vertex shader name
         * @param fragment_shader the fragment shader name
         * @return a graphics program
         */
        std::shared_ptr<GLProgram> CreateProgram(const std::string& vertex_shader, const std::string& fragment_shader);

        /**
         * @brief Initialize a new shader into the shader map
         * @param stage the shader description
         * @return true if the initialization was successful. Otherwise false.
         */
        bool InitializeShader(const ShaderStage& stage);

        /**
         * @brief Clear the shader map
         */
        void ClearShaders();

        /**
         * @brief Clear the graphics program cache
         */
        void ClearPrograms();

    private:
        /**
         * @brief Filename to GLShader map
         */
        std::unordered_map<std::string, std::shared_ptr<GLShader>> shader_map_;

        /**
         * @brief Concatenated shader name to graphics program map
         * Graphics program caching avoids having to link shaders that have already been linked again.
         */
        std::unordered_map<std::string, std::unique_ptr<GLProgram>> program_map_;

    }; // class GLCompiler

} // namespace render
} // namespace zero