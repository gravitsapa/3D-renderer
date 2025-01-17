#include "point.h"

Point Point::operator+(const Vector& rhs) const {
    return {x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_};
}
