cmake_minimum_required(VERSION 3.10)
project(Zero)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/math)

add_subdirectory(src)
