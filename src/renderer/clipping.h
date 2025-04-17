#pragma once

#include <polygon.h>
#include <plane.h>

namespace project {
namespace kernel {

Polygon ClipByPlane(Polygon polygon, const geometry::Plane& plane);
Polygon ClipByPlanes(Polygon polygon, const std::vector<geometry::Plane>& planes);

}  // namespace kernel
}  // namespace project
