#pragma once

#include <quantities.h>
#include <cmath>

namespace project {
namespace geometry {

struct Rotation {
    static Rotation ByAngles(const Coordinate& alpha, const Coordinate& beta,
                             const Coordinate& gamma);
                             
    Matrix3d rot_matrix = Matrix3d::Identity();
};

struct Position {
    Vector3d pos_vector = Vector3d::Zero();
};

struct Pose : Rotation, Position {
};

}  // namespace geometry
}  // namespace project
