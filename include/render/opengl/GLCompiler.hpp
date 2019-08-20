#pragma once

#include <string>
#include <unordered_map>
#include "render/ICompiler.hpp"

namespace zero::render {

    // Forward Declaration
    class GLShader;

    /**
     * @brief Create OpenGL graphics programs and manage GLShaders
     */
    class GLCompiler : public ICompiler {
    public:

        GLCompiler() = default;

        ~GLCompiler() override = default;

        /**
         * @brief Create a new graphics program from a Material component
         * @param material the material component
         * @return a graphics program
         */
        std::shared_ptr<IProgram> CreateProgram(const Material& material) override;

        /**
         * @brief Initialize a new shader into the shader map
         * @param stage the shader description
         * @return true if the initialization was successful. Otherwise false.
         */
        bool InitializeShader(const ShaderStage& stage) override;

        /**
         * @brief Clear the shader map
         */
        void ClearShaders();

    private:
        /**
         * @brief Filename to GLShader map
         */
        std::unordered_map<std::string, std::shared_ptr<GLShader>> shader_map_;

    }; // class GLCompiler

} // namespace zero::render