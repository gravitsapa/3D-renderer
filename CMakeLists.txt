cmake_minimum_required(VERSION 3.5)

project(3D-renderer)

add_executable(3D-renderer
    src/application/main.cpp 
    src/geometry/color.cpp
    src/geometry/object.cpp 
    src/geometry/pose.cpp
    src/renderer/renderer.cpp
    src/renderer/screen.cpp
    src/world/world.cpp
    src/application/application.cpp
    include/tgaimage/tgaimage.cpp
    )

target_include_directories(3D-renderer PRIVATE include/eigen)
target_include_directories(3D-renderer PRIVATE include/tgaimage)
target_include_directories(3D-renderer PRIVATE src/application)
target_include_directories(3D-renderer PRIVATE src/geometry)
target_include_directories(3D-renderer PRIVATE src/renderer)
target_include_directories(3D-renderer PRIVATE src/world)
