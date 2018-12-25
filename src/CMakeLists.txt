project(ZeroCore)

set(LIBRARY_OUTPUT_PATH ${ZERO_LIB_OUT_PATH})

add_library(${PROJECT_NAME}  math/Matrix3.cpp
                             math/Matrix4.cpp
                             math/Quaternion.cpp)