#pragma once

#include <point.h>
#include <color.h>

namespace project {

namespace kernel {

struct Polygon {
    geometry::Point3d a, b, c;
    Color color;
};

}  // namespace geometry
}  // namespace project
