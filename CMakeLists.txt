cmake_minimum_required(VERSION 3.5)

project(3D-renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(include/SFML)
add_executable(3D-renderer
    src/renderer/color.cpp
    src/renderer/object.cpp 
    src/geometry/pose.cpp
    src/geometry/point.cpp
    src/geometry/geom_calc.cpp
    src/geometry/plane.cpp
    src/geometry/quantities.cpp
    src/renderer/camera.cpp
    src/renderer/renderer.cpp
    src/renderer/screen.cpp
    src/renderer/world.cpp
    src/renderer/light.cpp
    src/renderer/texture.cpp
    src/renderer/mesh3d.cpp
    src/renderer/file_reader.cpp
    src/renderer/zbuffer.cpp
    src/renderer/raster_coordinates.cpp
    src/renderer/rasterizer.cpp
    src/renderer/interpolator.cpp
    src/renderer/clipping.cpp
    src/renderer/polygon.cpp
    src/renderer/vertex.cpp
    src/application/arg_parser.cpp
    src/application/application.cpp
    src/application/user_manual.cpp
    src/application/except.cpp 
    src/application/timer.cpp
    src/application/main.cpp 
    )

target_include_directories(3D-renderer PRIVATE include/eigen)
target_include_directories(3D-renderer PRIVATE src/application)
target_include_directories(3D-renderer PRIVATE src/geometry)
target_include_directories(3D-renderer PRIVATE src/renderer)
target_include_directories(3D-renderer PRIVATE src/except)
target_include_directories(3D-renderer PRIVATE src/structures)

target_link_libraries(3D-renderer PRIVATE sfml-graphics)
target_link_libraries(3D-renderer PRIVATE ${OpenCV_LIBS} )
