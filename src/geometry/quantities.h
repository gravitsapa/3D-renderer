#pragma once

#include <Eigen/Dense>

namespace geometry {

typedef double Coordinate;
typedef Eigen::Matrix<Coordinate, 3, 1> Vector3d;
typedef Eigen::Matrix<Coordinate, 3, 3> Matrix3d;
typedef Eigen::Matrix<Coordinate, 4, 1> Vector4d;
typedef Eigen::Matrix<Coordinate, 4, 4> Matrix4d;

}  // namespace geometry