#pragma once

#include <point.h>
#include <texture.h>

namespace project {
namespace kernel {

struct Vertex {
    geometry::Point3d point;
    geometry::Vector3d normal;
    TextureCoordinates text_coord;
};

}  // namespace kernel
}  // namespace project
