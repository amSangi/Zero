#pragma once

#include <memory>
#include "ShaderStage.hpp"
#include "ICompiler.hpp"
#include "IProgram.hpp"
#include "Components.hpp"

namespace zero::render {

    class ITextureManager {
    public:

        /**
         * @brief Default constructor
         */
        ITextureManager() = default;

        /**
         * @brief Default virtual destructor
         */
        virtual ~ITextureManager() = default;

        /**
         * @brief Initialize the Shader
         * @param shader_stage The shader stage to initialize
         * @return True if the shader was initialized successfully. Otherwise false.
         */
        virtual bool InitializeShader(ShaderStage shader_stage) = 0;

        /**
         * @brief Create a graphics program given a Material component
         * @param material The material component
         * @return a graphics program
         */
        virtual std::unique_ptr<IProgram> CreateProgram(Material material) = 0;

    }; // class ITextureManager

} // namespace zero::render