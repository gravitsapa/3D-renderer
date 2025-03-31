#pragma once

#include <quantities.h>

namespace project {
namespace geometry {

using Point3d = Vector3d;
using Point2d = Vector2d;
using HomogeneousPoint3d = Vector4d;

Vector3d Normalized(const Vector3d& vector);

HomogeneousPoint3d ConvertToHomogeneousPoint(const Point3d& point);
Point3d ConvertToPoint(const HomogeneousPoint3d& point);

Point2d TruncZ(const Point3d& point);

}  // namespace geometry

}  // namespace project
