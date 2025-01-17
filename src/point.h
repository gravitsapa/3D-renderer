#pragma once

#include "quantities.h"

struct Point;
struct Vector;

struct Point {
    Coordinate x_ = 0, y_ = 0, z_ = 0;

    Point operator+(const Vector& rhs) const;
};

struct Vector {
    Coordinate x_ = 0, y_ = 0, z_ = 0;
};
