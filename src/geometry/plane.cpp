#include <plane.h>

namespace project {
namespace geometry {

Plane::Plane(Coordinate a, Coordinate b, Coordinate c, Coordinate d) : normal_(a, b, c), d_(d) {
}

Coordinate Plane::Equation(const Point3d& point) const {
    return point.dot(normal_) + d_;
}

bool Plane::IsInHalfPlain(const Point3d& point) const {
    return GreaterOrEqual(Equation(point), 0.0);
}

bool Plane::IsStrictlyInHalfPlain(const Point3d& point) const {
    return Greater(Equation(point), 0.0);
}

bool Plane::IsStrictlyIntersectSegment(const Segment& segment) const {
    Coordinate value_a = Equation(segment.a);
    Coordinate value_b = Equation(segment.b);

    return (Less(value_a, 0.0) && Greater(value_b, 0.0)) ||
           (Greater(value_a, 0.0) && Less(value_b, 0.0));
}

Coordinate Plane::IntersectWithSegment(const Segment& segment) const {
    return -(d_ + segment.a.dot(normal_)) / normal_.dot(segment.b - segment.a);
}

}  // namespace geometry
}  // namespace project
