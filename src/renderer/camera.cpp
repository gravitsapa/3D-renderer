#include <camera.h>
#include <cassert>

namespace project {
namespace kernel {
Camera::Camera(geometry::Coordinate near_plane, geometry::Coordinate far_plane,
               geometry::Coordinate left_side, geometry::Coordinate right_side,
               geometry::Coordinate top_side, geometry::Coordinate bottom_side)
    : near_plane_(near_plane),
      far_plane_(far_plane),
      left_side_(left_side),
      right_side_(right_side),
      top_side_(top_side),
      bottom_side_(bottom_side) {
    assert(near_plane_ > 0 && far_plane_ > near_plane_);
    assert(left_side_ > 0 && right_side_ > 0);
    assert(top_side_ > 0 && bottom_side_ > 0);
}

geometry::Matrix4d Camera::GetProjectionMatrix() const {
    auto n = near_plane_;
    auto f = far_plane_;
    auto l = -left_side_;
    auto r = right_side_;
    auto t = top_side_;
    auto b = -bottom_side_;
    return geometry::Matrix4d{{2 * n / (r - l), 0, (r + l) / (r - l), 0},
                              {0, 2 * n / (t - b), (t + b) / (t - b), 0},
                              {0, 0, -(f + n) / (f - n), -2 * n * f / (f - n)},
                              {0, 0, -1, 0}};
}

Vertex Camera::ProjectVertexOnMe(const Vertex& vertex) const {
    return Vertex{.point = geometry::ConvertToPoint(
                      GetProjectionMatrix() * geometry::ConvertToHomogeneousPoint(vertex.point)),
                  .normal = vertex.normal,
                  .text_coord = vertex.text_coord};
}

Face Camera::ProjectFaceOnMe(const Face& face) const {
    return Face{ProjectVertexOnMe(face.a), ProjectVertexOnMe(face.b), ProjectVertexOnMe(face.c)};
}

}  // namespace kernel
}  // namespace project
