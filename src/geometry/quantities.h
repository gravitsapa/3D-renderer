#pragma once

#include <Eigen/Dense>

namespace project {
namespace geometry {

using Coordinate = double;
using Vector3d = Eigen::Vector3<Coordinate>;
using Matrix3d = Eigen::Matrix<Coordinate, 3, 3>;
using Vector4d = Eigen::Vector4<Coordinate>;
using Matrix4d = Eigen::Matrix<Coordinate, 4, 4>;

}  // namespace geometry
}  // namespace project
