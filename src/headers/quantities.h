#pragma once

#include <Eigen/Dense>

typedef double Coordinate;
typedef Eigen::Matrix<Coordinate, 3, 1> Vector3d;
typedef Eigen::Matrix<Coordinate, 3, 3> Matrix3d;
typedef Eigen::Matrix<Coordinate, 4, 1> Vector4d;
typedef Eigen::Matrix<Coordinate, 4, 4> Matrix4d;

Matrix3d EyeMatrix3d();

Vector3d ZeroVector3d();