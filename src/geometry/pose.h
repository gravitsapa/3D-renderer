#pragma once

#include <quantities.h>
#include <cmath>

namespace project {
namespace geometry {

struct Rotation {
    Matrix3d rot_matrix = Matrix3d::Identity();

    static Rotation ByAngles(const Coordinate& alpha, const Coordinate& beta,
                             const Coordinate& gamma);
};

struct Position {
    Vector3d pos_vector = Vector3d::Zero();
};

struct Pose : Rotation, Position {
    Pose() = default;

    Pose(const Position& pos, const Rotation& rot);
};

}  // namespace geometry
}  // namespace project
