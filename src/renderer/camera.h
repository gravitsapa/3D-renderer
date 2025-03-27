#pragma once

#include <point.h>

namespace project {
namespace kernel {

struct Camera {
    geometry::Coordinate NearPlane;
    geometry::Coordinate FarPlane;
    geometry::Coordinate LeftSide;
    geometry::Coordinate RightSide;
    geometry::Coordinate TopSide;
    geometry::Coordinate BottomSide;
};

}  // namespace kernel
}  // namespace project
