project(ZeroCore)

set(LIBRARY_OUTPUT_PATH ${ZERO_LIB_OUT_PATH})
add_compile_definitions(GLEW_STATIC=1)

## Create Library ##
add_library(${PROJECT_NAME}
                            # Animation Files
                            animation/AnimationSystem.cpp
                            # Component Files
                            component/Camera.cpp
                            component/Light.cpp
                            component/Material.cpp
                            component/PrimitiveInstance.cpp
                            component/Shapes.cpp
                            component/SkyDome.cpp
                            component/Transform.cpp
                            component/Volume.cpp
                            # Core Files
                            core/AssetManager.cpp
                            core/EventBus.cpp
                            core/Input.cpp
                            core/Logger.cpp
                            core/TransformSystem.cpp
                            # Engine Files
                            engine/Engine.cpp
                            engine/EntityInstantiator.cpp
                            # Math Files
                            math/Box.cpp
                            math/Intersection.cpp
                            math/Matrix3x3.cpp
                            math/Matrix4x4.cpp
                            math/Plane.cpp
                            math/Quaternion.cpp
                            math/Radian.cpp
                            math/Sphere.cpp
                            # Render Files
                            render/ModelLoader.cpp
                            render/CascadedShadowMap.cpp
                            render/Image.cpp
                            render/EntityFactory.cpp
                            render/MeshData.cpp
                            render/MeshGenerator.cpp
                            render/RenderSystem.cpp
                            render/Window.cpp
                            # Scene Files
                            render/scene/OrthographicViewVolume.cpp
                            render/scene/PerspectiveViewVolume.cpp
                            render/scene/ViewVolumeBuilder.cpp
                            render/scene/CullingManager.cpp
                            render/scene/SceneManager.cpp
                            render/scene/RenderView.cpp
                            # Renderer Files
                            render/renderer/DrawCallComparator.cpp
                            render/renderer/RenderingPipeline.cpp
                            render/renderer/UniformManager.cpp
                            # OpenGL Files
                            render/renderer/opengl/GLFrameBuffer.cpp
                            render/renderer/opengl/GLMesh.cpp
                            render/renderer/opengl/GLProgram.cpp
                            render/renderer/opengl/GLRenderHardware.cpp
                            render/renderer/opengl/GLSampler.cpp
                            render/renderer/opengl/GLShader.cpp
                            render/renderer/opengl/GLTexture.cpp
                            render/renderer/opengl/GLUniformBuffer.cpp
                            render/renderer/opengl/glew.c
                            # Render Pass and Draw Call Files
                            render/renderer/drawcall/EntityDrawCall.cpp
                            render/renderer/drawcall/SkyDomeDrawCall.cpp
                            render/renderer/drawcall/ShadowMapDrawCall.cpp
                            render/renderer/renderpass/CascadedShadowMapRenderPass.cpp
                            render/renderer/renderpass/EntityRenderPass.cpp)


## Link Libraries ##
target_link_libraries(${PROJECT_NAME} EnTT
        assimp
        SDL3-static
        SDL3_image::SDL3_image
        ${GLEW_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
)