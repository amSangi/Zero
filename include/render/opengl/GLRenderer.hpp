#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "core/TimeDelta.hpp"
#include "render/IRenderer.hpp"
#include "render/IShader.hpp"

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
        void Initialize() override;

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
        std::weak_ptr<IModel> GetModel(const std::string& model) override;

    private:
        /**
         * @brief Initialization helper
         */
        ///@{
        void InitializeGL();
        void InitializeShaders();
        void InitializeModels();
        void InitializeImages();
        ///@}

        /**
         * @brief Render the sky dome
         * @param camera the camera to render to
         * @param projection_matrix the projection matrix of the camera
         * @param view_matrix the view matrix of the camera
         * @param sky_dome the sky dome to render
         */
        void RenderSkyDome(const Camera &camera,
                           const math::Matrix4x4& projection_matrix,
                           const math::Matrix4x4& view_matrix,
                           const SkyDome& sky_dome);

        /**
         * @brief Render all viewable entities with a mesh
         * @param camera the camera to render to
         * @param registry the registry containing all the entities and their components
         */
        void RenderEntities(const Camera& camera, const entt::registry& registry);

        /**
         * @brief Render the wireframe of the bounding volume
         * @param camera the camera to render to
         * @param projection_matrix the projection matrix of the camera
         * @param view_matrix the view matrix of the camera
         * @param volume the volume to render
         */
        void RenderVolume(const Camera& camera,
                          const math::Matrix4x4& projection_matrix,
                          const math::Matrix4x4& view_matrix,
                          const Volume& volume);

        /**
         * @brief Update all OpenGL settings (e.g. Set viewport, clear color buffers, etc)
         * @param camera the camera being rendered to
         */
        static void UpdateGL(const Camera& camera);

        /**
         * @brief Toggle wireframe mode
         * @param enable_wireframe should wireframe be enabled?
         */
        static void ToggleWireframeMode(bool enable_wireframe);

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
         * @brief
         */
        EngineCore *engine_core_;

    }; // class GLRenderer

} // namespace render
} // namespace zero