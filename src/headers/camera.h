#pragma once

#include <point.h>

struct Camera {
    Coordinate NearPlane;
    Coordinate FarPlane;
    Coordinate LeftSide;
    Coordinate RightSide;
    Coordinate TopSide;
    Coordinate BottomSide;
};
