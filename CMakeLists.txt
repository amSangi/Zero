cmake_minimum_required(VERSION 3.10)
project(Zero)

set(CMAKE_CXX_STANDARD 17)

set(ZERO_LIB_OUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(ZERO_EXE_OUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(ZERO_TEST_OUT_PATH ${CMAKE_SOURCE_DIR}/bin)


include_directories(include)
include_directories(include/math)

add_subdirectory(src)
add_subdirectory(thirdparty)
add_subdirectory(test)

##### Create Zero EXE #####

set(EXECUTABLE_OUTPUT_PATH ${ZERO_EXE_OUT_PATH})

# Set to local Qt5 cmake config path
set(Qt5_DIR /Users/sangi/Applications/5.12.0/clang_64/lib/cmake/Qt5)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core)
target_link_libraries(${PROJECT_NAME} ZeroCore)