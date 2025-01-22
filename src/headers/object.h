#pragma once

#include <polygon.h>
#include <vector>

class Object {
public:
    void AddPolygon(const Polygon& polygon);

    const std::vector<Polygon>& GetPolygons() const;

    void Clear();

private:
    std::vector<Polygon> polygons_;
};
