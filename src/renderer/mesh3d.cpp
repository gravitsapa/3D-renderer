#include <mesh3d.h>

namespace project {
namespace kernel {

const std::vector<Face>& Mesh3d::GetAllFaces() const {
    return faces_;
}

Mesh3d::Mesh3d(std::vector<Face> faces) : faces_(std::move(faces)) {
}

Mesh3d Mesh3d::RectangularСuboid(const geometry::Point3d& center, geometry::Coordinate w,
                                 geometry::Coordinate h, geometry::Coordinate d) {
    w /= 2;
    h /= 2;
    d /= 2;

    return Parallelepiped(center, {w, 0, 0}, {0, h, 0}, {0, 0, d});
}

Mesh3d Mesh3d::RectangularСuboid(geometry::Coordinate w, geometry::Coordinate h,
                                 geometry::Coordinate d) {
    return RectangularСuboid(geometry::Point3d(), w, h, d);
}

Mesh3d Mesh3d::Parallelepiped(const geometry::Point3d& center, const geometry::Vector3d& w,
                              const geometry::Vector3d& h, const geometry::Vector3d d) {
    Mesh3d res;
    res.Insert(Rectangle(center + w, h, d));
    res.Insert(Rectangle(center - w, h, d));
    res.Insert(Rectangle(center + h, w, d));
    res.Insert(Rectangle(center - h, w, d));
    res.Insert(Rectangle(center + d, h, w));
    res.Insert(Rectangle(center - d, h, w));
    return res;
}

Mesh3d Mesh3d::Rectangle(const geometry::Point3d& center, const geometry::Vector3d& h,
                         const geometry::Vector3d& w) {
    Mesh3d res;
    res.Insert(Triangle(center + h + w, center - h - w, center + h - w));
    res.Insert(Triangle(center + h + w, center - h - w, center - h + w));
    return res;
}

Mesh3d Mesh3d::Triangle(const geometry::Point3d& a, const geometry::Point3d& b,
                        const geometry::Point3d& c) {
    Mesh3d res;
    auto default_normal = geometry::Vector3d{0, 0, 1};
    auto empty_tex_coord = TextureCoordinates{0, 0};
    res.faces_.push_back(
        Face{Vertex{.point = a, .normal = default_normal, .text_coord = empty_tex_coord},
             Vertex{.point = b, .normal = default_normal, .text_coord = empty_tex_coord},
             Vertex{.point = c, .normal = default_normal, .text_coord = empty_tex_coord}});
    return res;
}

void Mesh3d::Insert(Mesh3d&& rhs) {
    faces_.insert(faces_.end(), rhs.faces_.begin(), rhs.faces_.end());
}

}  // namespace kernel
}  // namespace project
