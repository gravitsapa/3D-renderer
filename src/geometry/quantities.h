#pragma once

#include <Eigen/Dense>

namespace geometry {

using Coordinate = double;
using Vector3d = Eigen::Matrix<Coordinate, 3, 1>;
using Matrix3d = Eigen::Matrix<Coordinate, 3, 3>;
using Vector4d = Eigen::Matrix<Coordinate, 4, 1>;
using Matrix4d = Eigen::Matrix<Coordinate, 4, 4>;

}  // namespace geometry
