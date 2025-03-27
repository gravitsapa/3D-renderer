#pragma once

#include <quantities.h>

namespace project {
namespace geometry {

using Point3d = Vector3d;
using HomogeneousPoint3d = Vector4d;

HomogeneousPoint3d ConvertToHomogeneousPoint(const Point3d& point);
Point3d ConvertToPoint(const HomogeneousPoint3d& point);

}

}  // namespace project
