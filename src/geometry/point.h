#pragma once

#include <quantities.h>

namespace project {
namespace geometry {

using Point3d = Vector3d;
using HomogeneousPoint3d = Vector4d;

Vector3d Normalized(const Vector3d& vector);

HomogeneousPoint3d ConvertToHomogeneousPoint(const Point3d& point);
Point3d ConvertToPoint(const HomogeneousPoint3d& point);

}

}  // namespace project
