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
    friend Texture ReadTextureFromFile(const std::string& filename);

public:
    Texture();

    Color GetPixelColor(TextureCoordinates coord) const;

private:
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

geometry::Vector3d NormalToFace(const Face& face);

class Mesh3d {
    friend Mesh3d ReadMeshFromFile(const std::string& filename);

public:
    const std::vector<Face>& GetAllFaces() const;

    static Mesh3d RectangularСuboid(const geometry::Point3d& center, geometry::Coordinate w, geometry::Coordinate h,
                                     geometry::Coordinate d);

    static Mesh3d RectangularСuboid(geometry::Coordinate w, geometry::Coordinate h, geometry::Coordinate d);

    static Mesh3d Parallelepiped(const geometry::Point3d& center, const geometry::Vector3d& w, const geometry::Vector3d& h,
                                  const geometry::Vector3d d);

    static Mesh3d Rectangle(const geometry::Point3d& center, const geometry::Vector3d& h, const geometry::Vector3d& w);

    static Mesh3d Triangle(const geometry::Point3d& a, const geometry::Point3d& b, const geometry::Point3d& c);

    void Insert(Mesh3d&& rhs);

private:
    Mesh3d() = default;
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
