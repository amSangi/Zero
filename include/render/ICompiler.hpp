#pragma once

#include <memory>
#include <string>
#include "Components.hpp"
#include "IShader.hpp"
#include "IProgram.hpp"

namespace zero::render {

    /**
     * @brief Stores Graphics Shaders and creates Graphics Programs
     */
    class ICompiler {
    public:

        /**
         * @brief Container for a Graphics Shader
         */
        struct ShaderStage {

            /**
             * @brief The Shader Type
             */
            IShader::Type type_;

            /**
             * @brief The Shader name
             */
            std::string name_;

            /**
             * @brief The Shader source code
             */
            std::string source_;

        }; // struct ShaderStage

        /**
         * @brief Default Constructor
         */
        ICompiler() = default;

        /**
         * @brief Virtual Destructor
         */
        virtual ~ICompiler() = default;

        /**
         * @brief Create and store a Graphics shader
         * @param stage The shader stage
         * @return True if the shader was setup correctly. Otherwise false.
         */
        virtual bool InitializeShader(const ShaderStage& stage) = 0;

        /**
         * @brief Create a Graphics Program given a material component
         * @param material The material component
         * @return A Graphics Program
         */
        virtual std::shared_ptr<IProgram> CreateProgram(const Material& material) = 0;

    }; // class ICompiler

} // namespace zero::render