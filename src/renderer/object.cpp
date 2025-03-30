#include <object.h>
#include <cassert>
#include <cmath>
#include <iostream>

namespace project {
namespace kernel {
    
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
