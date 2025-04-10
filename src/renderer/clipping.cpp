#include <clipping.h>
#include <cassert>

namespace project {
namespace kernel {

Polygon ClipByPlane(Polygon polygon, const geometry::Plane& plane) {
    assert(polygon.verticies.size() >= 3);

    int index = 0;
    while (index < polygon.verticies.size()) {
        const auto& a = polygon.verticies[index];
        int next_index = index + 1 < polygon.verticies.size() ? index + 1 : 0;
        const auto& b = polygon.verticies[next_index];
        auto segment = geometry::Segment{a.point, b.point};

        if (!plane.IsStrictlyIntersectSegment(segment)) {
            index++;
            continue;
        }

        geometry::Coordinate factor = plane.IntersectWithSegment(segment);
        polygon.verticies.insert(polygon.verticies.begin() + index + 1, a + (b - a) * factor);
        index += 2;
    }

    Polygon result;
    for (auto& vertex : polygon.verticies) {
        if (plane.IsInHalfPlain(vertex.point)) {
            result.verticies.push_back(vertex);
        }
    }

    return result;
}

Polygon ClipByPlanes(Polygon polygon, const std::vector<geometry::Plane>& planes) {
    for (const auto& plane : planes) {
        if (polygon.verticies.size() <= 2) {
            return EmptyPolygon();
        }

        polygon = ClipByPlane(polygon, plane);
    }

    if (polygon.verticies.size() <= 2) {
        return EmptyPolygon();
    }
    return polygon;
}

}  // namespace kernel
}  // namespace project
