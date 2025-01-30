#pragma once

#include <point.h>

namespace scene {
    
using namespace geometry;

struct Camera {
    Coordinate NearPlane;
    Coordinate FarPlane;
    Coordinate LeftSide;
    Coordinate RightSide;
    Coordinate TopSide;
    Coordinate BottomSide;
};

}  // namespace scene