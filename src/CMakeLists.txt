project(ZeroCore)

set(LIBRARY_OUTPUT_PATH ${ZERO_LIB_OUT_PATH})


## Create Library ##
add_library(${PROJECT_NAME}  event/EventBus.cpp
                             core/Engine.cpp
                             core/System.cpp
                             core/Transform.cpp
                             math/Box.cpp
                             math/Intersection.cpp
                             math/Matrix3x3.cpp
                             math/Matrix4x4.cpp
                             math/Plane.cpp
                             math/Quaternion.cpp
                             math/Radian.cpp
                             math/Sphere.cpp
                             render/Camera.cpp
                             render/Image.cpp
                             render/Material.cpp
                             render/Mesh.cpp
                             render/MeshGenerator.cpp
                             render/OrthographicViewVolume.cpp
                             render/PerspectiveViewVolume.cpp
                             render/PrimitiveInstance.cpp
                             render/Propagator.cpp
                             render/RenderSystem.cpp
                             render/Shapes.cpp
                             render/ViewVolumeBuilder.cpp
                             render/Volume.cpp
                             render/Window.cpp
                             render/opengl/GLAssetImportWrapper.cpp
                             render/opengl/GLCompiler.cpp
                             render/opengl/GLDefaultShader.cpp
                             render/opengl/GLInstantiator.cpp
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
                             render/opengl/GLTextureManager.cpp)


## Link Libraries ##
target_link_libraries(${PROJECT_NAME} EnTT
                                      assimp
                                      ${OPENGL_gl_LIBRARY}
                                      ${GLEW_LIBRARIES}
                                      ${SDL2_LIBRARY}
                                      ${SDL2_IMAGE_LIBRARIES})