#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "core/TimeDelta.hpp"
#include <render/AssimpLoader.hpp>
#include "render/IRenderer.hpp"
#include "render/IShader.hpp"
#include "render/RenderPipeline.hpp"

namespace zero
{
    // Forward declarations
    class Camera;
    class EngineCore;
    class SkyDome;
    class Volume;

namespace render
{

    // Forward declarations
    class GLCompiler;
    class GLModelManager;
    class GLPrimitiveMeshManager;
    class GLTextureManager;
    class GLUniformManager;
    class IProgram;

    /**
     * @brief Render all drawable entities in the game using OpenGL.
     * An entity is considered drawable if it contains Transform, Material, Volume, and Mesh components.
     * Entities with the Camera component are used to perform the rendering.
     */
    class GLRenderer final : public IRenderer
    {
    public:

        explicit GLRenderer(EngineCore* engine_core);

        ~GLRenderer() override;

        /**
         * @brief Load all OpenGL shaders, models, and textures
         */
        void Initialize(const RenderSystemConfig& config) override;

        /**
         * @see IRenderer::Render
         */
        void Render() override;

        /**
         * @brief Release the images in the TextureManager from main memory
         */
        void PostRender() override;

        /**
         * @brief Clear the Compiler of shaders, the ModelManager of models, and TextureManager of textures
         */
        void ShutDown() override;

        /**
         * @see IRenderer::GetModel
         */
        std::weak_ptr<Model> GetModel(const std::string& model) override;

    private:
        /**
         * @brief Initialization helper
         */
        ///@{
        void InitializeGL();
        void InitializeShaders();
        void InitializeModels();
        void InitializeImages();
        void InitializeRenderPasses(const RenderSystemConfig& config);
        ///@}

        /**
         * @brief Read the shader source into the destination
         * @param filename the fully qualified shader file
         * @param destination the destination
         */
        static void ReadShaderSource(const std::string& filename, std::string& destination);

        /**
         * @brief The log title
         */
        static const char* kTitle;

        /**
         * @brief Manage shader and graphics program usage
         */
        std::unique_ptr<GLCompiler> graphics_compiler_;

        /**
         * @brief Manage model meshes
         */
        std::unique_ptr<GLModelManager> model_manager_;

        /**
         * @brief Load 3D models
         */
        std::unique_ptr<AssimpLoader> model_loader_;

        /**
         * @brief Manage primitive meshes
         */
        std::unique_ptr<GLPrimitiveMeshManager> primitive_manager_;

        /**
         * @brief Manage texture image usage
         */
        std::unique_ptr<GLTextureManager> texture_manager_;

        /**
         * @brief Manage uniform buffer objects
         */
        std::unique_ptr<GLUniformManager> uniform_manager_;

        /**
         * @brief The pipeline that renders all entities to the screen by executing various render passes.
         * The passes are added during initialization.
         */
        std::unique_ptr<RenderPipeline> render_pipeline_;

        /**
         * @brief The engine core that contains shared objects and game data.
         */
        EngineCore* engine_core_;

    }; // class GLRenderer

} // namespace render
} // namespace zero