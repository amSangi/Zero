project(ZeroTest)

set(EXECUTABLE_OUTPUT_PATH ${ZERO_TEST_OUT_PATH})

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/googletest/googletest-src/googletest/include)

add_executable(${PROJECT_NAME} TestMain.cpp
                               math/AngleTests.cpp
                               math/BoxTests.cpp
                               math/IntersectionTests.cpp
                               math/Matrix3Tests.cpp
                               math/Matrix4Tests.cpp
                               math/PlaneTests.cpp
                               math/QuaternionTests.cpp
                               math/SphereTests.cpp
                               math/VectorTests.cpp)

target_link_libraries(${PROJECT_NAME} ZeroCore)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)