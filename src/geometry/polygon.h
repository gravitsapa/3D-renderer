#pragma once

#include <point.h>
#include <color.h>

namespace geometry {

struct Polygon {
    Point3d a, b, c;
    Color color;
};

}  // namespace geometry
