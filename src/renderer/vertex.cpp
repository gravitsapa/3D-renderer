#include <vertex.h>

namespace project {
namespace kernel {

Vertex operator+(const Vertex& lhs, const Vertex& rhs) {
    return Vertex{lhs.point + rhs.point, lhs.normal + rhs.normal, lhs.text_coord + rhs.text_coord};
}

Vertex operator-(const Vertex& lhs, const Vertex& rhs) {
    return Vertex{lhs.point - rhs.point, lhs.normal - rhs.normal, lhs.text_coord - rhs.text_coord};
}

Vertex operator*(const Vertex& lhs, geometry::Coordinate rhs) {
    return Vertex{lhs.point * rhs, lhs.normal * rhs, lhs.text_coord * rhs};
}

}  // namespace kernel
}  // namespace project
