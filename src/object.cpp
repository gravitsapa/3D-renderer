#include "object.h"

void Object::AddPolygon(const Polygon& polygon) {
    polygons_.push_back(polygon);
}

void Object::Clear() {
    polygons_.clear();
}
