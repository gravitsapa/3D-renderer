#include <object.h>

void Object::AddPolygon(const Polygon& polygon) {
    polygons_.push_back(polygon);
}

const std::vector<Polygon>& Object::GetPolygons() const {
    return polygons_;
}

void Object::Clear() {
    polygons_.clear();
}
