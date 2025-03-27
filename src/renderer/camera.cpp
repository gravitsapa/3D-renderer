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

geometry::Point3d Camera::ProjectPointOnMe(const geometry::Point3d& point) const {
    auto n = near_plane_;
    auto f = far_plane_;
    auto l = -left_side_;
    auto r = right_side_;
    auto t = top_side_;
    auto b = -bottom_side_;
    geometry::Matrix4d project_matrix{{2 * n / (r - l), 0, (r + l) / (r - l), 0},
                                      {0, 2 * n / (t - b), (t + b) / (t - b), 0},
                                      {0, 0, -(f + n) / (f - n), -2 * n * f / (f - n)},
                                      {0, 0, -1, 0}};
    geometry::Vector4d p = project_matrix * geometry::Vector4d{point.x(), point.y(), point.z(), 1};
    auto w = p(3, 0);
    return {p.x() / w, p.y() / w, p.z() / w};
}

geometry::Coordinate Camera::GetDepth() const {
    return far_plane_ - near_plane_;
}

}  // namespace kernel
}  // namespace project
