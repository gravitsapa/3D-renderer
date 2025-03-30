#pragma once
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <point.h>
#include <object.h>

namespace project {
namespace kernel {

Mesh3d ReadMeshFromFile(const std::string& filename);

Texture ReadTextureFromFile(const std::string& filename);

namespace detail {

std::ifstream OpenFileStream(const std::string& filename);

std::string ReadString(std::stringstream& stream);

geometry::Coordinate ReadCoordinate(std::stringstream& stream);

geometry::Point3d ReadPoint3D(std::stringstream& stream);

geometry::Vector3d ReadVector3D(std::stringstream& stream);

TextureCoordinates ReadTextureCoordinates(std::stringstream& stream);

bool LineIsNotEmpty(std::stringstream& stream);

bool NextSymbolIs(std::stringstream& stream, char symbol);

struct ObjFileVertexIndexes {
    using Index = int;
    Index point_index;
    Index texture_index;
    Index normal_index;
};

ObjFileVertexIndexes ReadVertexIndexes(std::stringstream& stream);

Vertex CreateVectexFromIndexes(ObjFileVertexIndexes indexes,
                               const std::vector<geometry::Point3d>& points,
                               const std::vector<geometry::Vector3d>& normals,
                               const std::vector<TextureCoordinates>& tex_coords);
}  // namespace detail

}  // namespace kernel
}  // namespace project
