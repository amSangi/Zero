project(ZeroCore)

set(LIBRARY_OUTPUT_PATH ${ZERO_LIB_OUT_PATH})


## Create Library ##
add_library(${PROJECT_NAME}  event/EventBus.cpp
                             core/System.cpp
                             math/Box.cpp
                             math/Intersection.cpp
                             math/Matrix3x3.cpp
                             math/Matrix4x4.cpp
                             math/Plane.cpp
                             math/Quaternion.cpp
                             math/Radian.cpp
                             math/Sphere.cpp
                             render/Window.cpp
                             render/opengl/GLProgram.cpp
                             render/opengl/GLShader.cpp)


## Link Libraries ##
target_link_libraries(${PROJECT_NAME} EnTT
                                      ${OPENGL_gl_LIBRARY}
                                      ${GLEW_LIBRARIES}
                                      ${SDL2_LIBRARY}
                                      ${SDL2_IMAGE_LIBRARIES})