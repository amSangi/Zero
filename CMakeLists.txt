cmake_minimum_required(VERSION 3.10)
project(Zero)

set(CMAKE_CXX_STANDARD 17)

set(ZERO_LIB_OUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(ZERO_EXE_OUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(ZERO_TEST_OUT_PATH ${CMAKE_SOURCE_DIR}/bin)


include_directories(include)
include_directories(include/core)
include_directories(include/math)
include_directories(thirdparty/entt/src/entt)

add_subdirectory(src)
add_subdirectory(thirdparty)
add_subdirectory(test)

##### Create Zero EXE #####

set(EXECUTABLE_OUTPUT_PATH ${ZERO_EXE_OUT_PATH})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ZeroCore)