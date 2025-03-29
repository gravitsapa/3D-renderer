cmake_minimum_required(VERSION 3.5)

project(3D-renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(include/SFML)
add_executable(3D-renderer
    src/renderer/color.cpp
    src/renderer/object.cpp 
    src/geometry/pose.cpp
    src/geometry/point.cpp
    src/renderer/camera.cpp
    src/renderer/renderer.cpp
    src/renderer/screen.cpp
    src/renderer/world.cpp
    src/renderer/file_reader.cpp
    src/application/application.cpp
    src/application/except.cpp 
    src/application/main.cpp 
    )

target_include_directories(3D-renderer PRIVATE include/eigen)
target_include_directories(3D-renderer PRIVATE src/application)
target_include_directories(3D-renderer PRIVATE src/geometry)
target_include_directories(3D-renderer PRIVATE src/renderer)
target_include_directories(3D-renderer PRIVATE src/except)
target_include_directories(3D-renderer PRIVATE src/structures)

target_link_libraries(3D-renderer PRIVATE sfml-graphics)
