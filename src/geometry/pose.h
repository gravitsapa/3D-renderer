#pragma once

#include <quantities.h>
#include <cmath>

namespace geometry {

struct Rotation {
    Matrix3d rot_matrix = Matrix3d::Identity();

    static Rotation ByAngles(const Coordinate& alpha, const Coordinate& beta,
                             const Coordinate& gamma);
};

struct Position {
    Vector3d pos_vector = Vector3d::Zero();
};

struct Pose {
    Position pos_;
    Rotation rot_;
};

}  // namespace geometry
