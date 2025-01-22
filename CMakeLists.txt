cmake_minimum_required(VERSION 3.5)

project(3D-renderer)

add_executable(3D-renderer
    src/main.cpp
    src/object.cpp
    src/world.cpp
    src/quantities.cpp
    include/tgaimage/tgaimage.cpp
    )

target_include_directories(3D-renderer PRIVATE include/eigen-3.4.0)
target_include_directories(3D-renderer PRIVATE include/tgaimage)
target_include_directories(3D-renderer PRIVATE src/headers)
