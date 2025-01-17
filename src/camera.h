#pragma once

#include "point.h"

struct Camera {
    Point corner_;
    Vector height_;
    Vector width_;

    Point focus_point_;
};
