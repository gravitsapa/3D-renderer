#pragma once

#include <object.h>
#include <plane.h>

namespace project {
namespace kernel {

struct Camera {
public:
    Camera(geometry::Coordinate near_plane, geometry::Coordinate far_plane,
           geometry::Coordinate left_side, geometry::Coordinate right_side,
           geometry::Coordinate top_side, geometry::Coordinate bottom_side);

    Vertex ProjectVertexOnMe(const Vertex& vertex) const;
    Face ProjectFaceOnMe(const Face& face) const;

    std::vector<Face> Clip(const Face& face) const;

private:
    geometry::Matrix4d GetProjectionMatrix() const;
    std::vector<geometry::Plane> GetPlanes() const;

    geometry::Coordinate near_plane_;
    geometry::Coordinate far_plane_;
    geometry::Coordinate left_side_;
    geometry::Coordinate right_side_;
    geometry::Coordinate top_side_;
    geometry::Coordinate bottom_side_;
};

}  // namespace kernel
}  // namespace project
