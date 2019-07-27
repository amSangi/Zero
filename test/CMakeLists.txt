project(ZeroTest)

set(EXECUTABLE_OUTPUT_PATH ${ZERO_TEST_OUT_PATH})

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/googletest/googletest-src/googletest/include)
include_directories(include)

add_executable(${PROJECT_NAME} TestMain.cpp
                               src/TestRegistry.cpp
                               src/core/TransformTests.cpp
                               src/math/AngleTests.cpp
                               src/math/BoxTests.cpp
                               src/math/IntersectionTests.cpp
                               src/math/Matrix3x3Tests.cpp
                               src/math/Matrix4x4Tests.cpp
                               src/math/PlaneTests.cpp
                               src/math/QuaternionTests.cpp
                               src/math/SphereTests.cpp
                               src/math/VectorTests.cpp
                               src/render/PropagatorTests.cpp
                               src/render/opengl/GLCompilerTests.cpp
                               src/render/opengl/GLProgramTests.cpp
                               src/render/opengl/GLShaderTests.cpp
                               src/render/opengl/GLTextureTests.cpp)

target_link_libraries(${PROJECT_NAME} ZeroCore)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)