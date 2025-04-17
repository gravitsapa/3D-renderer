#include <polygon.h>

namespace project {
namespace kernel {

Polygon EmptyPolygon() {
    return Polygon{{}};
}

Polygon PolygonByFace(const Face& face) {
    return Polygon{{face.a, face.b, face.c}};
}

std::vector<Face> TriangulatePolygon(const Polygon& polygon) {
    std::vector<Face> result;

    for (int index = 1; index + 1 < polygon.verticies.size(); ++index) {
        result.push_back(
            Face{polygon.verticies[0], polygon.verticies[index], polygon.verticies[index + 1]});
    }

    return result;
}

}  // namespace kernel
}  // namespace project
