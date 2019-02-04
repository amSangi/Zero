project(ZeroCore)

set(LIBRARY_OUTPUT_PATH ${ZERO_LIB_OUT_PATH})

add_library(${PROJECT_NAME}  math/Box.cpp
                             math/Intersection.cpp
                             math/Matrix3.cpp
                             math/Matrix4.cpp
                             math/Plane.cpp
                             math/Quaternion.cpp
                             math/Radian.cpp
                             math/Sphere.cpp
                             memory/PoolAllocator.cpp)