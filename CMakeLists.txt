cmake_minimum_required(VERSION 3.5)

project(3D-renderer)

add_executable(3D-renderer
    src/application/main.cpp 
    src/geometry/color.cpp
    src/geometry/object.cpp 
    src/geometry/pose.cpp
    src/pipeline/pipeline.cpp
    src/pipeline/screen.cpp
    src/world/world.cpp
    include/tgaimage/tgaimage.cpp
    )

target_include_directories(3D-renderer PRIVATE include/eigen-3.4.0)
target_include_directories(3D-renderer PRIVATE include/tgaimage)
target_include_directories(3D-renderer PRIVATE src/geometry)
target_include_directories(3D-renderer PRIVATE src/pipeline)
target_include_directories(3D-renderer PRIVATE src/world)
