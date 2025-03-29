#pragma once

#include <vector>
#include <color.h>
#include <table.h>
#include <string>
#include <point.h>

namespace project {
namespace kernel {

using TextureCoordinate = double;

struct TextureCoordinates {
    TextureCoordinate h;
    TextureCoordinate w;
};

class Texture {
public:
    Texture();

    Color GetPixelColor(TextureCoordinates coord) const;

private:
    TextureCoordinate MoveTo01Segment(TextureCoordinate x) const;
    int ConvertToIndexH(TextureCoordinate h) const;
    int ConvertToIndexW(TextureCoordinate w) const;

    structures::Table<Color> data_;
};

struct Vertex {
    geometry::Point3d point;
    geometry::Vector3d normal;
    TextureCoordinates text_coord;
};

using Factor = double;
Vertex WeightedSum(const Vertex& a, const Vertex& b, Factor alpha);

struct ColoredVertex {
    geometry::Point3d point;
    geometry::Vector3d normal;
    Color col;
};

ColoredVertex CreateColoredVertex(const Vertex& vertex, const Texture& texture);

struct Face {
    Vertex a;
    Vertex b;
    Vertex c;
};

class Mesh3d {
    friend Mesh3d ReadMeshFromFile(const std::string& filename);

public:
    const std::vector<Face>& GetAllFaces() const;

private:
    Mesh3d(std::vector<Face> faces);
    std::vector<Face> faces_;
};

struct Object {
    Mesh3d mesh;
    Texture texture;
};

void PrintDebugInfo(const Object& object, const std::string& object_name);

}  // namespace kernel
}  // namespace project
