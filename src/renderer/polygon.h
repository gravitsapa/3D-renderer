#pragma once

#include <vector>
#include <vertex.h>
#include <face.h>

namespace project {
namespace kernel {

struct Polygon {
    std::vector<Vertex> verticies;
};

Polygon EmptyPolygon();
Polygon PolygonByFace(const Face& face);
std::vector <Face> TriangulatePolygon(const Polygon& polygon);

}  // namespace kernel
}  // namespace project
