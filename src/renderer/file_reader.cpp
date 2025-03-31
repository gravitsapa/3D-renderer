#include <file_reader.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <point.h>
#include <object.h>
#include <type_traits>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

namespace project {
namespace kernel {

Mesh3d ReadMeshFromFile(const std::string& filename) {
    std::ifstream obj_file = detail::OpenFileStream(filename);

    std::vector<geometry::Point3d> points;
    std::vector<geometry::Vector3d> normals;
    std::vector<TextureCoordinates> tex_coords;
    std::vector<Face> faces;

    std::string line;
    while (getline(obj_file, line)) {
        std::stringstream stream;
        stream << line;

        std::string type = detail::ReadString(stream);
        if (type == "v") {
            points.push_back(detail::ReadPoint3D(stream));
        } else if (type == "vn") {
            normals.push_back(detail::ReadVector3D(stream));
        } else if (type == "vt") {
            tex_coords.push_back(detail::ReadTextureCoordinates(stream));
        } else if (type == "f") {
            std::vector<Vertex> vertices;
            while (detail::LineIsNotEmpty(stream)) {
                auto indexes = detail::ReadVertexIndexes(stream);
                vertices.push_back(
                    detail::CreateVectexFromIndexes(indexes, points, normals, tex_coords));
            }

            if (vertices.size() < 3) {
                throw std::runtime_error("Face contains to few vertices in obj file");
            }

            for (int i = 1; i + 1 < vertices.size(); ++i) {
                faces.push_back(Face{vertices[0], vertices[i], vertices[i + 1]});
            }
        }
    }

    return Mesh3d(faces);
}

Texture ReadTextureFromFile(const std::string& filename) {
    auto image = cv::imread(filename, cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error("Cannot read texture");
    }

    Texture texture;
    texture.data_ = structures::Table<Color>(image.rows, image.cols);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            cv::Vec3b bgr_pixel = image.at<cv::Vec3b>(i, j);
            texture.data_.Get(i, j) =
                ColorByChar(bgr_pixel.val[2], bgr_pixel.val[1], bgr_pixel.val[0]);
        }
    }

    return texture;
}

namespace detail {

std::ifstream OpenFileStream(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open obj file: " + filename);
    }
    return file;
}

std::string ReadString(std::stringstream& stream) {
    std::string str;
    stream >> str;
    return str;
}

geometry::Coordinate ReadCoordinate(std::stringstream& stream) {
    geometry::Coordinate coord;
    stream >> coord;
    return coord;
}

geometry::Point3d ReadPoint3D(std::stringstream& stream) {
    geometry::Coordinate x = ReadCoordinate(stream);
    geometry::Coordinate y = ReadCoordinate(stream);
    geometry::Coordinate z = ReadCoordinate(stream);
    return geometry::Point3d(x, y, z);
}

geometry::Vector3d ReadVector3D(std::stringstream& stream) {
    geometry::Coordinate x = ReadCoordinate(stream);
    geometry::Coordinate y = ReadCoordinate(stream);
    geometry::Coordinate z = ReadCoordinate(stream);
    return geometry::Vector3d(x, y, z);
}

TextureCoordinates ReadTextureCoordinates(std::stringstream& stream) {
    TextureCoordinates coordinates;
    stream >> coordinates.x() >> coordinates.y();
    coordinates.x() -= std::floor(coordinates.x());
    coordinates.y() -= std::floor(coordinates.y());

    coordinates.y() = 1 - coordinates.y();
    std::swap(coordinates.x(), coordinates.y());

    return coordinates;
}

bool LineIsNotEmpty(std::stringstream& stream) {
    return stream.peek() != std::stringstream::traits_type::eof();
}

bool NextSymbolIs(std::stringstream& stream, char symbol) {
    return stream.peek() == symbol;
}

ObjFileVertexIndexes ReadVertexIndexes(std::stringstream& stream) {
    ObjFileVertexIndexes indexes;
    char slash;
    stream >> indexes.point_index;
    stream >> slash;
    if (!NextSymbolIs(stream, '/')) {
        stream >> indexes.texture_index;
    } else {
        indexes.texture_index = 0;
    }
    stream >> slash;
    if (LineIsNotEmpty(stream)) {
        stream >> indexes.normal_index;
    } else {
        indexes.normal_index = 0;
    }
    return indexes;
}

Vertex CreateVectexFromIndexes(ObjFileVertexIndexes indexes,
                               const std::vector<geometry::Point3d>& points,
                               const std::vector<geometry::Vector3d>& normals,
                               const std::vector<TextureCoordinates>& tex_coords) {
    Vertex vertex;

    if (indexes.point_index <= 0 || indexes.point_index > points.size()) {
        throw std::runtime_error("Incorrect point index in obj file");
    }
    vertex.point = points[indexes.point_index - 1];

    if (indexes.texture_index < 0 || indexes.texture_index > tex_coords.size()) {
        throw std::runtime_error("Incorrect texture index in obj file");
    }
    if (indexes.texture_index == 0) {
        vertex.text_coord = TextureCoordinates{0, 0};
    } else {
        vertex.text_coord = tex_coords[indexes.texture_index - 1];
    }

    if (indexes.normal_index < 0 || indexes.normal_index > normals.size()) {
        throw std::runtime_error("Incorrect normal index in obj file");
    }
    if (indexes.normal_index == 0) {
        vertex.normal = geometry::Vector3d{0, 0, 1};
    } else {
        vertex.normal = normals[indexes.normal_index - 1];
    }

    return vertex;
}
}  // namespace detail

}  // namespace kernel
}  // namespace project
