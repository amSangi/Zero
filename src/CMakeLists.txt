project(ZeroCore)

set(LIBRARY_OUTPUT_PATH ${ZERO_LIB_OUT_PATH})

## Create Library ##
add_library(${PROJECT_NAME}
                            # Animation Files
                            animation/AnimationSystem.cpp
                            # Component Files
                            component/Animator.cpp
                            component/Camera.cpp
                            component/Component.cpp
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
                            core/TransformPropagator.cpp
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
                            render/EntityPrototype.cpp
                            render/Instantiator.cpp
                            render/Model.cpp
                            render/Node.cpp
                            render/RenderSystem.cpp
                            render/VolumePropagator.cpp
                            render/Window.cpp
                            # Scene Files
                            render/scene/CullingManager.cpp
                            render/scene/OrthographicViewVolume.cpp
                            render/scene/PerspectiveViewVolume.cpp
                            render/scene/Renderable.cpp
                            render/scene/RenderView.cpp
                            render/scene/SceneManager.cpp
                            render/scene/ViewVolumeBuilder.cpp
                            # Renderer Files
                            render/renderer/Image.cpp
                            render/renderer/IMesh.cpp
                            render/renderer/MeshGenerator.cpp
                            render/renderer/RenderPipeline.cpp
                            render/renderer/RenderStage.cpp
                            # Pipeline Files
                            render/renderer/pipeline/EntityRenderStage.cpp
                            render/renderer/pipeline/ShadowMapStage.cpp
                            render/renderer/pipeline/SkyDomeStage.cpp
                            # OpenGL Files
                            render/renderer/opengl/GLMesh.cpp
                            render/renderer/opengl/GLModelManager.cpp
                            render/renderer/opengl/GLPrimitiveGenerator.cpp
                            render/renderer/opengl/GLProgram.cpp
                            render/renderer/opengl/GLRenderingContext.cpp
                            render/renderer/opengl/GLRenderingManager.cpp
                            render/renderer/opengl/GLRenderTarget.cpp
                            render/renderer/opengl/GLSampler.cpp
                            render/renderer/opengl/GLShader.cpp
                            render/renderer/opengl/GLShaderManager.cpp
                            render/renderer/opengl/GLTexture.cpp
                            render/renderer/opengl/GLTextureManager.cpp
                            render/renderer/opengl/GLUniformManager.cpp
                            render/renderer/opengl/ubo/GLBaseUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLBoneUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLCameraUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLDirectionalLightUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLLightInfoUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLMaterialUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLModelUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLPointLightUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLShadowMapUniformBuffer.cpp
                            render/renderer/opengl/ubo/GLSpotLightUniformBuffer.cpp)


## Link Libraries ##
target_link_libraries(${PROJECT_NAME} EnTT
                                      assimp
                                      ${OPENGL_gl_LIBRARY}
                                      ${GLEW_LIBRARIES}
                                      ${SDL2_LIBRARY}
                                      ${SDL2_IMAGE_LIBRARIES})