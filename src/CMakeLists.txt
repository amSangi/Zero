# Set to local Qt5 cmake config path 
set(Qt5_DIR /Users/sangi/Applications/5.12.0/clang_64/lib/cmake/Qt5)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)


add_executable(${PROJECT_NAME}  main.cpp
                                math/Matrix3.cpp
                                math/Matrix4.cpp
        )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core)