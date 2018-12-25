project(ZeroTest)

set(EXECUTABLE_OUTPUT_PATH ${ZERO_TEST_OUT_PATH})

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/googletest/googletest-src/googletest/include)

add_executable(${PROJECT_NAME} AllTests.cpp
                               math/VectorTests.cpp)

target_link_libraries(${PROJECT_NAME} ZeroCore)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)