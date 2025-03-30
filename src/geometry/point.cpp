#include <point.h>
#include <cassert>
#include <iostream>

namespace project {
namespace geometry {

Vector3d Normalized(const Vector3d& vector) {
    return vector / vector.norm();
}

HomogeneousPoint3d ConvertToHomogeneousPoint(const Point3d& point) {
    return HomogeneousPoint3d{point.x(), point.y(), point.z(), 1};
}

Point3d ConvertToPoint(const HomogeneousPoint3d& point) {
    Coordinate w = point(3, 0);
    assert(w != 0 && "W coordinate must not be zero");
    return Point3d{point.x() / w, point.y() / w, point.z() / w};
}

}  // namespace geometry
}  // namespace project
