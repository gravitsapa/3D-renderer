#pragma once

#include <Eigen/Dense>

namespace project {
namespace geometry {

using Coordinate = double;
using Vector2d = Eigen::Vector2<Coordinate>;
using Matrix2d = Eigen::Matrix<Coordinate, 2, 2>;
using Vector3d = Eigen::Vector3<Coordinate>;
using Matrix3d = Eigen::Matrix<Coordinate, 3, 3>;
using Vector4d = Eigen::Vector4<Coordinate>;
using Matrix4d = Eigen::Matrix<Coordinate, 4, 4>;


constexpr double eps = 1e-7;
bool Equal(Coordinate lhs, Coordinate rhs);
bool GreaterOrEqual(Coordinate lhs, Coordinate rhs);
bool Greater(Coordinate lhs, Coordinate rhs);
bool LessOrEqual(Coordinate lhs, Coordinate rhs);
bool Less(Coordinate lhs, Coordinate rhs);


}  // namespace geometry
}  // namespace project
