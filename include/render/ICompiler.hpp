#pragma once

#include <memory>
#include <string>
#include "render/ShaderStage.hpp"
#include "render/IProgram.hpp"

namespace zero::render {

    // Forward declarations
    class Material;

    /**
     * @brief Stores Graphics Shaders and creates Graphics Programs. The Material component dictates the
     * shaders used during the rendering of an entity.
     */
    class ICompiler {
    public:

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
         * @return a graphics program. nullptr if a valid graphics program failed to be created.
         */
        virtual std::shared_ptr<IProgram> CreateProgram(const Material& material) = 0;

    }; // class ICompiler

} // namespace zero::render