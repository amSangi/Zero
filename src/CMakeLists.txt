project(ZeroCore)

set(LIBRARY_OUTPUT_PATH ${ZERO_LIB_OUT_PATH})

## Create Library ##
add_library(${PROJECT_NAME} component/Animator.cpp
                            component/Camera.cpp
                            component/Component.cpp
                            component/Light.cpp
                            component/Material.cpp
                            component/PrimitiveInstance.cpp
                            component/Shapes.cpp
                            component/SkyDome.cpp
                            component/Transform.cpp
                            component/Volume.cpp
                            core/AssetManager.cpp
                            core/EventBus.cpp
                            core/Input.cpp
                            core/Logger.cpp
                            core/TransformPropagator.cpp
                            engine/Engine.cpp
                            engine/EntityInstantiator.cpp
                            math/Box.cpp
                            math/Intersection.cpp
                            math/Matrix3x3.cpp
                            math/Matrix4x4.cpp
                            math/Plane.cpp
                            math/Quaternion.cpp
                            math/Radian.cpp
                            math/Sphere.cpp
                            render/AssimpLoader.cpp
                            render/CascadedShadowMap.cpp
                            render/CullingManager.cpp
                            render/Image.cpp
                            render/Instantiator.cpp
                            render/Mesh.cpp
                            render/MeshGenerator.cpp
                            render/Model.cpp
                            render/OrthographicViewVolume.cpp
                            render/PerspectiveViewVolume.cpp
                            render/RenderPipeline.cpp
                            render/RenderSystem.cpp
                            render/ViewVolumeBuilder.cpp
                            render/VolumePropagator.cpp
                            render/Window.cpp
                            render/opengl/GLCompiler.cpp
                            render/opengl/GLMesh.cpp
                            render/opengl/GLModel.cpp
                            render/opengl/GLModelManager.cpp
                            render/opengl/GLPrimitiveGenerator.cpp
                            render/opengl/GLPrimitiveMeshManager.cpp
                            render/opengl/GLProgram.cpp
                            render/opengl/GLRenderer.cpp
                            render/opengl/GLSampler.cpp
                            render/opengl/GLShader.cpp
                            render/opengl/GLTexture.cpp
                            render/opengl/GLTextureManager.cpp
                            render/opengl/GLUniformManager.cpp
                            render/opengl/pipeline/GLEntityRenderPass.cpp
                            render/opengl/pipeline/GLShadowMapPass.cpp
                            render/opengl/pipeline/GLSkyDomePass.cpp)


## Link Libraries ##
target_link_libraries(${PROJECT_NAME} EnTT
                                      assimp
                                      ${OPENGL_gl_LIBRARY}
                                      ${GLEW_LIBRARIES}
                                      ${SDL2_LIBRARY}
                                      ${SDL2_IMAGE_LIBRARIES})