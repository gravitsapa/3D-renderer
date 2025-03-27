#pragma once

#include <point.h>
#include <color.h>

namespace project {

namespace kernel {

struct Triangle {
    geometry::Point3d a;
    geometry::Point3d b;
    geometry::Point3d c;
    Color color;
};

}  // namespace geometry
}  // namespace project
