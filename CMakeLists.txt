cmake_minimum_required(VERSION 3.10)
project(Zero)

set(CMAKE_CXX_STANDARD 17)

## Output ##
set(ZERO_LIB_OUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(ZERO_EXE_OUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(ZERO_TEST_OUT_PATH ${CMAKE_SOURCE_DIR}/bin)


## Package Dependencies ##
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


## Header Includes ##
include_directories(include)
include_directories(thirdparty/entt/src/entt)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})


## Subdirectories ##
add_subdirectory(src)
add_subdirectory(thirdparty)
add_subdirectory(test)


## Create Zero EXE ##
set(EXECUTABLE_OUTPUT_PATH ${ZERO_EXE_OUT_PATH})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ZeroCore)
target_link_libraries(${PROJECT_NAME} EnTT)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})