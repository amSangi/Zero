project(ZeroTest)

set(EXECUTABLE_OUTPUT_PATH ${ZERO_TEST_OUT_PATH})

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/googletest/googletest/include)

add_executable(${PROJECT_NAME} TestMain.cpp
                               src/TestRegistry.cpp
                               src/component/CameraTests.cpp
                               src/component/ShapeTests.cpp
                               src/component/TransformTests.cpp
                               src/core/TransformPropagatorTests.cpp
                               src/math/AngleTests.cpp
                               src/math/BoxTests.cpp
                               src/math/IntersectionTests.cpp
                               src/math/Matrix3x3Tests.cpp
                               src/math/Matrix4x4Tests.cpp
                               src/math/PlaneTests.cpp
                               src/math/QuaternionTests.cpp
                               src/math/SphereTests.cpp
                               src/math/VectorTests.cpp
                               src/render/OrthographicViewVolumeTests.cpp
                               src/render/PerspectiveViewVolumeTests.cpp
        )

target_link_libraries(${PROJECT_NAME} ZeroCore
                                      gtest
                                      gtest_main)