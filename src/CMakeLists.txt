project(ZeroCore)

set(LIBRARY_OUTPUT_PATH ${ZERO_LIB_OUT_PATH})

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
                            render/AssimpLoader.cpp
                            render/CascadedShadowMap.cpp
                            render/Image.cpp
                            render/Instantiator.cpp
                            render/MeshData.cpp
                            render/MeshGenerator.cpp
                            render/RenderSystem.cpp
                            render/Window.cpp
                            # Scene Files
                            render/scene/OrthographicViewVolume.cpp
                            render/scene/PerspectiveViewVolume.cpp
                            render/scene/ViewVolumeBuilder.cpp
                            # Renderer Files
                            render/renderer/RenderQueue.cpp
                            # OpenGL Files
                            render/renderer/opengl/GLFrameBuffer.cpp
                            render/renderer/opengl/GLMesh.cpp
                            render/renderer/opengl/GLProgram.cpp
                            render/renderer/opengl/GLRenderHardware.cpp
                            render/renderer/opengl/GLSampler.cpp
                            render/renderer/opengl/GLShader.cpp
                            render/renderer/opengl/GLTexture.cpp
                            render/renderer/opengl/GLUniformBuffer.cpp)


## Link Libraries ##
target_link_libraries(${PROJECT_NAME} EnTT
                                      assimp
                                      ${OPENGL_gl_LIBRARY}
                                      ${GLEW_LIBRARIES}
                                      ${SDL2_LIBRARY}
                                      ${SDL2_IMAGE_LIBRARIES})