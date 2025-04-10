#pragma once

#include <point.h>
#include <segment.h>

namespace project {
namespace geometry {

class Plane {
public:
    Plane(Coordinate a, Coordinate b, Coordinate c, Coordinate d);

    bool IsInHalfPlain(const Point3d& point) const;
    bool IsStrictlyInHalfPlain(const Point3d& point) const;
    bool IsStrictlyIntersectSegment(const Segment& segment) const;
    Coordinate IntersectWithSegment(const Segment& segment) const;

private:
    Coordinate Equation(const Point3d& point) const;

    Vector3d normal_;
    Coordinate d_;
};

}  // namespace geometry
}  // namespace project
