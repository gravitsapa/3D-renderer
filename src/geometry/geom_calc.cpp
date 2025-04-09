#include <geom_calc.h>

namespace project {
namespace geometry {

Vector3d NormalToTriangle(const Point3d& a, const Point3d& b, const Point3d& c) {
    return (b - a).cross(c - a);
}

}  // namespace geometry
}  // namespace project
