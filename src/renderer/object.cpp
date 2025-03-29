#include <object.h>
#include <cassert>
#include <cmath>
#include <iostream>

namespace project {
namespace kernel {

// нужно дописать алгоритм для нормали
Vertex WeightedSum(const Vertex& a, const Vertex& b, Factor alpha) {
    return Vertex{
        .point = (a.point * (1 - alpha) + b.point * alpha),
        .normal = a.normal,
        .text_coord = TextureCoordinates{a.text_coord.h * (1 - alpha) + b.text_coord.h * alpha,
                                         a.text_coord.w * (1 - alpha) + b.text_coord.w * alpha}};
}

ColoredVertex CreateColoredVertex(const Vertex& vertex, const Texture& texture) {
    return ColoredVertex{.point = vertex.point,
                         .normal = vertex.normal,
                         .col = texture.GetPixelColor(vertex.text_coord)};
}

TextureCoordinate Texture::MoveTo01Segment(TextureCoordinate x) const {
    assert(0 <= x && x <= 1);
    return x;
}

Texture::Texture() : data_(1, 1, Color::Random()) {
}

int Texture::ConvertToIndexH(TextureCoordinate h) const {
    h = MoveTo01Segment(h);
    return static_cast<int>(h * (data_.GetHeight() - 1));
}

int Texture::ConvertToIndexW(TextureCoordinate w) const {
    w = MoveTo01Segment(w);
    return static_cast<int>(w * (data_.GetWidth() - 1));
}

Color Texture::GetPixelColor(TextureCoordinates coord) const {
    assert(data_.GetHeight() > 0 && data_.GetWidth() > 0);
    return data_.Get(ConvertToIndexH(coord.h), ConvertToIndexW(coord.w));
}

const std::vector<Face>& Mesh3d::GetAllFaces() const {
    return faces_;
}

Mesh3d::Mesh3d(std::vector<Face> faces) : faces_(std::move(faces)) {
}

void PrintDebugInfo(const Object& object, const std::string& object_name) {
    std::cerr << "DEBUG INFO ABOUT OBJECT: " << object_name << std::endl;
    const auto& faces = object.mesh.GetAllFaces();
    std::cerr << "Polygons count: " << faces.size() << std::endl;
    geometry::Point3d min_point = faces[0].a.point;
    geometry::Point3d max_point = faces[0].a.point;
    for (auto& face : faces) {
        for (auto& ver : {face.a, face.b, face.c}) {
            min_point.x() = std::min(min_point.x(), ver.point.x());
            min_point.y() = std::min(min_point.y(), ver.point.y());
            min_point.z() = std::min(min_point.z(), ver.point.z());

            max_point.x() = std::max(max_point.x(), ver.point.x());
            max_point.y() = std::max(max_point.y(), ver.point.y());
            max_point.z() = std::max(max_point.z(), ver.point.z());
        }
    }
    std::cerr << "X from " << min_point.x() << " to " << max_point.x() << std::endl;
    std::cerr << "Y from " << min_point.y() << " to " << max_point.y() << std::endl;
    std::cerr << "Z from " << min_point.z() << " to " << max_point.z() << std::endl;
    auto col = object.texture.GetPixelColor(TextureCoordinates{0, 0});
    std::cerr << "Tex color: " << (int)col.r << ' ' << (int)col.g << ' ' << (int)col.b << std::endl;
}

}  // namespace kernel
}  // namespace project
